#pragma once

#include "headers.h"

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool is_complete(){
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

class VulkanDevice {
public:
	VulkanDevice(VkInstance* instance);
	~VulkanDevice();

	void createLogicalDevice();

	VkPhysicalDevice GPU() const { return gpu_; }
	VkDevice Handle() const { return device_; }
	QueueFamilyIndices Indices() const { return indices_; }
	VkQueue GraphicsQueue() const { return graphicsQueue_; }
	VkQueue PresentQueue() const { return presentQueue_; }

private:

	VkPhysicalDevice	gpu_;
	VkDevice			device_;
	VkInstance*			instance_;

	QueueFamilyIndices	indices_;

	VkQueue				graphicsQueue_;
	VkQueue				presentQueue_;
	VkQueue				transferQueue_;

	std::vector<const char*>	validationLayers;
	std::vector<const char*>	deviceExtensions;

	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	int rateDeviceSuitability(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	
};