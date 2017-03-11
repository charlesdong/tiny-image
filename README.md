# TinyImage
A tiny C/C++ image loading and saving library for graphics APIs.

## Before using
Directly download this repository to your computer.

Create or open a project in your C/C++ compiler, and add `tinyimage.cpp` to your project.

Note that if you're using C, please rename `tinyimage.cpp` to `tinyimage.c`.

## How to use
TinyImage provides both C and C++ interfaces for users, so there are some differences between the two interfaces. But it's OK to use the C interface even though you're using C++.

### Image loading

#### C
Use the `tinyimg_load()` function to load an image from disk, for example:

```c
int width, height;
unsigned char * image = tinyimg_load("image.bmp", &width, &height, TINYIMG_LOAD_RGB);
```

When the image is no more being used, you should call the `tinyimg_free()` function to free the image data:

```c
tinyimg_free(image);
```

#### C++
Use the `load()` function in the namespace `tinyimg` to load an image from disk, for example:

```c++
int width, height;
unsigned char * image = tinyimg::load("image.bmp", width, height, TINYIMG_LOAD_RGB);
```

You can also use a `vector` to store the image data, and call `load_vec()` to load the image:

```c++
#include <vector>
...
int width, height;
vector<unsigned char> image = tinyimg::load_vec("image.bmp", width, height, TINYIMG_LOAD_RGB);
```

When the image is no more being used, you should call the `free()` function to free the image data, if the image data is stored in an `unsigned char` array (instead of `vector`):

```c++
tinyimg::free(image);
```
