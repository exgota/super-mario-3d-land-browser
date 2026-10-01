namespace sead
{

class Random
{
public:
        unsigned int getU32();
        void init( unsigned int seed );

private:
        unsigned int mX;
        unsigned int mY;
        unsigned int mZ;
        unsigned int mW;
};

unsigned int Random::getU32()
{
        unsigned int temporary = mX ^ ( mX << 11 );
        mX = mY;
        mY = mZ;
        mZ = mW;
        mW = ( mW ^ ( mW >> 19 ) ) ^ ( temporary ^ ( temporary >> 8 ) );
        return mW;
}

void Random::init( unsigned int seed )
{
        mX = 1812433253u * ( seed ^ ( seed >> 30 ) ) + 1;
        mY = 1812433253u * ( mX ^ ( mX >> 30 ) ) + 2;
        mZ = 1812433253u * ( mY ^ ( mY >> 30 ) ) + 3;
        mW = 1812433253u * ( mZ ^ ( mZ >> 30 ) ) + 4;
}

} // namespace sead
