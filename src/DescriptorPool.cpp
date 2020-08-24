#include "DescriptorPool.h"
#include "VulkanDevice.h"

DescriptorPool::DescriptorPool(const VulkanDevice& device, const std::vector<DescriptorBinding>& descriptorBindings, const size_t maxSets) :
	device_(device)
{
	std::vector<VkDescriptorPoolSize> poolSizes;

	for (const auto& binding : descriptorBindings)
	{
		poolSizes.push_back(VkDescriptorPoolSize{ binding.type, static_cast<uint32_t>(binding.descriptorCount) * static_cast<uint32_t>(maxSets) });
	}

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.maxSets		= static_cast<uint32_t>(maxSets);
	poolInfo.poolSizeCount	= static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes		= poolSizes.data();

	if (vkCreateDescriptorPool(device.Handle(), &poolInfo, nullptr, &descriptorPool_) != VK_SUCCESS)
		throw std::runtime_error("Failed to create descriptor pool!");
}

DescriptorPool::~DescriptorPool()
{
	if (descriptorPool_ != nullptr) {
		vkDestroyDescriptorPool(device_.Handle(), descriptorPool_, nullptr);
		descriptorPool_ = nullptr;
	}
}