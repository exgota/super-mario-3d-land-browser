#include <Util/seadDateUtil.h>

namespace sead
{

CalendarTime::Date::Date( const Year& year, const Month& month, const Day& day )
        : mYear( year ), mMonth( month ), mDay( day ),
          mWeekDay( DateUtil::calcWeekDay( year, month, day ) )
{
}

CalendarTime::Time::Time( const Hour& hour, const Minute& minute, const Second& second )
        : mHour( hour ), mMinute( minute ), mSecond( second )
{
}

} // namespace sead
