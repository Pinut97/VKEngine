#pragma once

#include "headers.h"

struct DescriptorBinding {
	uint32_t binding;
	uint32_t descriptorCount;
	VkDescriptorType type;
	VkShaderStageFlags stage;
};