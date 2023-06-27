/* register_types.cpp */

#include "register_types.h"
#include "core/object/class_db.h"
#include "image_text_edit.h"




void initialize_imagetextedit_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
		ClassDB::register_class<ImageTextEdit>();

	}

void uninitialize_imagetextedit_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}