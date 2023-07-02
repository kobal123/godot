#include "text_image_texture.h"

Ref<TextImageTexture> TextImageTexture::create_from_image__(const Ref<Image> &p_image) {
	Ref<TextImageTexture> image_texture;
	image_texture.instantiate();
	image_texture->set_image(p_image);
	return image_texture;
}

Ref<TextImageTexture> TextImageTexture::create_from_image_with_usage__(const Ref<Image> &p_image, uint32_t usage) {
	ERR_FAIL_COND_V_MSG(p_image.is_null() || p_image->is_empty(), Ref<TextImageTexture>(), "Invalid image");

	Ref<TextImageTexture> image_texture;
	image_texture.instantiate();
	image_texture->set_image_with_usage(p_image, usage);

	return image_texture;
}


void TextImageTexture::_bind_methods() {
	ClassDB::bind_static_method("TextImageTexture", D_METHOD("create_from_image__", "image"), &TextImageTexture::create_from_image__);
	ClassDB::bind_static_method("TextImageTexture", D_METHOD("create_from_image_with_usage__", "image", "usage_bits"), &TextImageTexture::create_from_image_with_usage__);
    ClassDB::bind_method(D_METHOD("get_line_col"), &TextImageTexture::get_line_col);

}