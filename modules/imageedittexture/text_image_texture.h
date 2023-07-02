#ifndef TEXT_IMAGE_TEXTURE_H
#define TEXT_IMAGE_TEXTURE_H

#include <scene/resources/texture.h>

class TextImageTexture : public ImageTexture {
    GDCLASS(TextImageTexture, ImageTexture);

public:
    int line;
    int col;

    void set_line(int p_line) {line = p_line;}
    void set_col(int p_col) {col = p_col;}
    Vector2i get_line_col() const { return Vector2i(line, col);}
    static Ref<TextImageTexture> create_from_image__(const Ref<Image> &p_image);
    static Ref<TextImageTexture> create_from_image_with_usage__(const Ref<Image> &p_image, uint32_t usage);

    TextImageTexture() {
        line = 0;
        col = 0;
    }

    ~TextImageTexture() {
        // Clean up any resources here
    }
protected:

	static void _bind_methods();
};

#endif // TEXT_IMAGE_TEXTURE_H