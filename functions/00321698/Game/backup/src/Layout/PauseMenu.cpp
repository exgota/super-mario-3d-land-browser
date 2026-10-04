#include <Layout/PauseMenu.h>
#include <Nerve/alNerveFunction.h>

#ifdef NON_MATCHING

struct PauseMenuInitialNerve : al::Nerve
{
        virtual void execute( al::NerveKeeper* keeper ) const;
};

extern "C"
{
void fn_0027e6c8( al::LayoutActor*, const char*, const char*, int );
int fn_00217584();
bool fn_002660DC();
bool fn_00217568();
bool fn_00217524();
void fn_0027E004( void*, int );
void fn_0027E73C( void* );
bool fn_00332A48( al::LayoutActor*, const char*, int );
extern const PauseMenuInitialNerve dat_003F1994;
extern const char dat_003B8168[8]; // InOut
extern const char dat_003B8170[8]; // Appear
}

inline void PauseMenu::updateMenuEntries()
{
        for ( int index = 0; index < 3; ++index ) {
                switch ( static_cast<unsigned char>( index ) ) {
                case 1:
                        if ( !mDisableEntry1 &&
                             ( fn_00217584() || fn_002660DC() || fn_00217568() ) &&
                             fn_00217524() )
                                break;
                        fn_0027E004( mMenuGroup, index );
                        break;
                case 2:
                        if ( !mDisableEntry2 &&
                             ( fn_00217584() || fn_002660DC() || fn_00217568() ) &&
                             fn_00217524() )
                                break;
                        fn_0027E004( mMenuGroup, index );
                        break;
                }
        }
}

void PauseMenu::exeAppear()
{
        if ( al::isFirstStep( this ) ) {
                fn_0027e6c8( this, dat_003B8168, dat_003B8170, 0 );
                updateMenuEntries();
        }
        if ( fn_00332A48( this, dat_003B8168, 0 ) ) {
                fn_0027E73C( mMenuGroup );
                updateMenuEntries();
                al::setNerve( this, &dat_003F1994 );
        }
}

#endif
