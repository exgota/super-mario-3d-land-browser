namespace sead
{

class DateUtil
{
public:
        static bool isLeapYear( unsigned int year );
};

bool DateUtil::isLeapYear( unsigned int year )
{
        return ( year % 4 == 0 && year % 100 != 0 ) || year % 400 == 0;
}

} // namespace sead
