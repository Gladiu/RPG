#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb/stb_image.h"

uint8_t *LoadImageFromMemory(const uint8_t *buffer, int len, int *x, int *y, int *channels_in_file, int desired_channels){

	return stbi_load_from_memory(buffer, len, x, y, channels_in_file, desired_channels);
}

uint8_t *LoadImageFromFile(FILE *f, int *x, int *y, int *channels_in_file, int desired_channels){

	return stbi_load_from_file(f, x, y, channels_in_file, desired_channels);
}

void FreeImage(void *ValueFromLoadImage){
	stbi_image_free(ValueFromLoadImage);
}
