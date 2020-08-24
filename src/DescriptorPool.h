#pragma once
#include "headers.h"
#include "DescriptorBinding.h"

class VulkanDevice;

class DescriptorPool {
public:
	DescriptorPool(const VulkanDevice& device, const std::vector<DescriptorBinding>& descriptorBindings, size_t maxSets);
	~DescriptorPool();

	const VkDescriptorPool& Handle() const { return descriptorPool_; }
	const VulkanDevice& Device() const { return device_; }

private:

	const VulkanDevice& device_;
	VkDescriptorPool descriptorPool_;

};