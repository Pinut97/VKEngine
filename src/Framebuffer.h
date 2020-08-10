#pragma once
#include "headers.h"

class ImageView;
class RenderPass;

class Framebuffer final {
public:
	Framebuffer(const ImageView& imageView, const RenderPass& renderPass);
	~Framebuffer();

	const VkFramebuffer Handle() { return framebuffer_; }
	const class ImageView& ImageView() const { return imageView_; }
	const class RenderPass& RenderPass() const { return renderPass_; }

private:
	
	VkFramebuffer framebuffer_;
	const class ImageView& imageView_;
	const class RenderPass& renderPass_;

};