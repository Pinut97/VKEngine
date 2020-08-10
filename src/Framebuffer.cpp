#include "Framebuffer.h"
#include "ImageView.h"
#include "RenderPass.h"
#include "VulkanSwapChain.h"
#include "VulkanDevice.h"

Framebuffer::Framebuffer(const class ImageView& imageView, const class RenderPass& renderPass) :
	imageView_(imageView), renderPass_(renderPass)
{

	VkImageView attachments[] = {
		imageView.Handle()
	};

	VkFramebufferCreateInfo framebufferInfo{};
	framebufferInfo.sType			= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass		= renderPass.Handle();
	framebufferInfo.attachmentCount = 1;
	framebufferInfo.pAttachments	= attachments;
	framebufferInfo.width			= renderPass_.SwapChain().Extent().width;
	framebufferInfo.height			= renderPass_.SwapChain().Extent().height;
	framebufferInfo.layers			= 1;

	if (vkCreateFramebuffer(imageView.Device().Handle(), &framebufferInfo, nullptr, &framebuffer_) != VK_SUCCESS)
		throw std::runtime_error("Failed to create framebuffer!");
}

Framebuffer::~Framebuffer()
{
	if (framebuffer_ != nullptr)
	{
		vkDestroyFramebuffer(renderPass_.SwapChain().Device()->Handle(), framebuffer_, nullptr);
		framebuffer_ = nullptr;
	}

}