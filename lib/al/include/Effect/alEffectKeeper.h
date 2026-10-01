#pragma once

#include <math/seadVector.h>

namespace al
{

class EffectKeeper
{
private:
        // These methods establish the count, array and update flag offsets.
        // Other members and the complete object size remain unrecovered.
        unsigned char mOpaquePrefix[ 4 ];
        int           mEffectSetCount;
        void**        mEffectSets;
        const char*   mActionName;
        bool          mIsUpdateActive;
        signed char   mActionChangeMode;

public:
        void update();
        void deleteAndClearEffectAll();
        void deleteEffectAll();
        void setActionName( const char* actionName );
};

class IUseEffectKeeper
{
public:
        virtual EffectKeeper* getEffectKeeper() const = 0;
};

void emitEffect( IUseEffectKeeper* p, const char* name, const sead::Vector3f* at = nullptr );
bool tryEmitEffect( IUseEffectKeeper* p, const char* name );

} // namespace al
