#include "System/CourseList.h"

#ifdef NON_MATCHING
bool CourseList::Course::isCourseTypeStage( CourseType type )
{
        // The retail range test is unsigned: invalid negative values are not stages.
        if ( static_cast<unsigned int>( type ) > CourseType_Championship )
                return false;
        return true;
}
#endif
