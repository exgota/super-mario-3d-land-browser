namespace
{
// AreaObj's volume query independently tests the byte at +0x48.
// This prefix does not claim the complete object layout or size.
struct AreaObjectActivationFields
{
        unsigned char mOpaquePrefix[ 0x48 ];
        bool mIsActive;
};
}
extern "C" void fn_001EA49C( void* areaObject )
{
        static_cast<AreaObjectActivationFields*>( areaObject )->mIsActive = false;
}
extern "C" void fn_001EA4C8( void* areaObject )
{
        static_cast<AreaObjectActivationFields*>( areaObject )->mIsActive = true;
}
