#ifndef CLEAN_LAYOUT_FRAME_ROOT_H
#define CLEAN_LAYOUT_FRAME_ROOT_H
#include <stddef.h>
namespace layout_frame {
struct Vec2 { float x,y; };
struct Insets { float left,right,top,bottom; };
struct TextureSize { unsigned short width,height; };
struct TexCoords { Vec2 corner[4]; };
struct Texture { unsigned char opaque00[8]; TextureSize size; };
struct Material { unsigned char opaque00[0x30]; unsigned format; Texture* textures; unsigned char opaque38[0x15]; unsigned char flags; };
struct Frame { unsigned char orientation; unsigned char opaque01[3]; Material* material; };
struct Root {
 unsigned char opaque00[0x48]; Vec2 size;
 unsigned char opaque50[0x30]; float matrix[12];
 unsigned char opaqueB0[5]; unsigned char alpha,origin;
 unsigned char opaqueB7[0x1b]; unsigned char cacheReady;
 unsigned char opaqueD3; unsigned cachedCount; float cachedCoords[24];
 Insets extension; unsigned colors[4]; unsigned char opaque158[4];
 TexCoords* contentCoords; Frame* frames; unsigned char frameCount;
 unsigned char opaque165[3]; Material* contentMaterial;
};
// These are observed prefixes, not established whole allocations or class names.
static_assert(offsetof(Root,cacheReady)==0xd2,"cache flag");
static_assert(offsetof(Root,cachedCoords)==0xd8,"cache storage");
static_assert(offsetof(Root,extension)==0x138,"content extension");
static_assert(offsetof(Root,frames)==0x160,"frame array");
static_assert(offsetof(Root,contentMaterial)==0x168,"content material");
static_assert(sizeof(Frame)==8,"frame stride");
static_assert(offsetof(Material,flags)==0x4d,"material flag");
}
namespace nn { namespace math { struct MTX34; }}
namespace nw { namespace lyt { struct Drawer { void SetUpMtx(const nn::math::MTX34&); }; }}
extern "C" void fn_0033CE74(layout_frame::Root*,void*,nw::lyt::Drawer*);
#endif
