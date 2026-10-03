#pragma once

class JMapInfo;

namespace al
{

struct KCollisionHeader
{
        union
        {
                u32   verticesOffset;
                void* verticesSection;
        };

        union
        {
                u32   normalsOffset;
                void* normalsSection;
        };

        union
        {
                u32   trianglesOffset;
                void* trianglesSection;
        };

        union
        {
                u32   spatialIndicesOffset;
                void* spatialIndicesSection;
        };

        // Observed query prefix beyond the existing relocated section pointers.
        u8 unknown10[4];
        float origin[3];
        u32 coordinateMasks[3];
        s32 subdivisionShifts[3];

        static KCollisionHeader* fromData( void* data )
        {
                return static_cast<KCollisionHeader*>( data );
        }
};

class KCollisionServer
{
private:
        union
        {
                KCollisionHeader* mHeader;
                void*             mHeaderData;
                u8*               mHeaderDataBytes;
        };

        JMapInfo* mAttributeInfo;
        float     _8;

public:
        // Ordinary access to the existing owner; no duplicate receiver layout.
        KCollisionHeader* getObservedHeader() const { return mHeader; }
        void setData( void* data );
        void initKCollisionServer( void* kclData, const void* paData );

public:
        KCollisionServer();
};

} // namespace al
