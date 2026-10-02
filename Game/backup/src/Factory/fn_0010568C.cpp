namespace
{
typedef unsigned int u32;
}

namespace nn
{
namespace gr
{
namespace CTR
{
class Vertex
{
public:
    class LoadArray
    {
        virtual void load() = 0;

    public:
        LoadArray();
    };
};

Vertex::LoadArray::LoadArray()
{
    *reinterpret_cast<u32*>(this) = 0;
}
}
}
}
