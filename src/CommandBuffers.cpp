#include "CommandBuffers.h"
#include "CommandPool.h"
#include "VulkanDevice.h"

CommandBuffers::CommandBuffers(CommandPool& commandPool, const uint32_t size):
	commandPool_(commandPool)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType					= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool			= commandPool.Handle();
	allocInfo.level					= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount	= size;

	commandBuffers_.resize(size);

	if (vkAllocateCommandBuffers(commandPool.Device().Handle(), &allocInfo, commandBuffers_.data()) != VK_SUCCESS)
		throw std::runtime_error("Failed to allocate frame buffers!");
}

CommandBuffers::~CommandBuffers()
{
	if (!commandBuffers_.empty())
	{
		vkFreeCommandBuffers(commandPool_.Device().Handle(), commandPool_.Handle(), (uint32_t)commandBuffers_.size(), commandBuffers_.data());
		commandBuffers_.clear();
	}
}

VkCommandBuffer CommandBuffers::Begin(const size_t i)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags				= 0; // VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	beginInfo.pInheritanceInfo	= nullptr;

	if (vkBeginCommandBuffer(commandBuffers_[i], &beginInfo) != VK_SUCCESS)
		throw std::runtime_error("Failed to begin command buffer recording!");
	return commandBuffers_[i];
}

void CommandBuffers::End(const size_t i)
{
	if (vkEndCommandBuffer(commandBuffers_[i]) != VK_SUCCESS)
		throw std::runtime_error("Failed to end record of command buffer!");
}
