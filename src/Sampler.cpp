#include "Sampler.h"
#include "VulkanDevice.h"

Sampler::Sampler(const VulkanDevice& device, const SamplerConfig& config) :
	device_(device)
{

	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = config.magFilter;
	samplerInfo.minFilter = config.minFilter;
	samplerInfo.addressModeU = config.addressModeU;
	samplerInfo.addressModeV = config.addressModeV;
	samplerInfo.addressModeW = config.addressModeW;
	samplerInfo.anisotropyEnable = config.anisotropyEnable;
	samplerInfo.borderColor = config.borderColor;
	samplerInfo.compareEnable = config.compareEnable;
	samplerInfo.compareOp = config.compareOp;
	samplerInfo.maxAnisotropy = config.maxAnisotropy;
	samplerInfo.mipLodBias = config.mipLodBias;
	samplerInfo.maxLod = config.maxLod;
	samplerInfo.minLod = config.minLod;
	samplerInfo.mipmapMode = config.mipmapMode;
	samplerInfo.unnormalizedCoordinates = config.unnormalizedCoordinates;

	if (vkCreateSampler(device.Handle(), &samplerInfo, nullptr, &sampler_) != VK_SUCCESS)
		throw std::runtime_error("Failed to create sampler!");
}

Sampler::~Sampler()
{
	if (sampler_ != nullptr) {
		vkDestroySampler(device_.Handle(), sampler_, nullptr);
		sampler_ = nullptr;
	}
}