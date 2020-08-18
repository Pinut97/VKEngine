#pragma once

#include "headers.h"

class CommandPool;

VkCommandBuffer beginSingleTimeCommands(CommandPool& commandPool);

void endSingleTimeCommand(VkCommandBuffer& commandBuffer, const VkQueue& queue, CommandPool& commandPool);
