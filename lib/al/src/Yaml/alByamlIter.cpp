#include <Yaml/alByamlContainerHeader.h>
#include <Yaml/alByamlData.h>
#include <Yaml/alByamlHashIter.h>
#include <Yaml/alByamlHeader.h>
#include <Yaml/alByamlIter.h>
#include <Yaml/alByamlStringTableIter.h>

namespace al
{

ByamlIter::ByamlIter( const u8* data, const u8* rootNode ) : mData( data ), mRootNode( rootNode )
{
}

ByamlIter::ByamlIter() : mData( nullptr ), mRootNode( nullptr )
{
}

ByamlIter::ByamlIter( const ByamlIter& other ) : mData( other.mData ), mRootNode( other.mRootNode )
{
}

ByamlIter::ByamlIter( const u8* data ) : mData( data ), mRootNode( nullptr )
{
        if ( mData )
        {
                if ( !alByamlLocalUtil::verifiByaml( data ) )
                {
                        mData     = nullptr;
                        mRootNode = nullptr;
                        return;
                }
                int offset = mHeader->getDataOffset();
                if ( offset != 0 )
                        mRootNode = mData + this->mHeader->getDataOffset();
        }
}

bool ByamlIter::isValid() const
{
        return mData != nullptr;
}

int ByamlIter::getSize() const
{
        if ( isTypeContainer() )
                return mContainerHeader->getCount();
        return 0;
}

bool ByamlIter::isTypeArray() const
{
        return mContainerHeader != nullptr && mContainerHeader->getType() == ByamlDataType_Array;
}

bool ByamlIter::isTypeHash() const
{
        return mContainerHeader != nullptr && mContainerHeader->getType() == ByamlDataType_Hash;
}

bool ByamlIter::isTypeContainer() const
{
        return mContainerHeader != nullptr && ( mContainerHeader->getType() == ByamlDataType_Array ||
                                                      mContainerHeader->getType() == ByamlDataType_Hash );
}

int ByamlIter::getKeyIndex( const char* key ) const
{
        ByamlStringTableIter iter( mData + mHeader->getHashKeyTableOffset() );
        return iter.findStringIndex( key );
}

bool ByamlIter::isExistKey( const char* key ) const
{
        if ( !isTypeHash() )
                return false;
        int keyIndex = getKeyIndex( key );
        if ( keyIndex < 0 )
                return false;
        ByamlHashIter iter( mRootNode );
        return iter.findPair( keyIndex ) != nullptr;
}

ByamlIter ByamlIter::getIterByKey( const char* key ) const
{
        ByamlData data;
        if ( getByamlDataByKey( &data, key ) )
        {
                if ( data.getType() == ByamlDataType_Array || data.getType() == ByamlDataType_Hash )
                        return ByamlIter( mData, mData + data.getIntValue() );
                if ( data.getType() == ByamlDataType_Null )
                        return ByamlIter( mData, nullptr );
        }
        return ByamlIter();
}

ByamlIter ByamlIter::getIterByIndex( int index ) const
{
        ByamlData data;
        if ( getByamlDataByIndex( &data, index ) )
        {
                if ( data.getType() == ByamlDataType_Array || data.getType() == ByamlDataType_Hash )
                        return ByamlIter( mData, mData + data.getIntValue() );
                if ( data.getType() == ByamlDataType_Null )
                        return ByamlIter( mData, nullptr );
        }
        return ByamlIter();
}

#pragma no_inline

bool ByamlIter::getByamlDataByKey( ByamlData* out, const char* key ) const
{
        if ( !isTypeHash() )
                return false;
        int keyIndex = getKeyIndex( key );
        if ( keyIndex < 0 )
                return false;
        ByamlHashIter iter( mRootNode );
        return iter.getDataByKey( out, keyIndex );
}

bool ByamlIter::tryGetIterByKey( ByamlIter* out, const char* key ) const
{
        *out = getIterByKey( key );
        return out->isValid();
}

bool ByamlIter::tryGetIterByIndex( ByamlIter* out, int index ) const
{
        *out = getIterByIndex( index );
        return out->isValid();
}

bool ByamlIter::tryGetStringByIndex( const char** out, int index ) const
{
        ByamlData data;
        if ( getByamlDataByIndex( &data, index ) )
                return tryConvertString( out, &data );
        return false;
}

bool ByamlIter::tryGetStringByKey( const char** out, const char* key ) const
{
        ByamlData data;
        if ( getByamlDataByKey( &data, key ) )
                return tryConvertString( out, &data );
        return false;
}

bool ByamlIter::tryGetBoolByKey( bool* out, const char* key ) const
{
        ByamlData data;

        if ( getByamlDataByKey( &data, key ) )
                return tryConvertBool( out, &data );
        return false;
}

bool ByamlIter::tryGetIntByKey( int* out, const char* key ) const
{
        ByamlData data;

        if ( getByamlDataByKey( &data, key ) )
                return tryConvertInt( out, &data );
        return false;
}

bool ByamlIter::tryGetFloatByKey( float* out, const char* key ) const
{
        ByamlData data;

        if ( getByamlDataByKey( &data, key ) )
                return tryConvertFloat( out, &data );
        return false;
}

#pragma inline

bool ByamlIter::tryConvertString( const char** out, const ByamlData* data ) const
{
        if ( data->getType() == ByamlDataType_String )
        {
                ByamlStringTableIter table( mData + mHeader->getStringTableOffset() );
                *out = table.getString( data->getIntValue() );
                return true;
        }
        return false;
}

bool ByamlIter::tryConvertBool( bool* out, const ByamlData* data ) const
{
        if ( data->getType() == ByamlDataType_Int || data->getType() == ByamlDataType_Bool )
        {
                *out = data->getIntValue();
                return true;
        }
        return false;
}

#pragma inline

bool ByamlIter::tryConvertInt( int* out, const ByamlData* data ) const
{
        if ( data->getType() == ByamlDataType_Int )
        {
                *out = data->getIntValue();
                return true;
        }
        return false;
}

#pragma inline

bool ByamlIter::tryConvertFloat( float* out, const ByamlData* data ) const
{
        if ( data->getType() == ByamlDataType_Float )
        {
                *out = data->getFloatValue();
                return true;
        }
        return false;
}

} // namespace al
