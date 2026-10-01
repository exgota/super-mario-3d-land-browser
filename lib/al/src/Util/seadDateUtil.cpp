#include <Util/seadDateUtil.h>

namespace sead
{

bool DateUtil::isLeapYear( unsigned int year )
{
        return ( year % 4 == 0 && year % 100 != 0 ) || year % 400 == 0;
}

unsigned char DateUtil::calcWeekDay( const CalendarTime::Year& year,
                                    const CalendarTime::Month& month,
                                    const CalendarTime::Day& day )
{
        int adjustedYear = year.mValue;
        int adjustedMonth = month.mValue;
        int dayValue = day.mValue;
        if ( adjustedMonth < 3 )
        {
                --adjustedYear;
                adjustedMonth += 12;
        }
        return ( adjustedYear + adjustedYear / 4 - adjustedYear / 100 +
                 adjustedYear / 400 + ( 26 * adjustedMonth + 16 ) / 10 +
                 dayValue ) % 7;
}

} // namespace sead
