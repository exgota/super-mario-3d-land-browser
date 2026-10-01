#pragma once

#include <Layout/alLayoutActor.h>

// The actual source class name is unrecovered. Only slot 0x08 is called by
// the recovered function; the two reserved declarations retain its offset.
class WorldSelectionInterface
{
public:
        virtual void reservedVirtual0() const = 0;
        virtual void reservedVirtual4() const = 0;
        virtual int getWorldIndex() const = 0;
};

// A recovered layout representation for the CourseSelectMap resource user.
// The retail source class identity and constructor are not reconstructed here.
class CourseSelectMapLayout : public al::LayoutActor
{
public:
        float mMarkerCoordinates[8];           // 0x30
        void* mMarkerGroup;                    // 0x50
        int mUnrecovered54;                    // 0x54
        WorldSelectionInterface* mWorldSource; // 0x58
        bool mUnrecovered5C;                   // 0x5C
        bool mMarkerVisibility[8];            // 0x5D
};

static_assert( sizeof( CourseSelectMapLayout ) == 0x68, "Retail layout allocation size" );

extern "C" void fn_00159fec( CourseSelectMapLayout* layout );
