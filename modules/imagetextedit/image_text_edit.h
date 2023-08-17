#ifndef BBCODE_TEXT_EDIT_H
#define BBCODE_TEXT_EDIT_H

#include <scene/gui/text_edit.h>

class ImageTextEdit : public TextEdit {
    GDCLASS(ImageTextEdit, TextEdit);

protected:
    static void _bind_methods();
    void _notification(int p_what);
    

public:
    void insert_image_at_caret(Ref<TextImageTexture> image, int p_caret, bool should_insert_char);
    

private:
    class LinePos {
        public:
            int line;
            int col;

            LinePos(int lineValue, int colValue) {
                line = lineValue;
                col = colValue;
            }


    };
	void _insert_image(int p_line, int p_char, Ref<TextImageTexture> image, int *r_end_line = nullptr, int *r_end_char = nullptr, bool should_insert_char = true);
	void _base_insert_image(int p_line, int p_char, Ref<TextImageTexture> image, int &r_end_line, int &r_end_column, bool should_insert_char);
    
};

#endif // BBCODE_TEXT_EDIT_H
