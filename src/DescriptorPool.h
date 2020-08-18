#pragma once
#include "headers.h"

class VulkanDevice;

struct DescriptorBinding {
	uint32_t binding;
	uint32_t descriptorCount;
	VkDescriptorType type;
	VkShaderStageFlags stage;
};

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