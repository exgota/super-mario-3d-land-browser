#include <clean/LayoutFrameRoot.h>
#ifdef NON_MATCHING
using namespace layout_frame;
using nw::lyt::Drawer;
extern "C" {
Insets fn_0033CCF4(Root*,unsigned,Frame*);
Vec2 fn_00215DA0(Root*);
void fn_00215924(Drawer*,Material*);
void fn_0022CA44(Drawer*,Material*,bool);
unsigned fn_00215424(Drawer*,Material*,const TexCoords*,float*);
void fn_0021539C(Drawer*,const float*,unsigned);
void fn_00215350(Drawer*,const Vec2*,const Vec2*);
void fn_0021503C(Drawer*,const unsigned*,unsigned);
void fn_00214FD8(Drawer*);
void fn_00214E98(TexCoords*,const Vec2*,const TextureSize*,unsigned);
void fn_00214D58(TexCoords*,const Vec2*,const TextureSize*,unsigned);
void fn_00214C18(TexCoords*,const Vec2*,const TextureSize*,unsigned);
void fn_00214AD8(TexCoords*,const Vec2*,const TextureSize*,unsigned);
void fn_0022CC14(Drawer*);
}
namespace {
__forceinline bool present(const Frame& f) { return (f.material->format&3)!=0; }
__forceinline void begin(Drawer* drawer, Frame& frame) {
 fn_00215924(drawer,frame.material);
 fn_0022CA44(drawer,frame.material,true);
}
__forceinline void submit(Drawer* drawer, Frame& frame, const TexCoords& coords, const Vec2& size, const Vec2& position) {
 float transformed[24];
 unsigned count=fn_00215424(drawer,frame.material,&coords,transformed);
 fn_0021539C(drawer,transformed,count);
 fn_00215350(drawer,&size,&position);
 fn_00214FD8(drawer);
}
__forceinline void top(Drawer* drawer,Frame& frame,const Vec2& size,const Vec2& position,unsigned orientation,const TextureSize* captured=0) {
 TexCoords coords;TextureSize texture=captured ? *captured : frame.material->textures->size;
 fn_00214E98(&coords,&size,&texture,orientation);
 submit(drawer,frame,coords,size,position);
}
__forceinline void right(Drawer* drawer,Frame& frame,const Vec2& size,const Vec2& position,unsigned orientation,const TextureSize* captured=0) {
 TexCoords coords;TextureSize texture=captured ? *captured : frame.material->textures->size;
 fn_00214D58(&coords,&size,&texture,orientation);
 submit(drawer,frame,coords,size,position);
}
__forceinline void bottom(Drawer* drawer,Frame& frame,const Vec2& size,const Vec2& position,unsigned orientation,const TextureSize* captured=0) {
 TexCoords coords;TextureSize texture=captured ? *captured : frame.material->textures->size;
 fn_00214C18(&coords,&size,&texture,orientation);
 submit(drawer,frame,coords,size,position);
}
__forceinline void left(Drawer* drawer,Frame& frame,const Vec2& size,const Vec2& position,unsigned orientation,const TextureSize* captured=0) {
 TexCoords coords;TextureSize texture=captured ? *captured : frame.material->textures->size;
 fn_00214AD8(&coords,&size,&texture,orientation);
 submit(drawer,frame,coords,size,position);
}
}
extern "C" void fn_0033CE74(Root* self,void*,Drawer* drawer) {
 Insets border=fn_0033CCF4(self,self->frameCount,self->frames);
 Vec2 origin=fn_00215DA0(self);
 fn_00215924(drawer,self->contentMaterial);
 fn_0022CA44(drawer,self->contentMaterial,true);
 if (!self->cacheReady || !(self->contentMaterial->flags&4)) {
  self->cachedCount=fn_00215424(drawer,self->contentMaterial,self->contentCoords,self->cachedCoords);
  self->cacheReady=1;
  self->contentMaterial->flags=(self->contentMaterial->flags&0xfb)|4;
 }
 fn_0021539C(drawer,self->cachedCoords,self->cachedCount);
 drawer->SetUpMtx(*reinterpret_cast<nn::math::MTX34*>(self->matrix));
 float insideX=origin.x+border.left;
 float insideY=origin.y-border.top;
 Vec2 size={((self->size.x-border.left)+self->extension.left)-border.right+self->extension.right,
            ((self->size.y-border.top)+self->extension.top)-border.bottom+self->extension.bottom};
 Vec2 position={insideX-self->extension.left,insideY+self->extension.top};
 fn_00215350(drawer,&size,&position);
 fn_0021503C(drawer,self->colors,self->alpha);
 fn_00214FD8(drawer);
 unsigned white[4];
 white[0]=white[1]=white[2]=white[3]=0xffffffff;
 fn_0021503C(drawer,white,self->alpha);
 if (self->frameCount==1) {
  Frame& f=self->frames[0];
  if (!present(f)) return;
  begin(drawer,f);
  TextureSize texture=f.material->textures->size;
  size.x=self->size.x-border.right;size.y=border.top;position=origin;
  top(drawer,f,size,position,0,&texture);
  position.x=(origin.x+self->size.x)-border.right;position.y=origin.y;
  size.x=border.right;size.y=self->size.y-border.bottom;
  right(drawer,f,size,position,1,&texture);
  position.x=insideX;position.y=(origin.y-self->size.y)+border.bottom;
  size.x=self->size.x-border.left;size.y=border.bottom;
  bottom(drawer,f,size,position,4,&texture);
  position.x=origin.x;position.y=insideY;
  size.x=border.left;size.y=self->size.y-border.top;
  left(drawer,f,size,position,2,&texture);
 } else if (self->frameCount==4) {
  Frame& a=self->frames[0];
  if(present(a)){begin(drawer,a);size.x=self->size.x-border.right;size.y=border.top;position=origin;top(drawer,a,size,position,a.orientation);}
  Frame& b=self->frames[1];
  if(present(b)){begin(drawer,b);position.x=(origin.x+self->size.x)-border.right;position.y=origin.y;size.x=border.right;size.y=self->size.y-border.bottom;right(drawer,b,size,position,b.orientation);}
  Frame& c=self->frames[3];
  if(present(c)){begin(drawer,c);position.x=insideX;position.y=(origin.y-self->size.y)+border.bottom;size.x=self->size.x-border.left;size.y=border.bottom;bottom(drawer,c,size,position,c.orientation);}
  Frame& d=self->frames[2];
  if(present(d)){begin(drawer,d);position.x=origin.x;position.y=insideY;size.x=border.left;size.y=self->size.y-border.top;left(drawer,d,size,position,d.orientation);}
 } else if (self->frameCount==8) {
  Frame& a=self->frames[0];
  if(present(a)){begin(drawer,a);size.x=border.left;size.y=border.top;position=origin;top(drawer,a,size,position,a.orientation);}
  Frame& b=self->frames[6];
  if(present(b)){begin(drawer,b);size.x=(self->size.x-border.left)-border.right;size.y=border.top;position.x=insideX;position.y=origin.y;top(drawer,b,size,position,b.orientation);}
  Frame& c=self->frames[1];
  if(present(c)){begin(drawer,c);size.x=border.right;size.y=border.top;position.x=(self->size.x+origin.x)-border.right;position.y=origin.y;right(drawer,c,size,position,c.orientation);}
  Frame& d=self->frames[5];
  if(present(d)){begin(drawer,d);size.x=border.right;size.y=(self->size.y-border.top)-border.bottom;position.x=(self->size.x+origin.x)-border.right;position.y=insideY;right(drawer,d,size,position,d.orientation);}
  Frame& e=self->frames[3];
  if(present(e)){begin(drawer,e);size.x=border.right;size.y=border.bottom;position.x=(self->size.x+origin.x)-border.right;position.y=(origin.y-self->size.y)+border.bottom;bottom(drawer,e,size,position,e.orientation);}
  Frame& f=self->frames[7];
  if(present(f)){begin(drawer,f);size.x=(self->size.x-border.left)-border.right;size.y=border.bottom;position.x=insideX;position.y=(origin.y-self->size.y)+border.bottom;bottom(drawer,f,size,position,f.orientation);}
  Frame& g=self->frames[2];
  if(present(g)){begin(drawer,g);size.x=border.left;size.y=border.bottom;position.x=origin.x;position.y=(origin.y-self->size.y)+border.bottom;left(drawer,g,size,position,g.orientation);}
  Frame& h=self->frames[4];
  if(present(h)){begin(drawer,h);size.x=border.left;size.y=(self->size.y-border.top)-border.bottom;position.x=origin.x;position.y=insideY;left(drawer,h,size,position,h.orientation);}
 }
 if (reinterpret_cast<unsigned char*>(drawer)[0x24]) fn_0022CC14(drawer);
}
#endif
