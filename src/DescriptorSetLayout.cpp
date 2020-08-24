#include "DescriptorSetLayout.h"
#include "VulkanDevice.h"
//#include "DescriptorPool.h"

DescriptorSetLayout::DescriptorSetLayout(const class VulkanDevice& device, const std::vector<DescriptorBinding>& bindings) :
	device_(device)
{

	std::vector<VkDescriptorSetLayoutBinding> layoutBindings;

	for (auto& binding : bindings)
	{
		VkDescriptorSetLayoutBinding b{};
		b.binding = binding.binding;
		b.descriptorCount = binding.descriptorCount;
		b.descriptorType = binding.type;
		b.stageFlags = binding.stage;
		b.pImmutableSamplers = nullptr;

		layoutBindings.push_back(b);
	}
	/*
	VkDescriptorSetLayoutBinding layoutBinding{};
	layoutBinding.binding				= 0;
	layoutBinding.descriptorCount		= 1;
	layoutBinding.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	layoutBinding.stageFlags			= VK_SHADER_STAGE_VERTEX_BIT;
	layoutBinding.pImmutableSamplers	= nullptr;

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding		= 1;
	layoutBinding.descriptorCount		= 1;
	layoutBinding.descriptorType		= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	layoutBinding.stageFlags			= VK_SHADER_STAGE_FRAGMENT_BIT;
	layoutBinding.pImmutableSamplers	= nullptr;

	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { layoutBinding, samplerLayoutBinding };
	*/

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType		= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
	layoutInfo.pBindings	= layoutBindings.data();

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