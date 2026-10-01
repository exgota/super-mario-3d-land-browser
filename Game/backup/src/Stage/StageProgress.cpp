#include <Stage/StageProgressAccessors.h>

namespace StageProgressReconstruction
{
extern "C" {
GameDataHolder* fn_002913CC();
SaveDataRecord* fn_0025BB50(GameDataHolder*);
StageData* fn_0025BB60();
void fn_0025B680(StageData*);
void fn_0025B5DC(SaveDataRecord*,int);
void fn_0025B398(SaveDataRecord*,int);
CourseIdentifier* fn_0027BD90(CourseIdentifier*,int);
bool fn_00276534(const CourseIdentifier*);
CourseProgress* fn_0025B93C(SaveDataRecord*,int);
bool fn_0025B4F4(const CourseProgress*);
bool fn_0025B804(const StageData*,int);
void fn_00138F68(SaveDataRecord*,int,int,int);
bool fn_0025B1F4();
void fn_0016BCE0();
}
CourseProgress* getCourseProgress();
int getAcquisitionMode(const CourseProgress* progress);
void setClearFlag(SaveDataRecord* save, int value);
int getCoinCollectCount(int courseIndex);
void fn_0016BCE0()
{
    SaveDataRecord* save = fn_0025BB50(fn_002913CC());
    StageData* stage = fn_0025BB60();
    fn_0025B680(fn_0025BB60());
    fn_0025B5DC(save, stage->lifeCount);
    fn_0025B398(save, stage->character);
    setClearFlag(save, stage->clearFlag);
    CourseProgress* progress = getCourseProgress();
    int courseIndex = fn_0025BB60()->courseIndex;
    int count = getCoinCollectCount(courseIndex);
    for (int index = 0; index < count; ++index)
        if (fn_0025B804(stage, index))
            fn_00138F68(save, progress->getCourseIndex(), index, getAcquisitionMode(progress));
    fn_0025B1F4();
    stage->pendingState = 0;
}

CourseProgress* getCourseProgress()
{
    return fn_0025BB60()->getCourseProgress();
}
int getCoinCollectCount(int courseIndex)
{
    CourseIdentifier identifier;
    fn_0027BD90(&identifier, courseIndex);
    int count;
    if (!fn_00276534(&identifier))
        count = 3;
    else
        count = fn_0025B4F4(fn_0025B93C(fn_0025BB50(fn_002913CC()), courseIndex)) ? 2 : 1;
    return count;
}
int CourseProgress::getCourseIndex() const { return courseIndex; }
int CourseProgress::getAcquisitionMode() const { return acquisitionMode; }
void SaveDataRecord::setClearFlag(int value) { clearFlag = value; }

void setClearFlag(SaveDataRecord* save, int value) { save->setClearFlag(value); }

} // namespace StageProgressReconstruction
