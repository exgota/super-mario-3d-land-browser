#include <File/alFileFunction.h>
#include <File/alFileLoader.h>
#include <Message/alMessageFunction.h>
#include <System/alSystemKit.h>
#include <Util/alStringUtil.h>

extern "C" s32 fn_0028E1E4( sead::BufferedSafeString* receiver, const char* format, ... );

namespace al
{

const char* getLanguageString();

void loadArchive( const sead::SafeString& archive )
{
        alProjectInterface::getSystemKit()->getFileLoader()->loadArchive(
                StringTmp<256>( "%s.szs", archive.cstr() ), nullptr );
}

void makeLocalizedArchivePath( sead::BufferedSafeString* out, const sead::SafeString& archive )
{
        ::fn_0028E1E4( out, "LocalizedData/%s/%s", al::getLanguageString(), archive.cstr() );
}

void makeStageDataArchivePath( sead::BufferedSafeString* out, const char* stageName, int scenario, const char* type )
{
        ::fn_0028E1E4( out, "StageData/%s%s%d", stageName, type, scenario );
}

} // namespace al
