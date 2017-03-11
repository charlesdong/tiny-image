#include "tinyimage.h"

#include <stdio.h>
#include <string.h>

/* Prevent error C4996 in Visual Studio compilers */
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

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
	int size;

	/* Checks if the pointers are not NULL */
	if (width == NULL || height == NULL)
	{
		_error = TINYIMG_INVALID_ARGUMENT;
		return NULL;
	}

	fseek(fp, 18, SEEK_SET);
	fread(width, sizeof(int), 1, fp);		// read width
	fread(height, sizeof(int), 1, fp);		// read height

	/* Allocates memory for the image */
	size = 3 * (*width) * (*height);
	image = (unsigned char *)malloc(sizeof(unsigned char) * size);

	/* Reads the image data */
	fseek(fp, 54, SEEK_SET);
	fread(image, sizeof(unsigned char), size, fp);

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