#pragma once

#include <AreaObj/alAreaShape.h>

namespace al
{

class AreaShapeCube : public AreaShape
{
private:
        u8 mIsCubeBase;

public:
        virtual bool isInVolume( const sead::Vector3f& trans ) const;
        // virtual void v2();
public:
        AreaShapeCube( bool isCubeBase );
};

} // namespace al
