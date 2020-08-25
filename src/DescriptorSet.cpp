#include "DescriptorSet.h"
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"
#include "VulkanDevice.h"

DescriptorSet::DescriptorSet(
	const DescriptorPool& descriptorPool,
	const DescriptorSetLayout& layout,
	std::map<uint32_t, VkDescriptorType> bindingTypes,
	const size_t size) :
	descriptorPool_(descriptorPool),
	layout_(layout)
{
	std::vector<VkDescriptorSetLayout> layouts(size, layout.Handle());

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType					= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool		= descriptorPool.Handle();
	allocInfo.descriptorSetCount	= size;
	allocInfo.pSetLayouts			= layouts.data();

	descriptorSet_.resize(size);

	if (vkAllocateDescriptorSets(descriptorPool.Device().Handle(), &allocInfo, descriptorSet_.data()) != VK_SUCCESS)
		throw std::runtime_error("Failed to allocate descriptor set!");
}

VkWriteDescriptorSet DescriptorSet::bind(const uint32_t index, const uint32_t binding, const VkDescriptorBufferInfo& bufferInfo, const uint32_t count) const
{
	VkWriteDescriptorSet descriptorWrite{};
	descriptorWrite.sType				= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet				= descriptorSet_[index];
	descriptorWrite.dstBinding			= binding;
	descriptorWrite.dstArrayElement		= 0;
	descriptorWrite.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrite.descriptorCount		= 1;
	descriptorWrite.pBufferInfo			= &bufferInfo;
	descriptorWrite.pImageInfo			= nullptr;
	descriptorWrite.pTexelBufferView	= nullptr;

	return descriptorWrite;
}