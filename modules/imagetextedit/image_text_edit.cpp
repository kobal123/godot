#include "image_text_edit.h"


void ImageTextEdit::_notification(int p_what) {
    TextEdit::_notification(p_what);

    if (p_what == NOTIFICATION_DRAW) {


    }
}


                                                          /*const String &p_text*/
void ImageTextEdit::_base_insert_image(int p_line, int p_char, Ref<TextImageTexture> image, int &r_end_line, int &r_end_column) {
	// Save for undo.
	ERR_FAIL_INDEX(p_line, text.size());
	ERR_FAIL_COND(p_char < 0);

	/* STEP 1: Remove \r from source text and separate in substrings. */
	const String text_to_insert = String::chr(0xfffc);
    // const String text_to_insert = String();
	Vector<String> substrings = text_to_insert.split("\n");

	// Is this just a new empty line?
	bool shift_first_line = p_char == 0 && substrings.size() == 2 && text_to_insert == "\n";

	/* STEP 2: Add spaces if the char is greater than the end of the line. */
	while (p_char > text[p_line].length()) {
		text.set(p_line, text[p_line] + String::chr(' '), structured_text_parser(st_parser, st_args, text[p_line] + String::chr(' ')));
	}

	/* STEP 3: Separate dest string in pre and post text. */
	String postinsert_text = text[p_line].substr(p_char, text[p_line].size());

	substrings.write[0] = text[p_line].substr(0, p_char) + substrings[0];
	substrings.write[substrings.size() - 1] += postinsert_text;

	Vector<Array> bidi_override;
	bidi_override.resize(substrings.size());
	for (int i = 0; i < substrings.size(); i++) {
		bidi_override.write[i] = structured_text_parser(st_parser, st_args, substrings[i]);
	}

	text.insert_image(p_line, substrings, image, bidi_override);

	if (shift_first_line) {
		text.move_gutters(p_line, p_line + 1);
		text.set_hidden(p_line + 1, text.is_hidden(p_line));

		text.set_hidden(p_line, false);
	}

	r_end_line = p_line + substrings.size() - 1;
	r_end_column = text[r_end_line].length() - postinsert_text.length();

	TextServer::Direction dir = TS->shaped_text_get_dominant_direction_in_range(text.get_line_data(r_end_line)->get_rid(), (r_end_line == p_line) ? carets[0].column : 0, r_end_column);
	if (dir != TextServer::DIRECTION_AUTO) {
		input_direction = (TextDirection)dir;
	}

	if (!text_changed_dirty && !setting_text) {
		if (is_inside_tree()) {
			MessageQueue::get_singleton()->push_call(this, "_text_changed_emit");
		}
		text_changed_dirty = true;
	}
	emit_signal(SNAME("lines_edited_from"), p_line, r_end_line);
}

void ImageTextEdit::_insert_image(int p_line, int p_char, Ref<TextImageTexture> image, int *r_end_line, int *r_end_char) {
	if (!setting_text && idle_detect->is_inside_tree()) {
		idle_detect->start();
	}

	if (undo_enabled) {
		_clear_redo();
	}

	int retline, retchar;
	_base_insert_image(p_line, p_char, image, retline, retchar);
	if (r_end_line) {
		*r_end_line = retline;
	}
	if (r_end_char) {
		*r_end_char = retchar;
	}

	if (!undo_enabled) {
		return;
	}

	/* UNDO!! */
	TextOperation op;
	op.type = TextOperation::TYPE_INSERT;
	op.from_line = p_line;
	op.from_column = p_char;
	op.to_line = retline;
	op.to_column = retchar;
	op.text = String::chr(0xfffc);
	op.version = ++version;
	op.chain_forward = false;
	op.chain_backward = false;
	op.start_carets = carets;
	op.end_carets = carets;

	// See if it should just be set as current op.
	if (current_op.type != op.type) {
		op.prev_version = get_version();
		_push_current_op();
		current_op = op;

		return; // Set as current op, return.
	}
	// See if it can be merged.
	if (current_op.to_line != p_line || current_op.to_column != p_char) {
		op.prev_version = get_version();
		_push_current_op();
		current_op = op;
		return; // Set as current op, return.
	}
	// Merge current op.

	current_op.text += String::chr(0xfffc);
	current_op.to_column = retchar;
	current_op.to_line = retline;
	current_op.version = op.version;
	current_op.end_carets = carets;
}

void ImageTextEdit::insert_image_at_caret(Ref<TextImageTexture> image, int p_caret) {
	ERR_FAIL_COND(p_caret > carets.size());
    
	begin_complex_operation();
	int from_line = 0;
    int from_col = 0;

    
    Vector<int> caret_edit_order = get_caret_index_edit_order();
	for (const int &i : caret_edit_order) {
		if (p_caret != -1 && p_caret != i) {
			continue;
		}

		delete_selection(i);

		from_line = get_caret_line(i);
		from_col = get_caret_column(i);

		int new_column, new_line;
        image->set_line(from_line);
        image->set_col(from_col);
        RID line_rid = text.text.write[from_line].data_buf->get_rid();
        
        Array objects = TS->shaped_text_get_objects(line_rid);
        int obj_size = objects.size();

        for (int i = 0; i < obj_size; i++) {
            Ref<TextImageTexture> img = Object::cast_to<TextImageTexture>(objects[i]);
            if (from_col <= img->col ) 
                img->col++;

        }

		_insert_image(from_line, from_col, image, &new_line, &new_column);
		_update_scrollbars();

		set_caret_line(new_line, false, true, 0, i);
		set_caret_column(new_column, i == 0, i);

		adjust_carets_after_edit(i, new_line, new_column, from_line, from_col);
	}

	if (!ime_text.is_empty()) {
		for (int i = 0; i < carets.size(); i++) {
			String t;
			if (get_caret_column(i) >= 0) {
				t = text[get_caret_line(i)].substr(0, get_caret_column(i)) + ime_text + text[get_caret_line(i)].substr(get_caret_column(i), text[get_caret_line(i)].length());
			} else {
				t = ime_text;
			}
			text.invalidate_cache(get_caret_line(i), get_caret_column(i), true, t, structured_text_parser(st_parser, st_args, t));
		}
	}

    // image_pos.insert(image->get_rid(), LinePos(from_line, from_col));
    

	end_complex_operation();
	queue_redraw();
}


void ImageTextEdit::_bind_methods() {
    ClassDB::bind_method(D_METHOD("insert_image_at_caret", "image", "caret_index"), &ImageTextEdit::insert_image_at_caret);


}
