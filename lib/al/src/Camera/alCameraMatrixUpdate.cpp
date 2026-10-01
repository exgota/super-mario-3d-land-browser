#include <Camera/alCameraMatrixUpdate.h>
#include <stddef.h>

namespace
{
struct CameraMatrix34
{
        float m[ 3 ][ 4 ];
};

struct CameraMatrix44
{
        float m[ 4 ][ 4 ];
};

struct CameraPosition
{
        float x, y, z;
        CameraPosition( float xValue, float yValue, float zValue )
            : x( xValue ), y( yValue ), z( zValue )
        {
        }
};

// These minimal interfaces describe only observed vtable slots. They do not
// assign an SDK identity or claim to recover the other virtual methods.
class CameraParentMatrixSource
{
public:
        virtual void unrecovered00() = 0;
        virtual void unrecovered04() = 0;
        virtual void unrecovered08() = 0;
        virtual void unrecovered0C() = 0;
        virtual void unrecovered10() = 0;
        virtual const CameraMatrix34* getWorldMatrix() const = 0;
};

class CameraViewUpdater
{
public:
        virtual void unrecovered00() = 0;
        virtual void unrecovered04() = 0;
        virtual void unrecovered08() = 0;
        virtual void update( CameraMatrix34* view, const CameraMatrix34* parent,
                             const CameraPosition* worldPosition ) = 0;
};

class CameraProjectionUpdater
{
public:
        virtual void unrecovered00() = 0;
        virtual void unrecovered04() = 0;
        virtual void unrecovered08() = 0;
        virtual void update( CameraMatrix44* projection, const CameraMatrix34* postProjection ) = 0;
};

struct CameraMatrixState
{
        void* mVtable;
        unsigned char mUnrecovered04[ 8 ];
        CameraParentMatrixSource* mParent;
        unsigned char mUnrecovered10[ 0x7C ];
        CameraMatrix34 mWorldMatrix;
        unsigned char mUnrecoveredBC[ 0x8C ];
        CameraMatrix34 mViewMatrix;
        CameraMatrix34 mInverseViewMatrix;
        CameraMatrix44 mProjectionMatrix;
        CameraMatrix44 mInverseProjectionMatrix;
        CameraMatrix34 mPostProjectionMatrix;
        CameraViewUpdater* mViewUpdater;
        unsigned char mUnrecovered25C[ 4 ];
        CameraProjectionUpdater* mProjectionUpdater;
};

typedef char ParentOffset[ offsetof( CameraMatrixState, mParent ) == 0x0C ? 1 : -1 ];
typedef char WorldOffset[ offsetof( CameraMatrixState, mWorldMatrix ) == 0x8C ? 1 : -1 ];
typedef char ViewOffset[ offsetof( CameraMatrixState, mViewMatrix ) == 0x148 ? 1 : -1 ];
typedef char InverseViewOffset[ offsetof( CameraMatrixState, mInverseViewMatrix ) == 0x178 ? 1 : -1 ];
typedef char ProjectionOffset[ offsetof( CameraMatrixState, mProjectionMatrix ) == 0x1A8 ? 1 : -1 ];
typedef char InverseProjectionOffset[ offsetof( CameraMatrixState, mInverseProjectionMatrix ) == 0x1E8 ? 1 : -1 ];
typedef char PostProjectionOffset[ offsetof( CameraMatrixState, mPostProjectionMatrix ) == 0x228 ? 1 : -1 ];
typedef char ViewUpdaterOffset[ offsetof( CameraMatrixState, mViewUpdater ) == 0x258 ? 1 : -1 ];
typedef char ProjectionUpdaterOffset[ offsetof( CameraMatrixState, mProjectionUpdater ) == 0x260 ? 1 : -1 ];

// Keep the target's expression grouping: its determinant sums all four
// positive groups before subtracting the four negative groups. Regrouping
// by cofactor would change floating-point rounding and generated code.
inline bool invertProjection( CameraMatrix44* result, const CameraMatrix44* matrix )
{
        const float a = matrix->m[ 0 ][ 0 ];
        const float b = matrix->m[ 0 ][ 1 ];
        const float c = matrix->m[ 0 ][ 2 ];
        const float d = matrix->m[ 0 ][ 3 ];
        const float e = matrix->m[ 1 ][ 0 ];
        const float f = matrix->m[ 1 ][ 1 ];
        const float g = matrix->m[ 1 ][ 2 ];
        const float h = matrix->m[ 1 ][ 3 ];
        const float i = matrix->m[ 2 ][ 0 ];
        const float j = matrix->m[ 2 ][ 1 ];
        const float k = matrix->m[ 2 ][ 2 ];
        const float l = matrix->m[ 2 ][ 3 ];
        const float m = matrix->m[ 3 ][ 0 ];
        const float n = matrix->m[ 3 ][ 1 ];
        const float o = matrix->m[ 3 ][ 2 ];
        const float p = matrix->m[ 3 ][ 3 ];

        const float determinant =
                a * ( f * k * p + g * l * n + h * j * o )
                + b * ( e * l * o + g * i * p + h * k * m )
                + c * ( e * j * p + f * l * m + h * i * n )
                + d * ( e * k * n + f * i * o + g * j * m )
                - a * ( f * l * o + g * j * p + h * k * n )
                - b * ( e * k * p + g * l * m + h * i * o )
                - c * ( e * l * n + f * i * p + h * j * m )
                - d * ( e * j * o + f * k * m + g * i * n );
        if ( determinant == 0.0f )
                return false;

        const float inverseDeterminant = 1.0f / determinant;
        const float kp_lo = k * p - l * o;
        const float jp_ln = j * p - l * n;
        const float kn_jo = k * n - j * o;
        const float gp_ho = g * p - h * o;
        const float hn_fp = h * n - f * p;
        const float fo_gn = f * o - g * n;
        const float hk_gl = h * k - g * l;
        const float fl_hj = f * l - h * j;
        const float gj_fk = g * j - f * k;
        const float ip_lm = i * p - l * m;
        const float km_io = k * m - i * o;
        const float ep_hm = e * p - h * m;
        const float gm_eo = g * m - e * o;
        const float hi_el = h * i - e * l;
        const float ek_gi = e * k - g * i;
        const float jm_in = j * m - i * n;
        const float en_fm = e * n - f * m;
        const float fi_ej = f * i - e * j;

        const float r00 = ( f * kp_lo - g * jp_ln - h * kn_jo ) * inverseDeterminant;
        const float r01 = ( c * jp_ln + d * kn_jo - b * kp_lo ) * inverseDeterminant;
        const float r02 = ( b * gp_ho + c * hn_fp + d * fo_gn ) * inverseDeterminant;
        const float r03 = ( b * hk_gl + c * fl_hj + d * gj_fk ) * inverseDeterminant;
        const float r10 = ( g * ip_lm + h * km_io - e * kp_lo ) * inverseDeterminant;
        const float r11 = ( a * kp_lo - c * ip_lm - d * km_io ) * inverseDeterminant;
        const float r12 = ( c * ep_hm + d * gm_eo - a * gp_ho ) * inverseDeterminant;
        const float r13 = ( c * hi_el + d * ek_gi - a * hk_gl ) * inverseDeterminant;
        const float r20 = ( e * jp_ln - f * ip_lm - h * jm_in ) * inverseDeterminant;
        const float r21 = ( b * ip_lm + d * jm_in - a * jp_ln ) * inverseDeterminant;
        const float r22 = ( d * en_fm - a * hn_fp - b * ep_hm ) * inverseDeterminant;
        const float r23 = ( d * fi_ej - a * fl_hj - b * hi_el ) * inverseDeterminant;
        const float r30 = ( e * kn_jo - f * km_io + g * jm_in ) * inverseDeterminant;
        const float r31 = ( b * km_io - c * jm_in - a * kn_jo ) * inverseDeterminant;
        const float r32 = ( -c * en_fm - a * fo_gn - b * gm_eo ) * inverseDeterminant;
        const float r33 = ( -c * fi_ej - a * gj_fk - b * ek_gi ) * inverseDeterminant;

        result->m[ 0 ][ 0 ] = r00;
        result->m[ 0 ][ 1 ] = r01;
        result->m[ 0 ][ 2 ] = r02;
        result->m[ 0 ][ 3 ] = r03;
        result->m[ 1 ][ 0 ] = r10;
        result->m[ 1 ][ 1 ] = r11;
        result->m[ 1 ][ 2 ] = r12;
        result->m[ 1 ][ 3 ] = r13;
        result->m[ 2 ][ 0 ] = r20;
        result->m[ 2 ][ 1 ] = r21;
        result->m[ 2 ][ 2 ] = r22;
        result->m[ 2 ][ 3 ] = r23;
        result->m[ 3 ][ 0 ] = r30;
        result->m[ 3 ][ 1 ] = r31;
        result->m[ 3 ][ 2 ] = r32;
        result->m[ 3 ][ 3 ] = r33;
        return true;
}
} // namespace

// Each address is an existing function boundary, independently identified
// from its body and other callers. No data-pool aliases are introduced.
extern "C" const CameraMatrix34* fn_00254890();
extern "C" const CameraMatrix44* fn_0022E130();
extern "C" int fn_00287284( CameraMatrix34* inverse, const CameraMatrix34* source );

#ifdef NON_MATCHING
extern "C" void fn_00261190( void* camera )
{
        CameraMatrixState* state = static_cast<CameraMatrixState*>( camera );
        CameraPosition position( state->mWorldMatrix.m[ 0 ][ 3 ],
                                 state->mWorldMatrix.m[ 1 ][ 3 ],
                                 state->mWorldMatrix.m[ 2 ][ 3 ] );
        CameraViewUpdater* viewUpdater = state->mViewUpdater;
        if ( state->mParent )
                viewUpdater->update( &state->mViewMatrix, state->mParent->getWorldMatrix(), &position );
        else
                viewUpdater->update( &state->mViewMatrix, fn_00254890(), &position );

        if ( !fn_00287284( &state->mInverseViewMatrix, &state->mViewMatrix ) )
                state->mInverseViewMatrix = *fn_00254890();

        state->mProjectionUpdater->update( &state->mProjectionMatrix, &state->mPostProjectionMatrix );
        if ( !invertProjection( &state->mInverseProjectionMatrix, &state->mProjectionMatrix ) )
                state->mInverseProjectionMatrix = *fn_0022E130();
}
#endif
