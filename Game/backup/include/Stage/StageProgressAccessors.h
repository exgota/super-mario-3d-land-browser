#pragma once

// Partial retail field views. These names do not assert original source identities.
namespace StageProgressReconstruction
{
struct SaveDataFile;
struct GameDataHolder;
struct CourseProgress
{
    signed char padding[0x60];
    signed char courseIndex;
    signed char reserved[3];
    signed char acquisitionMode;
    int getCourseIndex() const;
    int getAcquisitionMode() const;
};
struct StageData
{
    int lifeCount;
    int character;
    int clearFlag;
    char reserved[0x14];
    int pendingState;
    int courseIndex;
    char reservedAfterCourse[8];
    CourseProgress* progress;
    CourseProgress* getCourseProgress() const;
};
struct SaveDataRecord
{
    char reserved[8];
    signed char clearFlag;
    void setClearFlag(int value);
};
struct CourseIdentifier
{
    int world;
    int course;
};

} // namespace StageProgressReconstruction
