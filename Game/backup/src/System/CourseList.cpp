#include "System/CourseList.h"


#include <Resource/alResource.h>
#include <System/Application.h>
#include <Util/alStringUtil.h>
#include <Yaml/alByamlIter.h>

#include "System/GameSystem.h"
#include "System/RootTask.h"

extern "C" RootTask* fn_0028e678( const Application* application );

// Source-defined strings from the EU course list pool. Type, Normal and
// Miniature share an address anchor because the constructor derives Normal
// from Miniature - 8. Other strings retain their independently mapped rows.
static const char force_section( ".sdata_dat_003A2884" ) s_koopaCastle[12] = "KoopaCastle";
static const char force_section( ".sdata_dat_003A2890" ) s_type[8] = "Type";
static const char force_section( ".sdata_dat_003A2890" ) s_normal[8] = "Normal";
static const char force_section( ".sdata_dat_003A2890" ) s_miniature[12] = "Miniature";
static const char force_section( ".sdata_dat_003A28AC" ) s_koopaFortress[16] = "KoopaFortress";
static const char force_section( ".sdata_dat_003A28BC" ) s_koopaBattleShip[16] = "KoopaBattleShip";
static const char force_section( ".sdata_dat_003A28CC" ) s_championship[16] = "Championship";
static const char force_section( ".sdata_dat_003A28DC" ) s_kinopioHousePresent[20] = "KinopioHousePresent";
static const char force_section( ".sdata_dat_003A28F0" ) s_kinopioHouseAlbum[20] = "KinopioHouseAlbum";
static const char force_section( ".sdata_dat_003A2904" ) s_mysteryBox[12] = "MysteryBox";
static const char force_section( ".sdata_dat_003A2910" ) s_dokan[8] = "Dokan";
static const char force_section( ".sdata_dat_003A2918" ) s_empty[8] = "Empty";
static const char force_section( ".sdata_dat_003A2920" ) s_collectCoinNum[16] = "CollectCoinNum";
static const char force_section( ".sdata_dat_003A2930" ) s_scenario[12] = "Scenario";
static const char force_section( ".sdata_dat_003A293C" ) s_stage[8] = "Stage";
static const char force_section( ".sdata_dat_003A2944" ) s_worlds[8] = "Worlds";
struct CourseListResourceName
{
        char name[11];
        unsigned char padding;
};
static const CourseListResourceName force_section( ".sdata_dat_003A294C" ) s_courseList = {
        "CourseList", 0xFF
};
static_assert( sizeof( CourseListResourceName ) == 12, "CourseList name and alignment byte" );


CourseList::Course::Course( const al::ByamlIter* course )
    : mCourseType( CourseType_Normal ), mStageName( nullptr ), mScenario( -1 ),
      mMiniatureModelName( s_miniature ), mCoinCollectNum( 0 )

{
        const char* type = s_normal;
        course->tryGetStringByKey( &type, s_type );

        if ( al::isEqualString( type, s_koopaCastle ) )
                mCourseType = CourseType_KoopaCastle;
        else if ( al::isEqualString( type, s_koopaFortress ) )
                mCourseType = CourseType_KoopaFortress;
        else if ( al::isEqualString( type, s_koopaBattleShip ) )
                mCourseType = CourseType_KoopaBattleShip;
        else if ( al::isEqualString( type, s_championship ) )
                mCourseType = CourseType_Championship;
        else if ( al::isEqualString( type, s_kinopioHousePresent ) )
                mCourseType = CourseType_KinopioHousePresent;
        else if ( al::isEqualString( type, s_kinopioHouseAlbum ) )
                mCourseType = CourseType_KinopioHouseAlbum;
        else if ( al::isEqualString( type, s_mysteryBox ) )
                mCourseType = CourseType_MysteryBox;
        else if ( al::isEqualString( type, s_dokan ) )
                mCourseType = CourseType_Dokan;
        else if ( al::isEqualString( type, s_empty ) )
                mCourseType = CourseType_Empty;

        course->tryGetStringByKey( &mStageName, s_stage );
        course->tryGetIntByKey( &mScenario, s_scenario );
        course->tryGetStringByKey( &mMiniatureModelName, s_miniature );
        course->tryGetIntByKey( &mCoinCollectNum, s_collectCoinNum );
}

#ifdef NON_MATCHING
CourseList::World::World( const al::ByamlIter* world )
    : mCourses( nullptr ), mNumCourses( 0 ), mIsSpecialWorld( false )
{
        const char* type = "World";
        if ( world->tryGetStringByKey( &type, "Type" ) )
        {
                mIsSpecialWorld = al::isEqualString( type, "Special" );
                al::ByamlIter listIter;
                if ( world->tryGetIterByKey( &listIter, "Course" ) )
                {
                        mNumCourses = listIter.getSize();
                        if ( mNumCourses )
                        {
                                mCourses = new Course*[ mNumCourses ];
                                for ( int i = 0; i < mNumCourses; i++ )
                                {
                                        al::ByamlIter courseIter;
                                        if ( listIter.tryGetIterByIndex( &courseIter, i ) )
                                                mCourses[ i ] = new Course( &courseIter );
                                        else
                                                mCourses[ i ] = nullptr;
                                }
                        }
                }
        }
}


inline CourseList::List::List( const al::ByamlIter& courseListIter )
    : mWorlds( nullptr ), mNumWorlds( 0 )
{
        al::ByamlIter worlds;
        if ( !courseListIter.tryGetIterByKey( &worlds, s_worlds ) )
                return;

        mNumWorlds = worlds.getSize();
        if ( !mNumWorlds )
                return;

        mWorlds = new World*[ mNumWorlds ];
        for ( int i = 0; i < mNumWorlds; i++ )
        {
                al::ByamlIter curWorld;
                if ( worlds.tryGetIterByIndex( &curWorld, i ) )
                        mWorlds[ i ] = new World( &curWorld );
                else
                        mWorlds[ i ] = nullptr;
        }
}

#endif

CourseList::CourseList() : mCourseList( 0 )
{
        init( al::findOrCreateResource( "ObjectData/GameSystemDataTable" ) );
}

#ifdef NON_MATCHING
void CourseList::init( const al::Resource* gameSystemDataTable )
{
        mCourseList = new List( al::ByamlIter( gameSystemDataTable->getByml( s_courseList.name ) ) );
        mNumStages = 0;

        int i = 0;
        goto testWorld;

nextWorld:
        {
                World* world = mCourseList->mWorlds[ i ];
                int j = 0;
                if ( world->mNumCourses > 0 )
                {
                        do
                        {
                                Course* course = world->mCourses[ j ];
                                if ( Course::isCourseTypeStage( Course::CourseType( course->mCourseType ) ) )
                                        mNumStages++;
                                ++j;
                        } while ( world->mNumCourses > j );
                }
        }
        ++i;

testWorld:
        if ( mCourseList->mNumWorlds > i )
                goto nextWorld;
}
#endif

CourseList* rp::getCourseList()
{
        return fn_0028e678( al::getApplication() )->getGameSystem()->getCourseList();
}
