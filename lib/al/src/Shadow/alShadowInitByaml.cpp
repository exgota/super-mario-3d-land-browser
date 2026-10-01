#include <Shadow/alShadowInitByaml.h>
#include <LiveActor/alActorInitInfo.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alLiveActor.h>
#include <Util/alStringUtil.h>
#include <Yaml/alByamlIter.h>

#ifdef NON_MATCHING

// NonMatching. The names below describe observed ABI views only. Unknown
// shadow methods and data remain borrowed original interfaces, not replacements.
namespace ShadowInitByaml {
struct Shadow {
        void* const* table;
        u8 unknown04[0x6c];
        const char* name;
        const char* vanishingPointName;
        Shadow* vanishingPoint;
};
struct Keeper {
        Shadow** values;
        int capacity;
        int head;
        int count;
        bool hidden;
        Shadow* at( int index ) const {
                if ( static_cast<unsigned>( count ) <= static_cast<unsigned>( index ) )
                        return *values;
                int slot = head + index;
                if ( capacity <= slot ) slot -= capacity;
                return values[slot];
        }
        void append( Shadow* shadow ) {
                if ( count >= capacity ) return;
                int slot = count++ + head;
                if ( slot >= capacity ) slot -= capacity;
                values[slot] = shadow;
        }
};
struct String {
        void* const* table;
        const char* text;
        const char* cstr() const {
                typedef void (*Terminate)( const String* );
                reinterpret_cast<Terminate>( table[2] )( this );
                return text;
        }
        int length() const {
                const char* value = cstr();
                for ( int i = 0; i < 0x10000; ++i )
                        if ( !value[i] ) return i;
                return 0;
        }
};
extern "C" void* nnnstdMemCpy( void*, const void*, unsigned );
struct Buffer128 : String {
        int capacity;
        char storage[128];
        void init() {
                table = reinterpret_cast<void* const*>( 0x003DA510 );
                text = storage;
                capacity = 128;
                storage[127] = 0;
                table = reinterpret_cast<void* const*>( 0x003DA224 );
                const String& empty = *reinterpret_cast<const String*>( 0x003F361C );
                if ( this != &empty ) {
                        storage[0] = 0;
                        char* destination = const_cast<char*>( text );
                        const int oldLength = length();
                        int amount = empty.length();
                        if ( capacity <= amount ) amount = capacity - 1;
                        if ( amount > 0 ) {
                                nnnstdMemCpy( destination, empty.cstr(), amount );
                                if ( amount > oldLength ) destination[amount] = 0;
                        }
                }
                table = reinterpret_cast<void* const*>( 0x003D9CBC );
        }
};
struct Parameters {
        int type;
        sead::Vector3f offset;
        sead::Vector3f rotateOffset;
        sead::Vector3f size;
        float shadowOffset;
        const char* name;
        const char* jointName;
        const char* vanishingPointName;
        const char* category;
        const char* modelArchive;
        Buffer128 strings[5];
        bool unknown2FC;
        bool useActorTransRef;
        void init() {
                type = 0;
                offset.x = offset.y = offset.z = 0.0f;
                rotateOffset.x = rotateOffset.y = rotateOffset.z = 0.0f;
                size.x = size.y = size.z = 0.0f;
                shadowOffset = 0.0f;
                name = jointName = vanishingPointName = modelArchive = 0;
                category = "\x89" "e\x83{\x83\x8a\x83\x85\x81[\x83\x80";
                strings[0].init();
                strings[1].init();
                strings[2].init();
                strings[3].init();
                strings[4].init();
                unknown2FC = useActorTransRef = false;
        }
};
static_assert( sizeof(Keeper) == 0x14, "keeper ABI" );
static_assert( sizeof(Buffer128) == 0x8c, "string ABI" );
static_assert( sizeof(Parameters) == 0x300, "parameter ABI" );
}

namespace sead { class Heap; }
void* operator new[]( unsigned, sead::Heap*, int );

// These are observed original destinations. Scale/translation/matrix pointers
// remain borrowed from the actor; creators copy the local vectors immediately.
extern "C" {
sead::Vector3f* fn_0024F1CC( al::LiveActor* );
const sead::Matrix34f* fn_002519A8( const al::LiveActor*, const char* );
#define SHADOW_CREATOR(NAME) ShadowInitByaml::Shadow* NAME( const char*, \
        const al::ActorInitInfo&, const sead::Vector3f&, const sead::Matrix34f*, \
        const sead::Vector3f*, const sead::Vector3f*, const sead::Vector3f&, \
        const sead::Vector3f&, float )
SHADOW_CREATOR( fn_001E5BE4 );
SHADOW_CREATOR( fn_001E7804 );
SHADOW_CREATOR( fn_001E710C );
SHADOW_CREATOR( fn_001E7BCC );
SHADOW_CREATOR( fn_001E6254 );
#undef SHADOW_CREATOR
ShadowInitByaml::Shadow* fn_001E5C94( const char*, const al::ActorInitInfo&,
        const sead::Matrix34f*, const sead::Vector3f*, const sead::Vector3f*,
        const sead::Vector3f&, const sead::Vector3f&, float, float, float, float );
ShadowInitByaml::Shadow* fn_001E6304( const char*, const al::ActorInitInfo&,
        const char*, const sead::Vector3f&, const sead::Matrix34f*,
        const sead::Vector3f*, const sead::Vector3f*, const sead::Vector3f&,
        const sead::Vector3f&, float );
}

extern "C" bool fn_001C1064( al::ShadowKeeper* receiver, al::LiveActor* actor,
        const al::ActorInitInfo& info, const al::ByamlIter& input )
{
        using namespace al;
        using namespace ShadowInitByaml;
        Keeper* keeper = reinterpret_cast<Keeper*>( receiver );
        if ( input.isExistKey( "DontUseShadow" ) ) return true;
        ByamlIter shadows;
        input.tryGetIterByKey( &shadows, "Shadows" );
        if ( !shadows.isValid() ) {
                Shadow** values = new ( static_cast<sead::Heap*>(0), 4 ) Shadow*[4];
                if ( values ) {
                        keeper->values = values;
                        keeper->count = 0;
                        keeper->capacity = 4;
                        keeper->head = 0;
                }
                return false;
        }
        int count = shadows.getSize();
        if ( count > 0 ) {
                Shadow** values = new ( static_cast<sead::Heap*>(0), 4 ) Shadow*[count];
                if ( values ) {
                        keeper->values = values;
                        keeper->count = 0;
                        keeper->capacity = count;
                        keeper->head = 0;
                }
        }
        for ( int i = 0; i < count; ++i ) {
                ByamlIter entry;
                shadows.tryGetIterByIndex( &entry, i );
                Parameters p;
                p.init();
                entry.tryGetStringByKey( &p.category, "ExecCategory" );
                entry.tryGetStringByKey( &p.name, "Name" );
                entry.tryGetStringByKey( &p.jointName, "ActorJointName" );
                entry.tryGetStringByKey( &p.vanishingPointName, "VanishingPointShadowName" );
                entry.tryGetStringByKey( &p.modelArchive, "ModelArcName" );
                p.useActorTransRef = entry.isExistKey( "UseActorTransRef" );
                const char* typeName = "NULL";
                if ( entry.tryGetStringByKey( &typeName, "TypeName" ) ) {
                        const char* const* names = reinterpret_cast<const char* const*>( 0x003EF648 );
                        int type = 0;
                        for ( ; type < 10; ++type )
                                if ( isEqualString( typeName, names[type] ) ) break;
                        p.type = type < 10 ? type : 0;
                } else {
                        int type = 0;
                        entry.tryGetIntByKey( &type, "Type" );
                        p.type = type;
                }
                ByamlIter offset, rotateOffset, size;
                if ( entry.tryGetIterByKey( &offset, "Offset" ) ) {
                        offset.tryGetFloatByKey( &p.offset.x, "X" );
                        offset.tryGetFloatByKey( &p.offset.y, "Y" );
                        offset.tryGetFloatByKey( &p.offset.z, "Z" );
                }
                if ( entry.tryGetIterByKey( &rotateOffset, "RotateOffset" ) ) {
                        rotateOffset.tryGetFloatByKey( &p.rotateOffset.x, "X" );
                        rotateOffset.tryGetFloatByKey( &p.rotateOffset.y, "Y" );
                        rotateOffset.tryGetFloatByKey( &p.rotateOffset.z, "Z" );
                }
                if ( entry.tryGetIterByKey( &size, "Size" ) ) {
                        size.tryGetFloatByKey( &p.size.x, "X" );
                        size.tryGetFloatByKey( &p.size.y, "Y" );
                        size.tryGetFloatByKey( &p.size.z, "Z" );
                }
                entry.tryGetFloatByKey( &p.shadowOffset, "ShadowOffset" );
                const sead::Matrix34f* matrix = 0;
                const sead::Vector3f* trans = 0;
                const sead::Vector3f* scale = fn_0024F1CC( actor );
                if ( !p.useActorTransRef ) {
                        if ( p.jointName ) matrix = fn_002519A8( actor, p.jointName );
                        else if ( actor->getBaseMtx() ) matrix = actor->getBaseMtx();
                        else trans = getTransPtr( actor );
                } else trans = getTransPtr( actor );
                Shadow* shadow = 0;
                if ( p.type == 1 || p.type == 2 ) {}
                else if ( p.type == 3 ) shadow = fn_001E5BE4( p.name, info, p.size, matrix, trans, scale, p.offset, p.rotateOffset, p.shadowOffset );
                else if ( p.type == 4 ) shadow = fn_001E7804( p.name, info, p.size, matrix, trans, scale, p.offset, p.rotateOffset, p.shadowOffset );
                else if ( p.type == 5 ) shadow = fn_001E710C( p.name, info, p.size, matrix, trans, scale, p.offset, p.rotateOffset, p.shadowOffset );
                else if ( p.type == 6 ) shadow = fn_001E7BCC( p.name, info, p.size, matrix, trans, scale, p.offset, p.rotateOffset, p.shadowOffset );
                else if ( p.type == 7 ) shadow = fn_001E5C94( p.name, info, matrix, trans, scale, p.offset, p.rotateOffset, p.size.x, p.size.z, p.size.y, p.shadowOffset );
                else if ( p.type == 8 ) shadow = fn_001E6254( p.name, info, p.size, matrix, trans, scale, p.offset, p.rotateOffset, p.shadowOffset );
                else if ( p.type == 9 ) shadow = fn_001E6304( p.name, info, p.modelArchive, p.size, matrix, trans, scale, p.offset, p.rotateOffset, p.shadowOffset );
                if ( shadow ) {
                        shadow->vanishingPointName = p.vanishingPointName;
                        typedef void (*Initialize)( Shadow*, const ActorInitInfo&, const char* );
                        reinterpret_cast<Initialize>( shadow->table[0x60 / 4] )( shadow, info, p.category );
                        keeper->append( shadow );
                }
        }
        const int total = keeper->count;
        for ( int i = 0; i < total; ++i ) {
                Shadow* shadow = keeper->at( i );
                const char* target = shadow->vanishingPointName;
                if ( !target ) continue;
                const int end = keeper->count;
                Shadow* found = 0;
                for ( int j = 0; j < end; ++j ) {
                        if ( isEqualString( keeper->at(j)->name, target ) ) {
                                found = keeper->at(j);
                                break;
                        }
                }
                shadow->vanishingPoint = found;
        }
        return true;
}
#endif
