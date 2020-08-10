#include "ShaderModule.h"
#include "VulkanDevice.h"

ShaderModule::ShaderModule(const class VulkanDevice& device, const std::string& filename) :
	device_(device)
{

	std::vector<char> code = ReadFile(filename);

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType		= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize		= code.size();
	createInfo.pCode		= (const uint32_t*)code.data();

	if (vkCreateShaderModule(device.Handle(), &createInfo, nullptr, &shaderModule_) != VK_SUCCESS)
		throw std::runtime_error("Failed to create shader module!");

}

ShaderModule::~ShaderModule()
{
	if (shaderModule_ != nullptr) {
		vkDestroyShaderModule(device_.Handle(), shaderModule_, nullptr);
		shaderModule_ = nullptr;
	}
}

VkPipelineShaderStageCreateInfo ShaderModule::createShaderStage(VkShaderStageFlagBits stage) const
{
	VkPipelineShaderStageCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	createInfo.stage = stage;
	createInfo.module = shaderModule_;
	createInfo.pName = "main";

	return createInfo;
}

std::vector<char> ShaderModule::ReadFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
		throw std::runtime_error("Failed to open file '" + filename + "'");

	const auto fileSize = static_cast<size_t>(file.tellg());
	std::vector<char>buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}