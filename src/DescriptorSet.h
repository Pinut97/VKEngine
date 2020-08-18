#pragma once
#include "headers.h"

class DescriptorPool;
class DescriptorSetLayout;


class DescriptorSet {
public:
	DescriptorSet(const DescriptorPool& descriptorPool,
		const DescriptorSetLayout& layout,
		std::map<uint32_t, VkDescriptorType> bindingTypes,
		const size_t size);
	~DescriptorSet();

	VkWriteDescriptorSet bind(const uint32_t index, const uint32_t binding, const VkDescriptorBufferInfo& bufferInfo, const uint32_t count) const;

	VkDescriptorSet Handle(uint32_t index) const { return descriptorSet_[index]; }
	const std::vector<VkDescriptorSet> DescriptorSets() const { return descriptorSet_; }

private:

	std::vector<VkDescriptorSet>	descriptorSet_;
	const DescriptorPool&			descriptorPool_;
	const DescriptorSetLayout&		layout_;
};