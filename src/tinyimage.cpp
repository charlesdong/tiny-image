/* Source file for TinyImage */

/* Prevent error C4996 in Visual Studio compilers */
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "tinyimage.h"

#include <stdio.h>
#include <string.h>
#include <math.h>		// for pow()

/* The error code */
TinyImgError _error;

/* Internal functions */

int _equal(const char * str1, const char * str2);
int _tinyimg_check_format(const char * extension);
int _tinyimg_open_file(FILE ** fp, const char * filename);
unsigned char * _tinyimg_load(FILE * fp, const char * extension, int * width, int * height);
unsigned char * _tinyimg_load_bmp(FILE * fp, int * width, int * height);

int _equal(const char * str1, const char * str2)
{
	return strcmp(str1, str2) == 0;
}

int _tinyimg_check_format(const char * extension)
{
	return extension != NULL && (_equal(extension, ".bmp"));
}

int _tinyimg_open_file(FILE ** fp, const char * filename)
{
	*fp = fopen(filename, "rb");
	return *fp != NULL;
}

unsigned char * _tinyimg_load(FILE * fp, const char * extension, int * width, int * height)
{
	if (_equal(extension, ".bmp"))
		return _tinyimg_load_bmp(fp, width, height);
}

unsigned char * _tinyimg_load_bmp(FILE * fp, int * width, int * height)
{
	unsigned char * image;
	unsigned char * palette = NULL;
	int width_file;			/* Pixels per line in BMP (note that BMP has 4-pixel alignment for each line) */
	int size;
	short bit_count;
	int color_count = 0;
	unsigned char temp;
	int count = 0;

	/* Checks if the pointers are not NULL */
	if (width == NULL || height == NULL)
	{
		_error = TINYIMG_INVALID_ARGUMENT;
		return NULL;
	}

	fseek(fp, 18, SEEK_SET);
	fread(width, sizeof(int), 1, fp);		// read width
	fread(height, sizeof(int), 1, fp);		// read height

	fseek(fp, 28, SEEK_SET);
	fread(&bit_count, sizeof(short), 1, fp);	// read bit count

	fseek(fp, 54, SEEK_SET);
	if (bit_count != 24)
	{
		color_count = (int)pow(2.0, bit_count);
		palette = (unsigned char *)malloc(sizeof(unsigned char) * 4 * color_count);
		fread(palette, sizeof(unsigned char), 4 * color_count, fp);
	}

	/* Allocates memory for the image */
	width_file = ((*width) + 3) / 4 * 4;		// Increase the width to make sure it's multiple of 4
	size = width_file * (*height);
	image = (unsigned char *)malloc(sizeof(unsigned char) * size * 3);

	/* Reads the image data */
	if (bit_count == 24)
		fread(image, sizeof(unsigned char), size * 3, fp);
	else
	{
		while (count < size)
		{
			fread(&temp, sizeof(unsigned char), 1, fp);
			image[count * 3] = palette[temp * 4];
			image[count * 3 + 1] = palette[temp * 4 + 1];
			image[count * 3 + 2] = palette[temp * 4 + 2];
			count++;
		}
	}

	fclose(fp);
	return image;
}

/* External functions */

unsigned char * tinyimg_load(const char * filename, int * width, int * height, TinyImgColorType type)
{
	const char * extension;
	FILE * fp;

	/* Check if the image format is supported */
	extension = strrchr(filename, '.');
	if (!_tinyimg_check_format(extension))
	{
		_error = TINYIMG_FORMAT_UNSUPPORTED;
		return NULL;
	}

	/* Check if the file exists */
	if (!_tinyimg_open_file(&fp, filename))
	{
		_error = TINYIMG_FILE_NOT_FOUND;
		return NULL;
	}

	return _tinyimg_load(fp, extension, width, height);
}

void tinyimg_free(unsigned char * image)
{
	free(image);
}

TinyImgError tinyimg_get_error()
{
	return _error;
}

const char * tinyimg_get_error_str()
{
	switch (_error)
	{
	case TINYIMG_OK:
		return NULL;
	case TINYIMG_FORMAT_UNSUPPORTED:
		return "format unsupported";
	case TINYIMG_FILE_NOT_FOUND:
		return "file not found";
	case TINYIMG_INVALID_ARGUMENT:
		return "invalid argument";
	}
}