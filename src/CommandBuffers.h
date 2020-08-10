#pragma once

#include "headers.h"

class CommandPool;

class CommandBuffers
{
public:
	CommandBuffers(CommandPool& commandPool, uint32_t size);
	~CommandBuffers();

	uint32_t Size() const { return static_cast<uint32_t>(commandBuffers_.size()); }
	VkCommandBuffer& operator[] (const size_t i){ return commandBuffers_[i]; }

	VkCommandBuffer Begin(size_t i);
	void End(size_t);

private:

	const CommandPool& commandPool_;
	std::vector<VkCommandBuffer> commandBuffers_;
};