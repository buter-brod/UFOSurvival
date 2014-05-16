#ifndef _LOADTEXTURE_H_
#define _LOADTEXTURE_H_

#include <FreeImage.h>

GLuint loadTexture(std::string texName)
{
  GLuint texInd = -1;
  FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(texName.c_str(), 0), texName.c_str());

  unsigned int w = FreeImage_GetWidth(bitmap);
  unsigned int h = FreeImage_GetHeight(bitmap);

  glGenTextures(1, &texInd);

  glBindTexture(GL_TEXTURE_2D, texInd);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h,
    0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(bitmap));

  FreeImage_Unload(bitmap);
  return texInd;
}

#endif