#pragma once
#include "headers.h"

class VulkanSwapChain;
class VulkanDevice;
class RenderPass;
class DescriptorPool;
class DescriptorSet;
class DescriptorSetLayout;
class UniformBuffer;
class Scene;
class DepthBuffer;

class GraphicsPipeline {
public:
	GraphicsPipeline(
		const VulkanDevice& device,
		const VulkanSwapChain& swapChain,
		const std::vector<UniformBuffer>& uniformBuffer,
		const DepthBuffer& depthBuffer,
		const Scene& scene,
		bool isWireFrame
	);
	~GraphicsPipeline();

	const class RenderPass& RenderPass() const { return *renderPass_; }
	const VkPipeline& Handle() const { return graphicsPipeline_; }
	const VkPipelineLayout& PipelineLayout() const { return pipelineLayout_; }
	const DescriptorSet& DescriptorSet() const { return *descriptorSet_; }

private:
	const VulkanDevice&			device_;
	const VulkanSwapChain&		swapChain_;
	const bool					isWireFrame_;

	//temp
	const class DescriptorPool*			descriptorPool_;
	const class DescriptorSet*			descriptorSet_;
	const class DescriptorSetLayout*	descriptorSetLayout_;

	VkPipeline							graphicsPipeline_;
	VkPipelineLayout					pipelineLayout_;
	class RenderPass*					renderPass_;
	

};