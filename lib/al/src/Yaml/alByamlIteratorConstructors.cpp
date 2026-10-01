#include <Yaml/alByamlHashIter.h>
#include <Yaml/alByamlStringTableIter.h>

namespace al
{

#pragma no_inline

ByamlHashIter::ByamlHashIter( const u8* data ) : mData( data )
{
}

ByamlStringTableIter::ByamlStringTableIter( const u8* data ) : mData( data )
{
}

} // namespace al
