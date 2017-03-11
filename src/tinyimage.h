/*
	TinyImage
	A tiny C/C++ image loading and saving library for graphics APIs.
	Author: Charles Dong
*/

#ifndef TINYIMAGE_H_
#define TINYIMAGE_H_

enum TinyImgColorType
{
	TINYIMG_RGB,
	TINYIMG_RGBA
};

enum TinyImgError
{
	TINYIMG_OK,
	TINYIMG_FORMAT_UNSUPPORTED,
	TINYIMG_FILE_NOT_FOUND,
	TINYIMG_INVALID_ARGUMENT
};

/* C-style interface begin */

/*
	@brief Loads an image from disk.

	@param[filename] The name of the image file.
	@param[width] Pointer to an integer that stores the image width (should be not NULL).
	@param[height] Pointer to an integer that stores the image height (should be not NULL).
	@param[type] The color type of the final image data.

	@return The image data in unsigned chars. If an error occurs, then NULL will be returned.
*/
unsigned char * tinyimg_load(const char * filename, int * width, int * height, TinyImgColorType type);

/*
	@brief Frees the image data.

	@param[image] The image data.
*/
void tinyimg_free(unsigned char * image);

/*
	@brief Saves the given image to disk.

	@param[filename] The name of the image file.
	@param[width] The image width.
	@param[height] The image height.
	@param[type] The color type of the image data.
	@param[image] The image data.

	@return 1 if successful, 0 if fails.
*/
int tinyimg_save(const char * filename, int width, int height, TinyImgColorType type, const unsigned char * image);

/*
	@brief Gets the error code of the last error.

	@return The error code. If no error occured, 0 will be returned.
*/
TinyImgError tinyimg_get_error();

/*
	@brief Gets the description of the last error.

	@return The error string. If no error occured, NULL will be returned.
*/
const char * tinyimg_get_error_str();

/* C-style interface end */

/* C++ style interface begin */

#ifdef __cplusplus

#define TINYIMG_BEGIN namespace tinyimg {
#define TINYIMG_END }

#include <vector>

TINYIMG_BEGIN

/*
	@brief Loads an image from disk.

	@param[filename] The name of the image file.
	@param[width] The image width.
	@param[height] The image height.
	@param[type] The color type of the final image data.

	@return The image data in unsigned chars. If an error occurs, then nullptr will be returned.
*/
unsigned char * load(const char * filename, int & width, int & height, TinyImgColorType type);

/*
	@brief Frees the image data.
	
	@param[image] The image data.
*/
void free(unsigned char * image);
/*
	@brief Loads an image from disk into a vector.

	@param[filename] The name of the image file.
	@param[width] The image width.
	@param[height] The image height.
	@param[type] The color type of the final image data.

	@return The image data in a vector. If an error occurs, an empty vector will be returned.
*/
std::vector<unsigned char> load_vec(const char * filename, int & width, int & height, TinyImgColorType type);

/*
	@brief Saves the given image to disk.

	@param[filename] The name of the image file.
	@param[width] The image width.
	@param[height] The image height.
	@param[type] The color type of the image data.
	@param[image] The image data.

	@return true if successful, false if fails.
*/
bool save(const char * filename, int width, int height, TinyImgColorType type, const unsigned char * image);

/*
	@brief Gets the error code of the last error.

	@return The error code. If no error occured, 0 will be returned.
*/
TinyImgError get_error();

/*
@brief Gets the description of the last error.

@return The error string. If no error occured, nullptr will be returned.
*/
const char * get_error_str();

TINYIMG_END

#endif		/* __cplusplus */

#endif		/* TINYIMAGE_H_ */