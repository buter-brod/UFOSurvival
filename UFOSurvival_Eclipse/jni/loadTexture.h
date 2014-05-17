#ifndef LOADTEXTURE_H_
#define LOADTEXTURE_H_

extern "C"
{
#include "../modules/png/png.h"
}

#include <android_native_app_glue.h>

extern AAssetManager *g_assetManager;

void png_read_callback1(png_structp png, png_bytep data, png_size_t size)
{
  AAsset *asset = (AAsset*)png_get_io_ptr(png);
  int32_t lReadCount = AAsset_read(asset, data, size);
  if(lReadCount == 0)
  {
    AAsset_close(asset);
    png_error(png, "Error while reading PNG file");
  }
}

uint8_t* loadPngImage(std::string path, unsigned int &width, unsigned int &height, GLint& format)
{
  png_byte    lHeader[8];
  png_structp lPngPtr      = NULL;
  png_infop   lInfoPtr     = NULL;
  png_byte*   lImageBuffer = NULL;
  png_bytep*  lRowPtrs     = NULL;
  png_int_32  lRowSize;
  bool lTransparency;

  AAsset* asset  = AAssetManager_open(g_assetManager, path.c_str(), AASSET_MODE_UNKNOWN);
  int32_t lReadCount = AAsset_read(asset, lHeader, sizeof(lHeader));

  png_sig_cmp(lHeader, 0, 8);

  lPngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  lInfoPtr = png_create_info_struct(lPngPtr);
  png_set_read_fn(lPngPtr, asset, png_read_callback1);
  setjmp(png_jmpbuf(lPngPtr));

  png_set_sig_bytes(lPngPtr, 8);
  png_read_info(lPngPtr, lInfoPtr);

  png_int_32 lDepth, lColorType;
  png_uint_32 lWidth, lHeight;
  png_get_IHDR(lPngPtr, lInfoPtr, &lWidth, &lHeight,
    &lDepth, &lColorType, NULL, NULL, NULL);
  width = lWidth; height = lHeight;

  // Creates a full alpha channel if transparency is encoded as
  // an array of palette entries or a single transparent color.
  lTransparency = false;
  if(png_get_valid(lPngPtr, lInfoPtr, PNG_INFO_tRNS)) {
    png_set_tRNS_to_alpha(lPngPtr);
    lTransparency = true;
  }
  // Expands PNG with less than 8bits per channel to 8bits.
  if(lDepth < 8) {
    png_set_packing(lPngPtr);
    // Shrinks PNG with 16bits per color channel down to 8bits.
  }
  else if(lDepth == 16) {
    png_set_strip_16(lPngPtr);
  }
  // Indicates that image needs conversion to RGBA if needed.
  switch(lColorType) {
  case PNG_COLOR_TYPE_PALETTE:
    png_set_palette_to_rgb(lPngPtr);
    format = lTransparency ? GL_RGBA : GL_RGB;
    break;
  case PNG_COLOR_TYPE_RGB:
    format = lTransparency ? GL_RGBA : GL_RGB;
    break;
  case PNG_COLOR_TYPE_RGBA:
    format = GL_RGBA;
    break;
  case PNG_COLOR_TYPE_GRAY:
    png_set_expand_gray_1_2_4_to_8(lPngPtr);
    format = lTransparency ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
    break;
  case PNG_COLOR_TYPE_GA:
    png_set_expand_gray_1_2_4_to_8(lPngPtr);
    format = GL_LUMINANCE_ALPHA;
    break;
  }
  png_read_update_info(lPngPtr, lInfoPtr);

  lRowSize = png_get_rowbytes(lPngPtr, lInfoPtr);
  lImageBuffer = new png_byte[lRowSize * lHeight];
  lRowPtrs = new png_bytep[lHeight];
  for(int32_t i = 0; i < lHeight; ++i)
  {
    lRowPtrs[lHeight - (i + 1)] = lImageBuffer + i * lRowSize;
  }
  png_read_image(lPngPtr, lRowPtrs);

  AAsset_close(asset);

  png_destroy_read_struct(&lPngPtr, &lInfoPtr, NULL);
  delete[] lRowPtrs;
  return lImageBuffer;
}

GLuint loadTexture(std::string path)
{
  unsigned int width;
  unsigned int height;
  GLint format;

  uint8_t* lImageBuffer = loadPngImage(path, width, height, format);
  if(lImageBuffer == NULL)
    return false;

  GLuint texId;
  GLenum lErrorResult;
  glGenTextures(1, &texId);
  glBindTexture(GL_TEXTURE_2D, texId);

  // Set-up texture properties.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
    GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
    GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
    GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
    GL_CLAMP_TO_EDGE);

  // Loads image data into OpenGL.
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
    format, GL_UNSIGNED_BYTE, lImageBuffer);
  delete[] lImageBuffer;
  if(glGetError() != GL_NO_ERROR)
  {
    glDeleteTextures(1, &texId);
    return 0;
  }
  return texId;
}

#endif /* LOADTEXTURE_H_ */
