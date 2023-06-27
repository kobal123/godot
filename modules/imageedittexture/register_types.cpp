/* register_types.cpp */

#include "register_types.h"
#include "core/object/class_db.h"
#include "text_image_texture.h"




void initialize_imageedittexture_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
		ClassDB::register_class<TextImageTexture>();

	}

void uninitialize_imageedittexture_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}