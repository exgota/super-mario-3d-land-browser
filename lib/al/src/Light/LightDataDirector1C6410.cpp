#include <Light/LightDataDirector1C6410.h>

using namespace dot1c6410;
extern "C" {
extern unsigned int dat_003E23B8[];
sead::Heap* fn_00293088(unsigned int);
bool fn_002907E0(const sead::SafeString&);
CreatedObject* fn_0029C228(const CreateArgument&, Allocator&);
CreatedObject* fn_0029A4DC(const CreateArgument&, Allocator&);
int fn_002512F0(al::Resource*, const sead::SafeString&);
void fn_00251204(al::Resource*, Path&, const sead::SafeString&, int);
const u8* fn_0027AD28(al::Resource*, const Path&);
void fn_001C4F64(Area*, const al::ByamlIter&);
void fn_001D4B5C(Map*, const al::ByamlIter&);
bool fn_002D5918(const Area&, const Area&);
bool fn_002D5934(const Map&, const Map&);
void fn_0039F3FC(Area*, Area*, int, bool (*)(const Area&, const Area&));
void fn_0039E5BC(Area*, Area*, bool (*)(const Area&, const Area&));
void fn_003A0908(Area*, const Area&, bool (*)(const Area&, const Area&));
void fn_0039FE3C(Map*, Map*, int, bool (*)(const Map&, const Map&));
void fn_0039EA0C(Map*, Map*, bool (*)(const Map&, const Map&));
}
namespace al { bool isEqualSubString(const char*, const char*); }

#ifdef NON_MATCHING
// Full-root recovery. Original sort boundaries deliberately pass back(), which
// excludes the last populated record from the half-open sorting range.
Director::Director()
    : stageArea(new Area("\x83\x58\x83\x65\x81\x5b\x83\x57\x83\x66\x83\x74\x83\x48\x83\x8b\x83\x67\x83\x89\x83\x43\x83\x67", "\x83\x58\x83\x65\x81\x5b\x83\x57")),
      stageMap(new Map("\x83\x58\x83\x65\x81\x5b\x83\x57\x83\x66\x83\x74\x83\x48\x83\x8b\x83\x67\x92\x6e\x8c\x60\x83\x89\x83\x43\x83\x67", "\x83\x58\x83\x65\x81\x5b\x83\x57")),
      unknown28(), unknown44c(0), firstObject(0), firstResource(0),
      secondObject(0), fallback0(0), fallback1(0), unknown47c(0)
{
    Allocator allocator(fn_00293088(dat_003E23B8[0]));
    CreateArgument firstArgument;
    firstObject = fn_0029C228(firstArgument, allocator);
    CreateArgument secondArgument;
    secondObject = fn_0029A4DC(secondArgument, allocator);
    firstResource = firstObject->resource;

    if (fn_002907E0(sead::SafeString("ObjectData/LightDataArea"))) {
        al::Resource* resource = al::findOrCreateResource(sead::SafeString("ObjectData/LightDataArea"));
        if (resource) {
            int size = fn_002512F0(resource, sead::SafeString("/"));
            areas.allocate(size);
            for (int i = 0; i < size; ++i) {
                Path path;
                fn_00251204(resource, path, sead::SafeString("/"), i);
                if (al::isEqualSubString(path.cstr(), ".byml")) {
                    al::ByamlIter byaml(fn_0027AD28(resource, path));
                    Area value("NULL", "\x83\x47\x83\x8a\x83\x41");
                    fn_001C4F64(&value, byaml);
                    areas.append(value);
                }
            }
            Area* first = areas.front();
            Area* last = areas.back();
            if (first != last) {
                fn_0039F3FC(first, last, last - first, fn_002D5918);
                if (last - first > 16) {
                    fn_0039E5BC(first, first + 16, fn_002D5918);
                    for (Area* at = first + 16; at != last; ++at) {
                        Area value(*at);
                        fn_003A0908(at, value, fn_002D5918);
                    }
                } else fn_0039E5BC(first, last, fn_002D5918);
            }
        }
    }
    if (fn_002907E0(sead::SafeString("ObjectData/LightDataMap"))) {
        al::Resource* resource = al::findOrCreateResource(sead::SafeString("ObjectData/LightDataMap"));
        if (resource) {
            int size = fn_002512F0(resource, sead::SafeString("/"));
            maps.allocate(size);
            for (int i = 0; i < size; ++i) {
                Path path;
                fn_00251204(resource, path, sead::SafeString("/"), i);
                if (al::isEqualSubString(path.cstr(), ".byml")) {
                    al::ByamlIter byaml(fn_0027AD28(resource, path));
                    Map value("NULL", "\x92\x6e\x8c\x60");
                    fn_001D4B5C(&value, byaml);
                    maps.append(value);
                }
            }
            Map* first = maps.front();
            Map* last = maps.back();
            if (first != last) {
                fn_0039FE3C(first, last, last - first, fn_002D5934);
                if (last - first > 16) {
                    fn_0039EA0C(first, first + 16, fn_002D5934);
                    for (Map* at = first + 16; at != last; ++at) {
                        Map value(*at);
                        Map* destination = at;
                        Map* previous = at - 1;
                        while (fn_002D5934(value, *previous)) {
                            *destination = *previous;
                            destination = previous--;
                        }
                        *destination = value;
                    }
                } else fn_0039EA0C(first, last, fn_002D5934);
            }
        }
    }
    fallback0 = new Light(Color(0, 0, 0, 0), Color(0, 0, 0, 0),
                          Color(.5f, .5f, .5f, 0), Color(.5f, .5f, .5f, 0),
                          Color(0, 0, 0, 0), sead::Vector3f(0, 0, 1), false);
    fallback1 = new Light(Color(.23f, .22f, .178f, 1), Color(.255f, .254f, .213f, 1),
                          Color(.5f, .5f, .5f, 1), Color(.5f, .5f, .5f, 1),
                          Color(.2f, .2f, .155f, 1), sead::Vector3f(-1, -1, -.7f), true);
}
#endif
