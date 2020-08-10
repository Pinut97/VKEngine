#include "VulkanSwapChain.h"
#include "VulkanApplication.h"
#include "VulkanDevice.h"

VulkanSwapChain::VulkanSwapChain()
{
}

VulkanSwapChain::~VulkanSwapChain()
{
	VkDevice device = device_->Handle();
	for (auto& imgView : scImageViews)
		vkDestroyImageView(device, imgView->Handle(), nullptr);
	vkDestroySwapchainKHR(device, swapChain, nullptr);
}

// Function that queries the capabilities, format and present modes of the swap chain and stores in the
// struct support info for future uses
SwapChainSupportDetails VulkanSwapChain::querySwapChainSupport(VkPhysicalDevice device)
{
	VulkanApplication* app = VulkanApplication::Instance();

	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, app->Surface(), &details.capabilites);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, app->Surface(), &formatCount, nullptr);
	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, app->Surface(), &formatCount, details.formats.data());
	}

	uint32_t presentCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, app->Surface(), &presentCount, nullptr);
	if(presentCount){
		details.presentModes.resize(presentCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, app->Surface(), &presentCount, details.presentModes.data());
	}

	return details;
}

VkSurfaceFormatKHR VulkanSwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return availableFormat;
	}
	return availableFormats[0];
}

VkPresentModeKHR VulkanSwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			return availablePresentMode;
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilites)
{
	if (capabilites.currentExtent.width != UINT32_MAX)
		return capabilites.currentExtent;
	else {
		VkExtent2D actualExtent = { WIDTH, HEIGHT };

		actualExtent.width	= std::max(capabilites.currentExtent.width, std::min(capabilites.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilites.currentExtent.height, std::min(capabilites.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

void VulkanSwapChain::createSwapChain()
{
	VulkanApplication* app = VulkanApplication::Instance();

	device_ = (VulkanDevice*)app->DeviceObj();

	scPrivateVariables.swapChainSupportedDetails = querySwapChainSupport(app->DeviceObj()->GPU());

	scPrivateVariables.extent			= chooseSwapExtent(scPrivateVariables.swapChainSupportedDetails.capabilites);
	scPrivateVariables.presentMode		= chooseSwapPresentMode(scPrivateVariables.swapChainSupportedDetails.presentModes);
	scPrivateVariables.surfaceFormat	= chooseSwapSurfaceFormat(scPrivateVariables.swapChainSupportedDetails.formats);
	scPublicVariables.format = scPrivateVariables.surfaceFormat.format;

	scPrivateVariables.scImageCount = scPrivateVariables.swapChainSupportedDetails.capabilites.minImageCount + 1;

	if (scPrivateVariables.swapChainSupportedDetails.capabilites.maxImageCount > 0 && 
		scPrivateVariables.scImageCount > scPrivateVariables.swapChainSupportedDetails.capabilites.maxImageCount)
		scPrivateVariables.scImageCount = scPrivateVariables.swapChainSupportedDetails.capabilites.maxImageCount;

	scPublicVariables.surface		= app->Surface();
	QueueFamilyIndices indices		= app->DeviceObj()->Indices();
	uint32_t queueFamilyIndices[]	= { indices.graphicsFamily.value(), indices.presentFamily.value() };

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType				= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface				= scPublicVariables.surface;
	createInfo.minImageCount		= scPrivateVariables.scImageCount;
	createInfo.imageFormat			= scPrivateVariables.surfaceFormat.format;
	createInfo.imageColorSpace		= scPrivateVariables.surfaceFormat.colorSpace;
	createInfo.imageExtent			= scPrivateVariables.extent;
	createInfo.presentMode			= scPrivateVariables.presentMode;
	createInfo.imageArrayLayers		= 1;
	createInfo.imageUsage			= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}
	createInfo.preTransform			= scPrivateVariables.swapChainSupportedDetails.capabilites.currentTransform;
	createInfo.compositeAlpha		= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.clipped				= VK_TRUE;
	createInfo.oldSwapchain			= VK_NULL_HANDLE;

	VkDevice device = app->DeviceObj()->Handle();

	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
		throw std::runtime_error("Failed to create swap chain!");

	if (vkGetSwapchainImagesKHR(device, swapChain, &scPrivateVariables.scImageCount, nullptr) != VK_SUCCESS)
		throw std::runtime_error("Failed to get swap chain image count!");
	scPrivateVariables.scImages.resize(scPrivateVariables.scImageCount);
	if (vkGetSwapchainImagesKHR(device, swapChain, &scPrivateVariables.scImageCount, scPrivateVariables.scImages.data()) != VK_SUCCESS)
		throw std::runtime_error("Failed to get swap chain images!");

	for (const auto image : scPrivateVariables.scImages)
		scImageViews.push_back(std::make_unique<ImageView>(*device_, image, scPublicVariables.format, VK_IMAGE_ASPECT_COLOR_BIT));

}