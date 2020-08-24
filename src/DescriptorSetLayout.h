#pragma once
#include "headers.h"
#include "DescriptorBinding.h"

class VulkanDevice;

class DescriptorSetLayout {
public:
	DescriptorSetLayout(const class VulkanDevice& device, const std::vector<DescriptorBinding>& descriptorBindings);
	~DescriptorSetLayout();

	const VkDescriptorSetLayout& Handle() const { return descriptorSetLayout_; }

private:

	const class VulkanDevice& device_;
	VkDescriptorSetLayout	descriptorSetLayout_;
	VkPipelineLayout		pipelineLayout_;
};