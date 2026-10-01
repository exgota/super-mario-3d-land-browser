#include <Stage/StageProgressAccessors.h>

namespace StageProgressReconstruction
{

CourseProgress* StageData::getCourseProgress() const
{
    return progress;
}

int getAcquisitionMode(const CourseProgress* progress)
{
    return progress->acquisitionMode;
}

} // namespace StageProgressReconstruction
