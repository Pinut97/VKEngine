#include "Semaphore.h"
#include "VulkanDevice.h"

Semaphore::Semaphore(const class VulkanDevice& device) : 
	device_(device)
{

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	if (vkCreateSemaphore(device.Handle(), &semaphoreInfo, nullptr, &semaphore_) != VK_SUCCESS)
		throw std::runtime_error("Failed to create Semaphore!");
}

Semaphore::~Semaphore()
{
	if (semaphore_ != nullptr)
	{
		//vkDestroySemaphore(device_.Handle(), semaphore_, nullptr);
		//semaphore_ = nullptr;
	}
}