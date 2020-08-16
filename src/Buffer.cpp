#include "Buffer.h"
#include "CommandPool.h"
#include "CommandBuffers.h"

Buffer::Buffer(const class VulkanDevice& device, const size_t size, const VkBufferUsageFlags usage) :
	device_(device),
	size_(size)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType		= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size			= size;
	bufferInfo.usage		= usage;
	bufferInfo.sharingMode	= VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device.Handle(), &bufferInfo, nullptr, &buffer_) != VK_SUCCESS)
		throw std::runtime_error("Failed to create buffer!");
}

Buffer::~Buffer()
{
	if (buffer_ != nullptr)
	{
		//vkDestroyBuffer(device_.Handle(), buffer_, nullptr);
		buffer_ = nullptr;
	}
}

VkMemoryRequirements Buffer::getMemoryRequirements() const
{
	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(device_.Handle(), buffer_, &requirements);
	return requirements;
}

DeviceMemory Buffer::allocateMemory(const VkMemoryPropertyFlags properties)
{
	const auto requirements = getMemoryRequirements();
	DeviceMemory memory(device_, requirements.size, requirements.memoryTypeBits, properties);

	if (vkBindBufferMemory(device_.Handle(), buffer_, memory.Handle(), 0) != VK_SUCCESS)
		throw std::runtime_error("Failed to bind buffer memory!");
	return memory;
}

void Buffer::copyFrom(CommandPool& commandPool, const Buffer src, VkDeviceSize size)
{
	// Single Time command

	CommandBuffers commandBuffers(commandPool, 1);

	// Createa a unique begin info for copy purposes
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;	// Telling the driver we're using it just once

	vkBeginCommandBuffer(commandBuffers[0], &beginInfo);

	VkBufferCopy copyRegion{};
	copyRegion.dstOffset	= 0;
	copyRegion.srcOffset	= 0;
	copyRegion.size			= size;

	vkCmdCopyBuffer(commandBuffers[0], src.Handle(), Handle(), 1, &copyRegion);

	vkEndCommandBuffer(commandBuffers[0]);

	// Once the command has been stored, submit to the graphics queue
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[0];

	const auto graphicsQueue = commandPool.Device().GraphicsQueue();
	vkQueueSubmit(graphicsQueue, 1, &submitInfo, nullptr);
	vkQueueWaitIdle(graphicsQueue);
}