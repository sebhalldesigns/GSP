#ifndef GTEXTURE_H
#define GTEXTURE_H

// GTexture represents a bitmap image, usually loaded from the file system.

typedef void* GTexture;

GTexture GTexture_AllocFromFile(const char* filePath);
void GTexture_Free(GTexture texture);



#endif // GTEXTURE_H