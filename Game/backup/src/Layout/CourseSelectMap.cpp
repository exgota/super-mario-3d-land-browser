#include <Layout/CourseSelectMap.h>
#include <Util/alStringUtil.h>

extern "C"
{
int fn_0026afbc( WorldSelectionInterface*, int );
int fn_0026561c( WorldSelectionInterface*, int, int );
void fn_0027b91c( void*, int );
void fn_0027e6c8( al::LayoutActor*, const char*, const char*, int );
void fn_00260860( int*, int );
bool fn_003270f0( int );
bool fn_002607f4( int );
void* fn_0027be70( void*, int );
void fn_0027be2c( void*, const char* );
bool fn_00265524( int );
void fn_00170eac( void*, int, bool );
void fn_002607e4( void* );
void fn_00265794( al::LayoutActor*, const char* );
void fn_00273458( al::LayoutActor*, const char* );
bool fn_00260780( int );
extern const char* const dat_003f1850[8];
extern const char* const dat_003f1870[16];
}

extern "C" void fn_00159fec( CourseSelectMapLayout* layout )
{
        int worldIndex = layout->mWorldSource->getWorldIndex();
        int courseCount = fn_0026afbc( layout->mWorldSource, worldIndex );
        int visibleCount;
        if ( courseCount < 8 ) {
                visibleCount = courseCount;
                for ( int index = visibleCount; index < 8; ++index ) {
                        fn_0027b91c( layout->mMarkerGroup, index );
                        fn_0027e6c8( layout, dat_003f1850[index], "Hide", 0 );
                }
        } else {
                visibleCount = 8;
        }

        int firstCourse = fn_0026561c( layout->mWorldSource, worldIndex, 0 );
        for ( int index = 0; index < visibleCount; ++index ) {
                int courseIndex = index + firstCourse;
                bool hasMedal = false;
                int markerKind;
                int imageIndex;
                fn_00260860( &markerKind, courseIndex );
                switch ( markerKind ) {
                case 0:
                        hasMedal = fn_003270f0( courseIndex );
                        imageIndex = 0;
                        break;
                case 1:
                        imageIndex = 6;
                        break;
                case 2:
                        imageIndex = 2;
                        break;
                case 3:
                        imageIndex = 14;
                        break;
                case 4:
                        imageIndex = 4;
                        break;
                case 5:
                        imageIndex = 8;
                        break;
                case 6:
                        imageIndex = 12;
                        break;
                case 7:
                        imageIndex = 10;
                        break;
                default:
                        continue;
                }

                if ( !fn_002607f4( courseIndex ) )
                        ++imageIndex;
                fn_0027be2c( fn_0027be70( layout->mMarkerGroup, index ), dat_003f1870[imageIndex] );

                bool isMarkerVisible = fn_00265524( courseIndex );
                if ( isMarkerVisible ) {
                        fn_00170eac( layout->mMarkerGroup, index, true );
                        fn_0027e6c8( layout, dat_003f1850[index], "Show", 0 );
                } else {
                        fn_0027b91c( layout->mMarkerGroup, index );
                        fn_0027e6c8( layout, dat_003f1850[index], "Hide", 0 );
                }

                if ( imageIndex == 10 || imageIndex == 11 ) {
                        isMarkerVisible = false;
                        fn_002607e4( fn_0027be70( layout->mMarkerGroup, index ) );
                }
                if ( hasMedal ) {
                        fn_00265794( layout, al::StringTmp<32>( "PicMedal%d", index + 1 ).cstr() );
                } else {
                        fn_00273458( layout, al::StringTmp<32>( "PicMedal%d", index + 1 ).cstr() );
                }
                layout->mMarkerVisibility[index] = isMarkerVisible;
        }

        if ( fn_00260780( courseCount + firstCourse - 1 ) )
                fn_0027e6c8( layout, "WorldLine", "WorldLine", 0 );
        else
                fn_0027e6c8( layout, "WorldLine", "Wait", 0 );
}
