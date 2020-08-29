
#include "VulkanApplication.h"
#include <stdexcept>
#include "GraphicsPipeline.h"
#include "Framebuffer.h"
#include "CommandPool.h"
#include "CommandBuffers.h"
#include "RenderPass.h"
#include "Semaphore.h"
#include "Fence.h"
#include "UniformBuffer.h"
#include "Camera.h"

//temp?
#include "DepthBuffer.h"
#include "Buffer.h"
#include "Scene.h"

#include "DescriptorSet.h"
#include "DescriptorSetLayout.h"

VulkanApplication* VulkanApplication::appInstance = nullptr;

const int MAX_FRAMES_IN_FLIGHT = 2;

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto app = (VulkanApplication*)(glfwGetWindowUserPointer(window));
	app->frambufferResized = true;
}

void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
}	

VulkanApplication::VulkanApplication() 
{
	appInstance = this;

	validationLayers = enableValidationLayers
		? std::vector<const char*>{"VK_LAYER_KHRONOS_validation"}
		: std::vector<const char*>();

	if (!checkValidationLayerSupport(validationLayers))
		throw std::runtime_error("Validation layers requested, but not available!");
}

VulkanApplication::~VulkanApplication()
{
	cleanup();
}

void VulkanApplication::run()
{
	initWindow();
	initVulkan();
	initApp();
	mainLoop();
}

void VulkanApplication::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "VkEngine", nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

	//glfwSetKeyCallback(window, onKeyCallback);
}

void VulkanApplication::initVulkan() 
{
	currentFrame_ = 0;

	// TODO: Revise swap chain creation order (function + create swap chain object after logical device?)
	// This way we could reduce the createSwapChain function (and instantiate the actual scObject there instead of before)
	createInstance();
	setupDebugMessenger();
	createSurface();
	swapChain_ = new VulkanSwapChain();
	device_ = new VulkanDevice(&instance);
	commandPool_ = new CommandPool(*device_, device_->Indices().graphicsFamily.value(), true);
	createSwapChain();
	createGraphicsPipeline();
}

void VulkanApplication::initApp()
{
	camera_ = new Camera();
	//camera_->eye_ = glm::vec3(20.0f, 20.0f, 20.0f);
	//camera_->setPerspective(45.0f, (float)swapChain_->Extent().width / (float)swapChain_->Extent().height, 0.1f, 1000.0f);
	//camera_->center_ = glm::vec3(0, 0, 0);
	//camera_->up_ = glm::vec3(0, 0, 1);
	//camera_->matrices_.view = glm::lookAt(glm::vec3(20.0f, 20.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//camera_->matrices_.projection = glm::perspective(glm::radians(45.0f), swapChain_->Extent().width / (float)swapChain_->Extent().height, 0.1f, 1000.0f);
}

void VulkanApplication::mainLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		deltaTime_ = currentTime - lastTime_;
		lastTime_ = currentTime;
		glfwPollEvents();
		drawFrame();
		input(window);
	}
	vkDeviceWaitIdle(device_->Handle());
}

uint32_t VulkanApplication::findMemoryTypes(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(device_->GPU(), &memProperties);
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}
	throw std::runtime_error("Failed to find suitable memory type!");
}

void VulkanApplication::render(VkCommandBuffer commandBuffer, const uint32_t imageIndex)
{
	std::array<VkClearValue, 2> clearColor;
	clearColor[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	clearColor[1].depthStencil = { 1.0f, 0 };

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType				= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass			= graphicsPipeline_->RenderPass().Handle();
	renderPassInfo.framebuffer			= framebuffers_[imageIndex]->Handle();
	renderPassInfo.renderArea.offset	= { 0,0 };
	renderPassInfo.renderArea.extent	= swapChain_->Extent();
	renderPassInfo.clearValueCount		= static_cast<uint32_t>(clearColor.size());
	renderPassInfo.pClearValues			= clearColor.data();

	VkBuffer vertexBuffers[] = { scene_->VertexBuffer().Handle() };
	VkBuffer indexBuffers[] = { scene_->IndexBuffer().Handle() };
	VkDeviceSize offsets[] = { 0 };

	VkDescriptorSet descriptorSet[] = { graphicsPipeline_->DescriptorSet().Handle(imageIndex) };

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline_->Handle());
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, *indexBuffers, 0, VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline_->PipelineLayout(), 0, 1, descriptorSet, 0, nullptr);
		//vkCmdDraw(commandBuffer, 3, 1, 0, 0);
		vkCmdDrawIndexed(commandBuffer, scene_->Indices().size(), 1, 0, 0, 0);
	vkCmdEndRenderPass(commandBuffer);
}

void VulkanApplication::drawFrame()
{
	const auto  noTimeout = std::numeric_limits<uint64_t>::max();

	// Semaphores
	const auto imageAvailableSemaphore = imageAvailableSemaphore_[currentFrame_].Handle();
	const auto renderFinishedSemaphore = renderFinishedSemaphore_[currentFrame_].Handle();

	uint32_t imageIndex;
	auto result = vkAcquireNextImageKHR(device_->Handle(), swapChain_->Handle(), UINT64_MAX, imageAvailableSemaphore, nullptr, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		throw std::runtime_error("Failed to acquire next image");

	auto& inFlightFence = inFlightFences_[imageIndex];
	inFlightFence.wait(noTimeout);

	//--- COMMAND BUFFERS --- *! 
	// Since we do not need to change the command we could also record them before the draw frame and thus we wouldn't have to reset it everyframe
	// TODO: Search for optimization!! What would the best option be??
	const auto commandBuffer = commandBuffers_->Begin(imageIndex);
	render(commandBuffer, imageIndex);
	commandBuffers_->End(imageIndex);

	updateUniformBuffer(imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[]		= { imageAvailableSemaphore };
	VkSemaphore signalSemaphores[]		= { renderFinishedSemaphore };
	VkCommandBuffer commandBuffers[]	= { commandBuffer };
	VkPipelineStageFlags waitStages[]	= { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	submitInfo.waitSemaphoreCount	= 1;
	submitInfo.pWaitSemaphores		= waitSemaphores;
	submitInfo.pWaitDstStageMask	= waitStages;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores	= signalSemaphores;
	submitInfo.commandBufferCount	= 1;
	submitInfo.pCommandBuffers		= commandBuffers;

	inFlightFence.reset();

	if (vkQueueSubmit(device_->GraphicsQueue(), 1, &submitInfo, inFlightFence.Handle()) != VK_SUCCESS)
		throw std::runtime_error("Failed to submit draw command buffer!");

	VkSwapchainKHR swapChains[] = { swapChain_->Handle() };

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType				= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount	= 1;
	presentInfo.pWaitSemaphores		= signalSemaphores;
	presentInfo.swapchainCount		= 1;
	presentInfo.pSwapchains			= swapChains;
	presentInfo.pImageIndices		= &imageIndex;
	presentInfo.pResults			= nullptr;

	result = vkQueuePresentKHR(device_->PresentQueue(), &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || frambufferResized) {
		frambufferResized = false;
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS)
		throw std::runtime_error("Failed to present swap chain image!");

	currentFrame_ = (currentFrame_ + 1) % MAX_FRAMES_IN_FLIGHT;

}

void VulkanApplication::cleanup()
{
	cleanupSwapChain();

	if (enableValidationLayers)
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);

	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();
}

void VulkanApplication::getAvailableInstanceExtensions()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	extensions.resize(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
}

void VulkanApplication::createInstance()
{
	VkApplicationInfo appInfo{};
	appInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.apiVersion			= VK_API_VERSION_1_2;
	appInfo.pApplicationName	= "VkEngine";
	appInfo.applicationVersion	= VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName			= "Pinuts";
	appInfo.engineVersion		= VK_MAKE_VERSION(1, 0, 0);

	auto extensions = getRequiredExtensions();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (enableValidationLayers) 
		populateDebugMessengerCreateInfo(debugCreateInfo);

	VkInstanceCreateInfo instanceInfo{};
	instanceInfo.sType						= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo			= &appInfo;
	instanceInfo.enabledExtensionCount		= extensions.size();
	instanceInfo.ppEnabledExtensionNames	= extensions.data();
	instanceInfo.enabledLayerCount			= enableValidationLayers ? validationLayers.size() : 0;
	instanceInfo.ppEnabledLayerNames		= enableValidationLayers ? validationLayers.data() : nullptr;
	instanceInfo.pNext						= enableValidationLayers 
												? (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo : nullptr;
	
	if (vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS)
		throw std::runtime_error("Failed to create instance!");
}

bool VulkanApplication::checkValidationLayerSupport(std::vector<const char*> validationLayers)
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

std::vector<const char*> VulkanApplication::getRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

void VulkanApplication::createSurface()
{
	VulkanApplication* app = VulkanApplication::Instance();

	if (glfwCreateWindowSurface(instance, app->window, nullptr, &surface) != VK_SUCCESS)
		throw std::runtime_error("Failed to create surface!");
}

void VulkanApplication::updateUniformBuffer(const uint32_t imageIndex)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo = {};
	ubo.model = glm::mat4(1.0f);
	ubo.view = glm::lookAt(camera_->eye_, camera_->center_, camera_->up_);
	//ubo.proj[1][1] *= -1;

	//ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//glm::vec3 eye = glm::vec3(20.0f, 20.0f, 20.0f);
	//glm::vec3 center = glm::vec3(0, 0, 0);
	//glm::vec3 up = glm::vec3(0, 0, 1);
	//ubo.view = glm::lookAt(glm::vec3(20.0f, 20.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), swapChain_->Extent().width / (float)swapChain_->Extent().height, 0.1f, 1000.0f);

	//ubo.view = camera_->View();
	//ubo.proj = camera_->Projection();
	ubo.proj[1][1] *= -1;

	uniformBuffers_[imageIndex].setValue(ubo);
}

void VulkanApplication::createSwapChain()
{
	swapChain_->createSwapChain();
	depthBuffer_ = new DepthBuffer(*commandPool_, swapChain_->Extent());

	for (size_t i = 0; i != swapChain_->ImageViews().size(); i++)
	{
		imageAvailableSemaphore_.emplace_back(*device_);
		renderFinishedSemaphore_.emplace_back(*device_);
		inFlightFences_.emplace_back(*device_, true);
		uniformBuffers_.emplace_back(*device_);
	}
}

void VulkanApplication::recreateSwapChain()
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(this->window, &width, &height);
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(this->window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(device_->Handle());

	cleanupSwapChain();

	createSwapChain();
	createGraphicsPipeline();
}

void VulkanApplication::cleanupSwapChain()
{
	commandBuffers_->~CommandBuffers();
	framebuffers_.clear();
	graphicsPipeline_->~GraphicsPipeline();
	uniformBuffers_.clear();
	inFlightFences_.clear();
	renderFinishedSemaphore_.clear();
	imageAvailableSemaphore_.clear();
	depthBuffer_->~DepthBuffer();
	swapChain_->~VulkanSwapChain();
}

void VulkanApplication::createGraphicsPipeline()
{
	//--- CREATE COMMAND POOL ---
	//commandPool_ = new CommandPool(*device_, device_->Indices().graphicsFamily.value(), true);

	//--- CREATE GRAPHICS PIPELINE --
	graphicsPipeline_ = new GraphicsPipeline(*device_, *swapChain_, uniformBuffers_, *depthBuffer_, getScene(), false);

	//--- CREATE FRAMEBUFFERS ---
	for (const auto& imageView : swapChain_->ImageViews())
	{
		framebuffers_.push_back(new Framebuffer(*imageView, graphicsPipeline_->RenderPass()));
	}

	//--- CREATE COMMAND BUFFER ---
	commandBuffers_ = new CommandBuffers(*commandPool_, framebuffers_.size());
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}

void VulkanApplication::setupDebugMessenger()
{
	if (!enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug messenger!");
	}
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
	const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

const Scene& VulkanApplication::getScene()
{
	if (scene_ == nullptr)
	{
		scene_ = new Scene(*commandPool_);
	}
	return *scene_;
}

void VulkanApplication::input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float speed = 5.0f * deltaTime_;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		glm::vec3 vector = glm::vec3(0.0f, 0.0f, -1.0f) * speed;
		camera_->eye_ += vector;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		glm::vec3 vector = glm::vec3(0.0f, 0.0f, 1.0f) * speed;
		camera_->eye_ += vector;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera_->eye_ += glm::vec3(-1.0f, 0.0f, 0.0f) * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera_->eye_ += glm::vec3(1.0f, 0.0f, 0.0f) * speed;
		camera_->center_ = camera_->center_ + glm::vec3(1.0f, 0.0f, 0.0f) * speed;
	}
	//	camera_->move(glm::vec3(0.0f, 0.0f, -1.0f) * speed);
}