#pragma once

#include "headers.h"
#include <fstream>

class VulkanDevice;

class ShaderModule final
{
public:
	ShaderModule(const VulkanDevice& device, const std::string& filename);
	~ShaderModule();

	const class VulkanDevice& Device() const { return device_; }
	VkPipelineShaderStageCreateInfo createShaderStage(VkShaderStageFlagBits stage)const;

private:

	static std::vector<char> ReadFile(const std::string& filename);

	const class VulkanDevice& device_;

	VkShaderModule shaderModule_;
};