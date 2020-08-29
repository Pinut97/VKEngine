#pragma once

#include "headers.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "Vertex.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class GraphicsPipeline;
class Framebuffer;
class CommandPool;
class CommandBuffers;
class Semaphore;
class Fence;
class Scene;
class UniformBuffer;
class DepthBuffer;
class Camera;

static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

class VulkanApplication final{
public:

	VulkanApplication();
	~VulkanApplication();

	static VulkanApplication* Instance()
	{
		if (!appInstance)
			appInstance = new VulkanApplication();
		return appInstance;
	}

	GLFWwindow* window;
	bool frambufferResized = false;

	Camera* camera_;
	float deltaTime_;
	float lastTime_ = 0.0f;

	void run();
	void onKey();
	void input(GLFWwindow* window);
	void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);

	const Scene& getScene();

	VkSurfaceKHR Surface() const { return surface; }
	VulkanDevice* Device() const { return device_; }
	VulkanSwapChain* SwapChain() const { return swapChain_; }

	const std::vector<VkExtensionProperties>& Extensions() const	{	return extensions;	}
	const std::vector<VkPhysicalDevice>& PhysicalDevices() const	{	return physicalDevices;	}
	const std::vector<const char*>&	ValidationLayers() const	{	return validationLayers; }

private:

	static VulkanApplication* appInstance;

	void initWindow();
	void initVulkan();
	void initApp();
	void mainLoop();
	void render(VkCommandBuffer commandBuffer, const uint32_t inmageIndex);
	void drawFrame();
	void cleanup();

	void getAvailableInstanceExtensions();
	void createInstance();
	void setupDebugMessenger();
	void createSurface();
	void createSwapChain();
	void createGraphicsPipeline();
	void updateUniformBuffer(const uint32_t imageIndex);

	void recreateSwapChain();
	void cleanupSwapChain();

	std::vector<VkExtensionProperties>	extensions;
	std::vector<VkPhysicalDevice>		physicalDevices;
	std::vector<const char*>			validationLayers;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	VkInstance					instance;
	VkDebugUtilsMessengerEXT	debugMessenger;
	VkSurfaceKHR				surface;
	VulkanDevice*				device_;
	VulkanSwapChain*			swapChain_ = nullptr;
	GraphicsPipeline*			graphicsPipeline_;
	CommandPool*				commandPool_;
	CommandBuffers*				commandBuffers_;
	std::vector<Framebuffer*>	framebuffers_;
	std::vector<Semaphore>		imageAvailableSemaphore_;
	std::vector<Semaphore>		renderFinishedSemaphore_;
	std::vector<Fence>			inFlightFences_;
	std::vector<UniformBuffer>	uniformBuffers_;
	DepthBuffer*				depthBuffer_;
	Scene*						scene_;

	size_t currentFrame_	= 0;

	uint32_t findMemoryTypes(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	bool checkValidationLayerSupport(std::vector<const char*> validationLayers);
	std::vector<const char*> getRequiredExtensions();
};