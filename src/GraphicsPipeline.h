#pragma once
#include "headers.h"

class VulkanSwapChain;
class VulkanDevice;
class RenderPass;

class GraphicsPipeline {
public:
	GraphicsPipeline(
		const VulkanDevice& device,
		const VulkanSwapChain& swapChain,
		bool isWireFrame
	);
	~GraphicsPipeline();

	const class RenderPass& RenderPass() const { return *renderPass_; }
	const VkPipeline& Handle() const { return graphicsPipeline_; }

private:
	const class VulkanDevice&	device_;
	const VulkanSwapChain&		swapChain_;
	const bool					isWireFrame_;

	VkPipeline							graphicsPipeline_;
	VkPipelineLayout					pipelineLayout_;
	class RenderPass*					renderPass_;
	//std::unique_ptr<class RenderPass>	renderPass_;

};