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

void CalendarTime::setDate( const Date& date )
{
        mDate = date;
        mDate.mWeekDay = DateUtil::calcWeekDay( mDate.mYear, mDate.mMonth, mDate.mDay );
}

CalendarTime::CalendarTime( const Year& year, const Month& month, const Day& day,
                            const Hour& hour, const Minute& minute, const Second& second )
        : mDate( year, month, day ), mTime( hour, minute, second )
{
}

} // namespace sead
