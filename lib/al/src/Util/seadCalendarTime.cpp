#include <Util/seadDateUtil.h>

namespace nn
{
namespace fnd
{
struct DateTimeParameters
{
        int mYear;
        signed char mMonth;
        signed char mDay;
        signed char mWeekDay;
        signed char mHour;
        signed char mMinute;
        signed char mSecond;
        short mMillisecond;
};
}
}

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

template <class Value>
static inline Value makeCalendarValue( int value )
{
        Value result;
        result.mValue = value;
        return result;
}

CalendarTime::CalendarTime( const nn::fnd::DateTimeParameters& parameters )
        : mDate( makeCalendarValue<Year>( parameters.mYear ),
                 makeCalendarValue<Month>( parameters.mMonth ),
                 makeCalendarValue<Day>( parameters.mDay ) ),
          mTime( parameters.mHour, parameters.mMinute, parameters.mSecond )
{
}

} // namespace sead
