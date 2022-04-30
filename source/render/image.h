#pragma once
#include <inttypes.h>
#include <stdio.h>

uint8_t *LoadImageFromMemory(const uint8_t *buffer, int len, int *x, int *y, int *channels_in_file, int desired_channels);

uint8_t *LoadImageFromFile(FILE *f, int *x, int *y, int *channels_in_file, int desired_channels);


void FreeImage(void *ValueFromLoadImage);
