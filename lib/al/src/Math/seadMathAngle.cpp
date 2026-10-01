namespace MathAngleReconstruction
{

struct ArcTangentEntry
{
        unsigned int index;
        float difference;
};

} // namespace MathAngleReconstruction

extern "C" const MathAngleReconstruction::ArcTangentEntry dat_003C3D6C[ 129 ];

namespace sead
{

template <typename T>
class MathCalcCommon
{
public:
        static unsigned int atanIdx_( float value );
};

template <>
unsigned int MathCalcCommon<float>::atanIdx_( float value )
{
        const float scaled = value * 128.0f;
        const int index = static_cast<int>( scaled );
        const float fraction = scaled - static_cast<float>( index );
        return dat_003C3D6C[ index ].index +
               static_cast<unsigned int>( dat_003C3D6C[ index ].difference * fraction );
}

} // namespace sead
