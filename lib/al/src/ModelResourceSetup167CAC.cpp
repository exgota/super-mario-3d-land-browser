#include <stddef.h>
#include <string.h>

#ifdef NON_MATCHING
// NonMatching: clean reconstruction of the complete 0x00167CAC..0x001684F0
// root, including code after the interior literal pool. Names below describe
// observed binary fields, not a recovered original C++ class or public ABI.
namespace model_resource_167cac {
typedef unsigned int Word;

template<class T> struct Relative {
    int displacement;
    T* get() const {
        return displacement ? reinterpret_cast<T*>(
            reinterpret_cast<char*>(const_cast<Relative*>(this)) + displacement) : 0;
    }
    void set(T* value) {
        displacement = value ? reinterpret_cast<char*>(value) -
            reinterpret_cast<char*>(this) : 0;
    }
};
template<class T> struct Entry {
    Word tree[3];
    Relative<T> value;
};
template<class T> struct Dictionary {
    Word header[7];
    Entry<T> entries[1];
    Entry<T>* begin() { return entries; }
};
template<class T> inline Entry<T>* entries(Dictionary<T>* dictionary) {
    return dictionary ? dictionary->begin() : 0;
}
struct Texture {
    Word type;
    char unknown04[0x2c];
    Word memoryFlags;
};
struct Primitive {
    char unknown00[0x14];
    Word memoryFlags;
};
struct PrimitiveSet {
    Word count;
    Relative<Relative<Primitive> > primitives;
    char unknown08[8];
    Word flags;
};
struct SubShape {
    char unknown00[0x0c];
    Word count;
    Relative<Relative<PrimitiveSet> > primitiveSets;
};
struct Attribute {
    Word type;
    char unknown04[0x0c];
    Word memoryFlags;
};
struct Shape {
    Word type;
    char unknown04[0x28];
    Word subShapeCount;
    Relative<Relative<SubShape> > subShapes;
    Word unknown34;
    Word attributeCount;
    Relative<Relative<Attribute> > attributes;
};
struct ShaderLink {
    char unknown00[0x1c];
    Relative<void> shader;
};
struct Material {
    char unknown00[0x168];
    Word mappingIndex;
    char unknown16c[0x118];
    Relative<ShaderLink> shaderLink;
    Word unknown288;
    Word mapping;
};
struct Mesh {
    char unknown00[0x18];
    Word shapeIndex;
    char unknown1c[0x0c];
    Word primitiveSetIndex;
};
struct Model {
    char unknown00[0xb4];
    int meshCount;
    Relative<Relative<Mesh> > meshes;
    int materialCount;
    Relative<Dictionary<Material> > materials;
    Word shapeCount;
    Relative<Relative<Shape> > shapes;
};
struct Resource {
    char unknown00[0x1c];
    int modelCount;
    Relative<Dictionary<Model> > models;
    Word textureCount;
    Relative<Dictionary<Texture> > textures;
    char unknown2c[0x14];
    Relative<void> shaders;
};
struct Wrapper { Resource* resource; };

static_assert(sizeof(Relative<void>) == 4, "relative pointer");
static_assert(sizeof(Entry<void>) == 16, "dictionary entry");
static_assert(offsetof(Model, shapes) == 0xc8, "model shape array");
static_assert(offsetof(Material, shaderLink) == 0x284, "material shader link");
static_assert(offsetof(Material, mapping) == 0x28c, "material mapping");
}

extern "C" {
// Neutral declarations borrow existing whole map rows. No helper body or
// unproven original class identity is supplied by this translation unit.
void* fn_0025C334(void*, const char*, unsigned int);
void fn_0028ECAC(void*);
void fn_0028EED4(void*);
void fn_002A9DC4(void*, unsigned int);
unsigned int fn_002AA23C(void*, void*, void*);
extern void* dat_003E26CC;
extern const unsigned int dat_003A2D40[]; // whole 32-byte row, never partitioned
}

extern "C" unsigned int fn_00167CAC(void*, void* resourceView,
        void* contextView, unsigned int flags, void* secondaryView) {
    using namespace model_resource_167cac;
    Wrapper* primary = static_cast<Wrapper*>(resourceView);
    Wrapper* secondary = static_cast<Wrapper*>(secondaryView);
    const Word memoryFlags = flags | 0x02000000;

    Entry<Texture>* textureEnd = entries(primary->resource->textures.get()) +
        primary->resource->textureCount;
    for (Entry<Texture>* entry = entries(primary->resource->textures.get());
            entry != textureEnd; ++entry) {
        Texture* texture = entry->value.get();
        if (texture->type == 0x20000009 || texture->type == 0x20000011)
            texture->memoryFlags = memoryFlags;
    }

    Entry<Model>* modelEnd = entries(primary->resource->models.get()) +
        primary->resource->modelCount;
    for (Entry<Model>* entry = entries(primary->resource->models.get());
            entry != modelEnd; ++entry) {
        Relative<Shape>* shapeEnd = entry->value.get()->shapes.get() +
            entry->value.get()->shapeCount;
        for (Relative<Shape>* shape = entry->value.get()->shapes.get();
                shape != shapeEnd; ++shape) {
            Relative<SubShape>* subEnd = shape->get()->subShapes.get() +
                shape->get()->subShapeCount;
            for (Relative<SubShape>* sub = shape->get()->subShapes.get();
                    sub != subEnd; ++sub) {
                Relative<PrimitiveSet>* setEnd = sub->get()->primitiveSets.get() +
                    sub->get()->count;
                for (Relative<PrimitiveSet>* set = sub->get()->primitiveSets.get();
                        set != setEnd; ++set) {
                    Relative<Primitive>* primitiveEnd = set->get()->primitives.get() +
                        set->get()->count;
                    for (Relative<Primitive>* primitive = set->get()->primitives.get();
                            primitive != primitiveEnd; ++primitive)
                        primitive->get()->memoryFlags = memoryFlags;
                }
            }
        }
    }

    modelEnd = entries(primary->resource->models.get()) + primary->resource->modelCount;
    for (Entry<Model>* entry = entries(primary->resource->models.get());
            entry != modelEnd; ++entry) {
        Relative<Shape>* shapeEnd = entry->value.get()->shapes.get() +
            entry->value.get()->shapeCount;
        for (Relative<Shape>* shape = entry->value.get()->shapes.get();
                shape != shapeEnd; ++shape) {
            Shape* typedShape = shape->get();
            if (typedShape && (typedShape->type & 0x10000001) != 0x10000001)
                typedShape = 0;
            if (typedShape) {
                Relative<Attribute>* attributeEnd = typedShape->attributes.get() +
                    typedShape->attributeCount;
                for (Relative<Attribute>* attribute = typedShape->attributes.get();
                        attribute != attributeEnd; ++attribute) {
                    Attribute* typedAttribute = attribute->get();
                    if (typedAttribute && !(typedAttribute->type & 0x40000000))
                        typedAttribute = 0;
                    if (typedAttribute) typedAttribute->memoryFlags = memoryFlags;
                }
            }
        }
    }

    fn_0028EED4(dat_003E26CC);
    if (secondary && primary->resource->modelCount > 0) {
        void* shaderDictionary = secondary->resource->shaders.get();
        void* fastShader = 0;
        if (shaderDictionary) {
            Entry<void>* shaderEntry = static_cast<Entry<void>*>(fn_0025C334(
                &shaderDictionary, "FastShader", strlen("FastShader")));
            if (shaderEntry) fastShader = shaderEntry->value.get();
        }
        if (primary->resource->modelCount > 0) {
            Model* first = entries(primary->resource->models.get())->value.get();
            for (int i = 0; i < first->materialCount; ++i) {
                Dictionary<Material>* materials = first->materials.get();
                Material* material = materials ? materials->entries[i].value.get() : 0;
                if (material->shaderLink.get() && !material->shaderLink.get()->shader.get())
                    material->shaderLink.get()->shader.set(fastShader);
            }
        }
        fn_002AA23C(primary, contextView, secondary->resource);
    }
    Word result = fn_002AA23C(primary, contextView, primary->resource);
    if (primary->resource->modelCount > 0) {
        modelEnd = entries(primary->resource->models.get()) + primary->resource->modelCount;
        for (Entry<Model>* entry = entries(primary->resource->models.get());
                entry != modelEnd; ++entry) {
            Entry<Material>* materialEnd = entries(entry->value.get()->materials.get()) +
                entry->value.get()->materialCount;
            for (Entry<Material>* materialEntry = entries(entry->value.get()->materials.get());
                    materialEntry != materialEnd; ++materialEntry) {
                Material* material = materialEntry->value.get();
                if (material->shaderLink.get()) {
                    Word mapping[4] = { dat_003A2D40[0], dat_003A2D40[1],
                        dat_003A2D40[2], dat_003A2D40[3] };
                    material->mapping = mapping[material->mappingIndex];
                }
            }
        }
        Model* first = entries(primary->resource->models.get())->value.get();
        for (int i = 0; i < first->meshCount; ++i) {
            Mesh* mesh = first->meshes.get()[i].get();
            Shape* shape = first->shapes.get()[mesh->shapeIndex].get();
            Relative<SubShape>* subEnd = shape->subShapes.get() + shape->subShapeCount;
            for (Relative<SubShape>* sub = shape->subShapes.get(); sub != subEnd; ++sub) {
                PrimitiveSet* set = sub->get()->primitiveSets.get()[mesh->primitiveSetIndex].get();
                if (!(set->flags & 1)) fn_002A9DC4(&set, 0);
            }
        }
    }
    fn_0028ECAC(dat_003E26CC);
    return result;
}

#endif // NON_MATCHING
