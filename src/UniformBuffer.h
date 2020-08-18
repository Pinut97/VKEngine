#pragma once
#include "headers.h"

class Buffer;
class DeviceMemory;
class VulkanDevice;

class UniformBufferObject {
public:
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

class UniformBuffer {
public:

	UniformBuffer(const UniformBuffer&) = delete;

	explicit UniformBuffer(const VulkanDevice& device);
	UniformBuffer(UniformBuffer&& other) noexcept;
	~UniformBuffer();

	const Buffer& Buffer() const { return *buffer_; }

	void setValue(const UniformBufferObject& ubo);

private:
	std::unique_ptr<class Buffer> buffer_;
	std::unique_ptr<DeviceMemory> memory_;
};