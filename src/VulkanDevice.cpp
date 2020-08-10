#include "VulkanDevice.h"
#include "VulkanApplication.h"
#include <set>

VulkanDevice::VulkanDevice(VkInstance* instance)
{
	deviceExtensions = std::vector<const char*>{ "VK_KHR_swapchain" };

	instance_	= instance;
	device_		= nullptr;
	gpu_		= nullptr;

	pickPhysicalDevice();
	createLogicalDevice();
}

VulkanDevice::~VulkanDevice() {
	instance_ = nullptr;
	vkDestroyDevice(device_, nullptr);
}

void VulkanDevice::createLogicalDevice()
{
	indices_ = findQueueFamilies(gpu_);

	std::vector<VkDeviceQueueCreateInfo> queueInfos;
	std::set<uint32_t>uniqueQueueFamilies = { indices_.graphicsFamily.value(), indices_.presentFamily.value() };

	float queuePriority = 1.0f;
	for (const auto& family : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueInfo{};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueCount = 1;
		queueInfo.queueFamilyIndex = family;
		queueInfo.pQueuePriorities = &queuePriority;

		queueInfos.push_back(queueInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo deviceInfo{};
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.queueCreateInfoCount = queueInfos.size();
	deviceInfo.pQueueCreateInfos = queueInfos.data();
	deviceInfo.pEnabledFeatures = &deviceFeatures;
	deviceInfo.enabledExtensionCount = deviceExtensions.size();
	deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
	deviceInfo.enabledLayerCount = enableValidationLayers ?
		VulkanApplication::Instance()->ValidationLayers().size() : 0;
	deviceInfo.ppEnabledLayerNames = enableValidationLayers ?
		VulkanApplication::Instance()->ValidationLayers().data() : NULL;

	if (vkCreateDevice(gpu_, &deviceInfo, nullptr, &device_) != VK_SUCCESS)
		throw std::runtime_error("Failed to create logical device!");

	vkGetDeviceQueue(device_, indices_.graphicsFamily.value(), 0, &graphicsQueue_);
	vkGetDeviceQueue(device_, indices_.presentFamily.value(), 0, &presentQueue_);
}

void VulkanDevice::pickPhysicalDevice()
{
	uint32_t deviceCount = 0;

	vkEnumeratePhysicalDevices(*instance_, &deviceCount, nullptr);
	if (deviceCount == 0)
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(*instance_, &deviceCount, devices.data());

	for (const auto& device : devices)
	{
		if (isDeviceSuitable(device)) {
			gpu_ = device;
			break;
		}
	}
	if (gpu_ == VK_NULL_HANDLE)
		throw std::runtime_error("Failed to find a suitable GPU!");
}

bool VulkanDevice::isDeviceSuitable(VkPhysicalDevice device)
{
	VulkanApplication* app = VulkanApplication::Instance();

	QueueFamilyIndices indices = findQueueFamilies(device);

	bool extensionSupported = checkDeviceExtensionSupport(device);
	bool swapChainAdequate = false;

	if (extensionSupported) {
		SwapChainSupportDetails swapChainDetails = app->SwapChain()->querySwapChainSupport(device);
		swapChainAdequate = !swapChainDetails.formats.empty() && !swapChainDetails.presentModes.empty();
	}

	return indices.is_complete() && extensionSupported && swapChainAdequate;
}

bool VulkanDevice::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties>availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions = { deviceExtensions.begin(), deviceExtensions.end() };

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();

}

// TODO: use such function to pick the highest performance gpu available in the system
int VulkanDevice::rateDeviceSuitability(VkPhysicalDevice device)
{
	// To evaluate the suitability of the device we can start by querying for some details.
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	int score = 0;

	// Discrete gpus have a significant performance advantage
	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		score += 1000;

	// Maximum possible size of textures affects graphics quality
	score += deviceProperties.limits.maxImageDimension2D;

	if (!deviceFeatures.geometryShader)
		return 0;

	return score;
}

QueueFamilyIndices VulkanDevice::findQueueFamilies(VkPhysicalDevice device)
{
	VulkanApplication* app = VulkanApplication::Instance();
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices.graphicsFamily = i;

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, app->Surface(), &presentSupport);
		if (presentSupport)
			indices.presentFamily = i;

		if (indices.is_complete())
			break;
		i++;
	}

	return indices;
}


