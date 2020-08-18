#pragma once

#include "headers.h"
#include "ImageView.h"

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR		capabilites;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR>	presentModes;

};

struct SwapChainPrivateVariables {

	SwapChainSupportDetails swapChainSupportedDetails;
	VkExtent2D				extent;
	VkSurfaceFormatKHR		surfaceFormat;
	VkPresentModeKHR		presentMode;
	uint32_t				scImageCount;
	std::vector<VkImage>	scImages;
};

struct SwapChainPublicVariables {
	VkSurfaceKHR	surface;
	VkFormat		format;
};

class VulkanDevice;

class VulkanSwapChain {
public:

	VulkanSwapChain();
	~VulkanSwapChain();

	SwapChainPublicVariables scPublicVariables;

	void createSwapChain();
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	const VkExtent2D Extent() const { return scPrivateVariables.extent; }
	const class VulkanDevice* Device() const { return device_; }
	const std::vector<VkImage> Images() const { return scPrivateVariables.scImages; }
	const std::vector<std::unique_ptr<ImageView>>& ImageViews() const { return scImageViews; }
	const VkSwapchainKHR& Handle() const { return swapChain_; }

private:

	const VulkanDevice*			device_;
	VkSwapchainKHR				swapChain_ = nullptr;
	SwapChainPrivateVariables	scPrivateVariables;
	std::vector<std::unique_ptr<ImageView>>		scImageViews;

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availableModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilites);

};