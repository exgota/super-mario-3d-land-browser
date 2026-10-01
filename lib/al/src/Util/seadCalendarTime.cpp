#include <Util/seadDateUtil.h>

namespace sead
{

CalendarTime::Date::Date( const Year& year, const Month& month, const Day& day )
        : mYear( year ), mMonth( month ), mDay( day ),
          mWeekDay( DateUtil::calcWeekDay( year, month, day ) )
{
}

} // namespace sead
