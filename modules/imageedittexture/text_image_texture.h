#ifndef TEXT_IMAGE_TEXTURE_H
#define TEXT_IMAGE_TEXTURE_H

#include <scene/resources/texture.h>

class TextImageTexture : public ImageTexture {
    GDCLASS(TextImageTexture, ImageTexture);

public:
    int line = 0;
    int col = 0;

    void set_line(int p_line) {line = p_line;}
    void set_col(int p_col) {col = p_col;}

};

#endif // TEXT_IMAGE_TEXTURE_H