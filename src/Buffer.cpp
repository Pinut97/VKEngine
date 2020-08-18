#include "Buffer.h"
#include "CommandPool.h"
#include "CommandBuffers.h"
#include "HelperFunctions.h"

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

	commandBuffers[0] =  beginSingleTimeCommands(commandPool);

	VkBufferCopy copyRegion{};
	copyRegion.dstOffset	= 0;
	copyRegion.srcOffset	= 0;
	copyRegion.size			= size;

	vkCmdCopyBuffer(commandBuffers[0], src.Handle(), Handle(), 1, &copyRegion);

	const auto graphicsQueue = commandPool.Device().GraphicsQueue();
	endSingleTimeCommand(commandBuffers[0], graphicsQueue, commandPool);
}