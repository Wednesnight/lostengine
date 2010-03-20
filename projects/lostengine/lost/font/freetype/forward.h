#ifndef LOST_FONT_FREETYPE_FORWARD_H
#define LOST_FONT_FREETYPE_FORWARD_H

namespace lost
{
namespace font
{
namespace freetype
{

struct Face;
typedef lost::shared_ptr<Face> FacePtr;

struct Library;
typedef lost::shared_ptr<Library> LibraryPtr;

}
}
}

struct  FT_LibraryRec_;
typedef struct  FT_LibraryRec_ FT_LibraryRec;
typedef struct FT_LibraryRec_  *FT_Library;

struct  FT_FaceRec_;
typedef struct  FT_FaceRec_ FT_FaceRec;
typedef struct FT_FaceRec_*  FT_Face;

#endif
