#include "GraphicsPipeline.h"
#include "VulkanDevice.h"
#include "ShaderModule.h"
#include "VulkanSwapChain.h"
#include "RenderPass.h"
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"
#include "DescriptorSet.h"
#include "UniformBuffer.h"
#include "Buffer.h"

#include "VulkanApplication.h"

GraphicsPipeline::GraphicsPipeline(
	const VulkanDevice& device,
	const VulkanSwapChain& swapChain,
	const std::vector<UniformBuffer>& uniformBuffers,
	const bool isWireFrame
) : device_(device), swapChain_(swapChain), isWireFrame_(isWireFrame)
{

	//--- SHADER STAGES ---
	const ShaderModule vertexShader(device, "../../data/shaders/vert.spv");
	const ShaderModule fragmentShader(device, "../../data/shaders/frag.spv");

	VkPipelineShaderStageCreateInfo shaderStages[] = {
		vertexShader.createShaderStage(VK_SHADER_STAGE_VERTEX_BIT),
		fragmentShader.createShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT)
	};

	//--- FIXED-FUNCTION STATE ---
	// Input
	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescription = Vertex::getAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.vertexAttributeDescriptionCount = 2;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescription.data();

	// Assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	// Viewport
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0;
	viewport.width = static_cast<float>(swapChain.Extent().width);
	viewport.height = static_cast<float>(swapChain.Extent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissors{};
	scissors.offset = { 0, 0 };
	scissors.extent = swapChain.Extent();

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissors;

	// Rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	// Color blending
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	
	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;

	// TODO: create descriptor manager!

	std::vector<DescriptorBinding> descriptorBinding = {
		{0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT}
	};

	std::map<uint32_t, VkDescriptorType> bindingTypes;

	for (const auto& binding : descriptorBinding) {
		if (!bindingTypes.insert(std::make_pair(binding.binding, binding.type)).second)
			throw std::invalid_argument("binding collision");
	}

	descriptorPool_			= new DescriptorPool(device_, descriptorBinding, swapChain_.ImageViews().size());
	descriptorSetLayout_	= new DescriptorSetLayout(device_);
	descriptorSet_			= new class DescriptorSet(*descriptorPool_, *descriptorSetLayout_, bindingTypes, uniformBuffers.size());

	auto& descriptorSets = descriptorSet_->DescriptorSets();

	for (uint32_t i = 0; i != swapChain_.Images().size(); i++)
	{
		// Uniform buffer
		VkDescriptorBufferInfo uniformBufferInfo{};
		uniformBufferInfo.buffer	= uniformBuffers[i].Buffer().Handle();
		uniformBufferInfo.offset	= 0;
		uniformBufferInfo.range		= VK_WHOLE_SIZE;

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType				= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount		= 1;
		descriptorWrite.dstBinding			= 0;
		descriptorWrite.dstArrayElement		= 0;
		descriptorWrite.dstSet				= descriptorSets[i];
		descriptorWrite.pBufferInfo			= &uniformBufferInfo;
		descriptorWrite.pImageInfo			= nullptr;
		descriptorWrite.pTexelBufferView	= nullptr;

		vkUpdateDescriptorSets(device_.Handle(), 1, &descriptorWrite, 0, nullptr);

	}
	
	//--- PIPELINE LAYOUT ---
	VkDynamicState dynamicStates[] =
	{
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = 2;
	dynamicState.pDynamicStates = dynamicStates;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType			= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount	= 1;
	pipelineLayoutInfo.pSetLayouts		= &descriptorSetLayout_->Handle();

	if (vkCreatePipelineLayout(device.Handle(), &pipelineLayoutInfo, nullptr, &pipelineLayout_) != VK_SUCCESS)
		throw std::runtime_error("Failed to create pipeline layout!");

	//--- RENDER PASS ---
	renderPass_ = new class RenderPass(swapChain);

	//--- PIPELINE CREATION ---
	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType					= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount				= 2;
	pipelineInfo.pStages				= shaderStages;
	pipelineInfo.pVertexInputState		= &vertexInputInfo;
	pipelineInfo.pInputAssemblyState	= &inputAssemblyInfo;
	pipelineInfo.pViewportState			= &viewportState;
	pipelineInfo.pRasterizationState	= &rasterizer;
	pipelineInfo.pMultisampleState		= &multisampling;
	pipelineInfo.pDepthStencilState		= nullptr;
	pipelineInfo.pColorBlendState		= &colorBlending;
	pipelineInfo.pDynamicState			= nullptr;
	pipelineInfo.layout					= pipelineLayout_;
	pipelineInfo.renderPass				= renderPass_->Handle();
	pipelineInfo.subpass				= 0;
	pipelineInfo.basePipelineHandle		= VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex		= -1;

	if (vkCreateGraphicsPipelines(device.Handle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline_) != VK_SUCCESS)
		throw std::runtime_error("Failed to create graphics pipeline!");

}

GraphicsPipeline::~GraphicsPipeline()
{
	vkDestroyPipeline(device_.Handle(), graphicsPipeline_, nullptr);
	vkDestroyPipelineLayout(device_.Handle(), pipelineLayout_, nullptr);
	vkDestroyRenderPass(device_.Handle(), renderPass_->Handle(), nullptr);
}