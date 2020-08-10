#pragma once
#include "headers.h"

class VulkanSwapChain;

class RenderPass final
{
public:
	RenderPass(const class VulkanSwapChain& swapChain);
	~RenderPass();

	const class VulkanSwapChain& SwapChain() const { return swapChain_; };
	VkRenderPass Handle() const { return renderPass_; }

private:
	
	VkRenderPass renderPass_;
	const class VulkanSwapChain& swapChain_;

};