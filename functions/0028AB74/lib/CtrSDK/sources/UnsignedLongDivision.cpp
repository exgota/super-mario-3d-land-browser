// Clean-room unsigned reciprocal division from the owner's EU executable.
namespace observed_unsigned_division {
typedef unsigned Word;
typedef unsigned long long Wide;
struct Result { Word quotientLow, quotientHigh, remainderLow, remainderHigh; };

static int highestBit(Wide value) {
    Word upper = static_cast<Word>(value >> 32);
    return upper ? 63 - __clz(upper) : 31 - __clz(static_cast<Word>(value));
}

// The upper 64 bits of the 96-bit product, after discarding its low word.
static Wide upperProduct(Wide value, Word reciprocal) {
    Wide low = static_cast<Wide>(static_cast<Word>(value)) * reciprocal;
    return (low >> 32) + (value >> 32) * reciprocal;
}

static Word refineSmall(Word reciprocal, Word divisor, unsigned bit) {
    Wide error = -(static_cast<Wide>(reciprocal) * divisor << (32 - bit));
    return reciprocal + static_cast<Word>((static_cast<Wide>(reciprocal) *
        static_cast<Word>(error >> 32)) >> 32);
}

static Word refineLarge(Word reciprocal, Wide divisor, unsigned bit) {
    Wide error = -(upperProduct(divisor, reciprocal) << (64 - bit));
    return reciprocal + static_cast<Word>((static_cast<Wide>(reciprocal) *
        static_cast<Word>(error >> 32)) >> 32);
}
}

extern "C" unsigned long long __aeabi_ldiv0(unsigned long long);

extern "C" __value_in_regs observed_unsigned_division::Result
fn_0028AB74(unsigned long long numerator, unsigned long long denominator) {
    using namespace observed_unsigned_division;
    Wide quotient = 0;
    Wide remainder = numerator;
    if (!denominator) {
        quotient = __aeabi_ldiv0(0);
    } else {
        int divisorBit = highestBit(denominator);
        int difference = highestBit(numerator) - divisorBit;
        if (difference >= 0) {
            if (difference <= 4) {
                Wide shifted = denominator << difference;
                do {
                    quotient <<= 1;
                    if (remainder >= shifted) {
                        remainder -= shifted;
                        ++quotient;
                    }
                    shifted >>= 1;
                } while (difference-- != 0);
            } else {
                // Derived floor(2^36 / (17 + index)), not copied executable words.
                static const Word seeds[16] = {
                    (1ULL << 36) / 17, (1ULL << 36) / 18,
                    (1ULL << 36) / 19, (1ULL << 36) / 20,
                    (1ULL << 36) / 21, (1ULL << 36) / 22,
                    (1ULL << 36) / 23, (1ULL << 36) / 24,
                    (1ULL << 36) / 25, (1ULL << 36) / 26,
                    (1ULL << 36) / 27, (1ULL << 36) / 28,
                    (1ULL << 36) / 29, (1ULL << 36) / 30,
                    (1ULL << 36) / 31, (1ULL << 36) / 32
                };
                Word index = static_cast<Word>((denominator << (63 - divisorBit)) >> 59) & 15;
                Word reciprocal = seeds[index];
                if (divisorBit < 32) {
                    Word divisor = static_cast<Word>(denominator);
                    reciprocal = refineSmall(reciprocal, divisor, divisorBit);
                    reciprocal = refineSmall(reciprocal, divisor, divisorBit);
                    if (numerator >> 32)
                        reciprocal = refineSmall(reciprocal, divisor, divisorBit);
                    quotient = upperProduct(numerator, reciprocal) >> divisorBit;
                    remainder -= quotient * divisor;
                    if (remainder >= divisor) {
                        Wide correction = upperProduct(remainder, reciprocal) >> divisorBit;
                        quotient += correction;
                        remainder -= correction * divisor;
                        if (remainder >= divisor) {
                            remainder -= divisor;
                            ++quotient;
                            if (remainder >= divisor) {
                                remainder -= divisor;
                                ++quotient;
                            }
                            if (remainder >= divisor) {
                                remainder -= divisor;
                                ++quotient;
                            }
                        }
                    }
                } else {
                    reciprocal = refineLarge(reciprocal, denominator, divisorBit);
                    reciprocal = refineLarge(reciprocal, denominator, divisorBit) - 1;
                    quotient = upperProduct(numerator, reciprocal) >> divisorBit;
                    remainder -= quotient * denominator;
                    if (remainder >= denominator) {
                        Wide correction = upperProduct(remainder, reciprocal) >> divisorBit;
                        quotient += correction;
                        remainder -= correction * denominator;
                        if (remainder >= denominator) {
                            remainder -= denominator;
                            ++quotient;
                        }
                    }
                }
            }
        }
    }
    Result result = {static_cast<Word>(quotient), static_cast<Word>(quotient >> 32),
        static_cast<Word>(remainder), static_cast<Word>(remainder >> 32)};
    return result;
}
