#pragma once
#include "headers.h"

class VulkanSwapChain;
class DepthBuffer;

class RenderPass final
{
public:
	RenderPass(const class VulkanSwapChain& swapChain, const DepthBuffer& depthBuffer);
	~RenderPass();

	const class VulkanSwapChain& SwapChain() const { return swapChain_; };
	const class DepthBuffer& DepthBuffer() const { return depthBuffer_; }
	VkRenderPass Handle() const { return renderPass_; }

private:
	
	VkRenderPass renderPass_;
	const class VulkanSwapChain&	swapChain_;
	const class DepthBuffer&		depthBuffer_;

};