#pragma once
#include "headers.h"

class VulkanDevice;

class Semaphore 
{
public:
	Semaphore(const VulkanDevice& device);
	~Semaphore();

	const VkSemaphore& Handle() const { return semaphore_; }

private:

	const class VulkanDevice&	device_;
	VkSemaphore					semaphore_;
};