#ifndef SEAD_DATE_UTIL_SOURCE_H
#define SEAD_DATE_UTIL_SOURCE_H

namespace sead
{

class CalendarTime
{
public:
        struct Year { int mValue; };
        struct Month { int mValue; };
        struct Day { int mValue; };
        struct Hour { int mValue; };
        struct Minute { int mValue; };
        struct Second { int mValue; };
        class Time
        {
        public:
                Time( const Hour& hour, const Minute& minute, const Second& second );

        private:
                Hour mHour;
                Minute mMinute;
                Second mSecond;
        };
        class Date
        {
        public:
                Date( const Year& year, const Month& month, const Day& day );

        private:
                friend class CalendarTime;
                Year mYear;
                Month mMonth;
                Day mDay;
                unsigned char mWeekDay;
        };

        CalendarTime( const Year& year, const Month& month, const Day& day,
                      const Hour& hour, const Minute& minute, const Second& second );
        void setDate( const Date& date );

private:
        Date mDate;
        Time mTime;
};

class DateUtil
{
public:
        static bool isLeapYear( unsigned int year );
        static unsigned char calcWeekDay( const CalendarTime::Year& year,
                                          const CalendarTime::Month& month,
                                          const CalendarTime::Day& day );
};

} // namespace sead

#endif
