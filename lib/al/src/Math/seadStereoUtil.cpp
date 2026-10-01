namespace sead
{

template <typename T>
class Matrix44
{
public:
        T m[ 4 ][ 4 ];
};

class StereoUtil
{
public:
        static void getProjectionParam( float* top, float* bottom, float* left, float* right,
                float* nearDistance, float* farDistance, const Matrix44<float>& matrix );
};

void StereoUtil::getProjectionParam( float* top, float* bottom, float* left, float* right,
        float* nearDistance, float* farDistance, const Matrix44<float>& matrix )
{
        *nearDistance = matrix.m[ 2 ][ 3 ] / matrix.m[ 2 ][ 2 ];
        *farDistance = matrix.m[ 2 ][ 2 ] * *nearDistance / ( matrix.m[ 2 ][ 2 ] + 1.0f );
        *right = *nearDistance * ( matrix.m[ 0 ][ 2 ] + 1.0f ) / matrix.m[ 0 ][ 0 ];
        *left = *right - ( *nearDistance * 2.0f ) / matrix.m[ 0 ][ 0 ];
        *top = *nearDistance * ( matrix.m[ 1 ][ 2 ] + 1.0f ) / matrix.m[ 1 ][ 1 ];
        *bottom = *top - ( *nearDistance * 2.0f ) / matrix.m[ 1 ][ 1 ];
}

} // namespace sead
