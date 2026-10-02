#pragma once
// Private observed views for complete EU root 002E0D2C. No original class name claimed.
namespace effect2ea9e8 { struct Attribute; } // Existing proposal's shared attribute interface.
namespace primitive_draw_setup {
typedef unsigned char Byte;
typedef unsigned Word;
struct Vertex { float position[3]; float texture[2]; float color[4]; };
struct IndexBuffer { unsigned short* data; Word physical; Word count; Byte format; Byte padding[3]; };
static_assert_(sizeof(Vertex) == 0x24);
static_assert_(sizeof(IndexBuffer) == 0x10);
}
extern "C" void fn_002E0D2C(void* receiver, void* heap, const void* shaderBinary);
