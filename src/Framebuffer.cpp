#include "Framebuffer.h"
#include "ImageView.h"
#include "RenderPass.h"
#include "VulkanSwapChain.h"
#include "VulkanDevice.h"
#include "DepthBuffer.h"

Framebuffer::Framebuffer(const class ImageView& imageView, const class RenderPass& renderPass) :
	imageView_(imageView), renderPass_(renderPass)
{

	std::array<VkImageView, 2> attachments = {
		imageView.Handle(),
		renderPass.DepthBuffer().ImageView().Handle()
	};

	VkFramebufferCreateInfo framebufferInfo{};
	framebufferInfo.sType			= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass		= renderPass.Handle();
	framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	framebufferInfo.pAttachments	= attachments.data();
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