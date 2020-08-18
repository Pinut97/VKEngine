#include "DescriptorSetLayout.h"
#include "VulkanDevice.h"

DescriptorSetLayout::DescriptorSetLayout(const class VulkanDevice& device) :
	device_(device)
{
	VkDescriptorSetLayoutBinding layoutBinding{};
	layoutBinding.binding				= 0;
	layoutBinding.descriptorCount		= 1;
	layoutBinding.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	layoutBinding.stageFlags			= VK_SHADER_STAGE_VERTEX_BIT;
	layoutBinding.pImmutableSamplers	= nullptr;

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType		= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings	= &layoutBinding;

	if (vkCreateDescriptorSetLayout(device.Handle(), &layoutInfo, nullptr, &descriptorSetLayout_) != VK_SUCCESS)
		throw std::runtime_error("Failed to create descriptor set layout!");
}

DescriptorSetLayout::~DescriptorSetLayout()
{
	if (descriptorSetLayout_ != nullptr)
	{
		vkDestroyDescriptorSetLayout(device_.Handle(), descriptorSetLayout_, nullptr);
		descriptorSetLayout_ = nullptr;
	}
}