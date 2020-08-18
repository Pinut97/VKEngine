#include "UniformBuffer.h"
#include "Buffer.h"

UniformBuffer::UniformBuffer(const VulkanDevice& device)
{
	const auto bufferSize = sizeof(UniformBufferObject);

	buffer_ = std::unique_ptr<class Buffer>(new class Buffer(device, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT));
	memory_ = std::unique_ptr<DeviceMemory>(new DeviceMemory(buffer_->allocateMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)));
}

UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept :
	buffer_(other.buffer_.release()),
	memory_(other.memory_.release())
{
}

UniformBuffer::~UniformBuffer()
{
	buffer_.reset();
	memory_.reset();
}

void UniformBuffer::setValue(const UniformBufferObject& ubo)
{
	const auto data = memory_->map(0, sizeof(UniformBufferObject));
	std::memcpy(data, &ubo, sizeof(UniformBufferObject));
	memory_->unmap();
}