#pragma once

#include "headers.h"

class VulkanDevice;

struct SamplerConfig final
{
	VkFilter magFilter = VK_FILTER_LINEAR;
	VkFilter minFilter = VK_FILTER_LINEAR;
	VkSamplerAddressMode addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	VkSamplerAddressMode addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	VkSamplerAddressMode addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	VkBool32 anisotropyEnable = VK_TRUE;
	float maxAnisotropy = 16.0f;
	VkBorderColor borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	VkBool32 unnormalizedCoordinates = VK_FALSE;
	VkBool32 compareEnable = VK_FALSE;
	VkCompareOp compareOp = VK_COMPARE_OP_ALWAYS;
	VkSamplerMipmapMode mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	float mipLodBias = 0.0f;
	float minLod = 0.0f;
	float maxLod = 0.0;
};

class Sampler final
{
public:

	Sampler(const VulkanDevice& device, const SamplerConfig& config);
	~Sampler();

	const VkSampler Handle() const { return sampler_; }
	const VulkanDevice& Device() const { return device_; }

private:

	VkSampler			sampler_;
	const VulkanDevice& device_;
};