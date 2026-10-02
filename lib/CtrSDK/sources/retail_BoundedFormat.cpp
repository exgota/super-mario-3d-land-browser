// Retail integer/string formatting root, 00395114..00395D3C.
// Guarded NonMatching proposal; no byte-exact acceptance.
// Neutral identity: the original namespace and library ownership are unresolved.
// Clean-room reconstruction from the owner's EU executable only.
#ifdef NON_MATCHING
#include <stdarg.h>
#include <stddef.h>

// The original call at003955BC supplies a 64-bit dividend and divisor.
extern "C" unsigned long long fn_0028AB74(unsigned long long, unsigned long long);

namespace {
struct FormatOutput {
    char* start;
    char* cursor;
    unsigned remaining;

    FormatOutput(char* buffer, unsigned capacity)
        : start(buffer), cursor(buffer), remaining(capacity) {}

    void put(char value) {
        if (remaining) {
            *cursor = value;
            --remaining;
        }
        ++cursor;
    }
    void fill(char value, int count) {
        if (count > 0) {
            unsigned stored = remaining;
            if (stored > (unsigned)count) stored = count;
            for (unsigned i = 0; i < stored; ++i) cursor[i] = value;
            remaining -= stored;
            cursor += count;
        }
    }
    void copy(const char* source, int count) {
        if (count > 0) {
            unsigned stored = remaining;
            if (stored > (unsigned)count) stored = count;
            for (unsigned i = 0; i < stored; ++i) cursor[i] = source[i];
            remaining -= stored;
            cursor += count;
        }
    }
};
}

extern "C" int fn_00395114(char* buffer, unsigned capacity,
                             const char* format, va_list inputArguments) {
    va_list arguments = inputArguments;
    FormatOutput output(buffer, capacity);
    while (*format) {
        if (*format != '%') {
            output.put(*format++);
            continue;
        }
        int base = 10;
        unsigned flags = 0;
        int letterBias = 'a' - 10;
        int precision = -1;
        const char* begin = format;
        int width = 0;
        for (;;) {
            ++format;
            switch (*format) {
            case ' ': flags |= 1; continue;
            case '+': if (format[-1] == ' ') { flags |= 2; continue; } break;
            case '-': flags |= 8; continue;
            case '0': flags |= 16; continue;
            }
            break;
        }
        if (*format == '*') {
            width = va_arg(arguments, int);
            ++format;
            if (width < 0) { width = -width; flags |= 8; }
        } else {
            while ((unsigned)(*format - '0') <= 9) {
                width = width * 10 + *format - '0';
                ++format;
            }
        }
        if (*format == '.') {
            ++format;
            precision = 0;
            if (*format == '*') {
                precision = va_arg(arguments, int);
                ++format;
                if (precision < 0) precision = -1;
            } else {
                while ((unsigned)(*format - '0') <= 9) {
                    precision = precision * 10 + *format - '0';
                    ++format;
                }
            }
        }
        if (*format == 'h') {
            ++format;
            if (*format == 'h') { flags |= 0x100; ++format; }
            else flags |= 0x40;
        } else if (*format == 'l') {
            ++format;
            if (*format == 'l') { flags |= 0x80; ++format; }
            else flags |= 0x20;
        }
        switch (*format) {
        case 'X': letterBias = 'A' - 10;
        case 'x': base = 16; flags |= 0x1000; goto integer;
        case 'p': flags |= 4; precision = 8; base = 16; flags |= 0x1000; goto integer;
        case 'o': base = 8; flags |= 0x1000; goto integer;
        case 'u': flags |= 0x1000;
        case 'd': case 'i':
        integer: {
            if (flags & 8) flags &= ~16;
            if (precision >= 0) flags &= ~16;
            else precision = 1;
            unsigned long long value;
            int prefixCount = 0;
            char prefix[4];
            if (flags & 0x1000) {
                if (flags & 0x100) value = (unsigned char)va_arg(arguments, unsigned);
                else if (flags & 0x40) value = (unsigned short)va_arg(arguments, unsigned);
                else if (flags & 0x80) value = va_arg(arguments, unsigned long long);
                else value = va_arg(arguments, unsigned);
                flags &= ~3;
                if (flags & 4) {
                    if (base == 16 && value != 0) {
                        prefixCount = 2; prefix[0] = letterBias + 33; prefix[1] = '0';
                    } else if (base == 8) {
                        prefixCount = 1; prefix[0] = '0';
                    }
                }
            } else {
                long long signedValue;
                if (flags & 0x100) signedValue = (signed char)va_arg(arguments, int);
                else if (flags & 0x40) signedValue = (short)va_arg(arguments, int);
                else if (flags & 0x80) signedValue = va_arg(arguments, long long);
                else signedValue = va_arg(arguments, int);
                value = signedValue;
                if (signedValue < 0) {
                    value = 0 - value; prefixCount = 1; prefix[0] = '-';
                } else if (value != 0 || precision != 0) {
                    if (flags & 2) { prefixCount = 1; prefix[0] = '+'; }
                    else if (flags & 1) { prefixCount = 1; prefix[0] = ' '; }
                }
            }
            int digitCount = 0;
            char digits[24];
            switch (base) {
            case 8:
                while (value) { digits[digitCount++] = (value & 7) + '0'; value >>= 3; }
                break;
            case 10:
                if ((value >> 32) == 0) {
                    unsigned small = (unsigned)value;
                    while (small) { unsigned quotient = small / 10; digits[digitCount++] = small - quotient * 10 + '0'; small = quotient; }
                } else {
                    while (value) { unsigned long long quotient = fn_0028AB74(value, 10); digits[digitCount++] = value - quotient * 10 + '0'; value = quotient; }
                }
                break;
            case 16:
                while (value) {
                    int digit = value & 15;
                    digits[digitCount++] = digit < 10 ? digit + '0' : digit + letterBias;
                    value >>= 4;
                }
                break;
            }
            if (prefixCount > 0 && prefix[0] == '0') { digits[digitCount++] = '0'; prefixCount = 0; }
            int zeroCount = precision - digitCount;
            if (flags & 16) {
                int padding = width - digitCount - prefixCount;
                if (padding > zeroCount) zeroCount = padding;
            }
            if (zeroCount > 0) width -= zeroCount;
            int spaces = width - (prefixCount + digitCount);
            if (!(flags & 8)) output.fill(' ', spaces);
            while (prefixCount > 0) output.put(prefix[--prefixCount]);
            output.fill('0', zeroCount);
            while (digitCount > 0) output.put(digits[--digitCount]);
            if (flags & 8) output.fill(' ', spaces);
            ++format;
            break;
        }
        case 'c':
            if (precision < 0) {
                char value = va_arg(arguments, int);
                if (flags & 8) { output.put(value); output.fill(' ', width - 1); }
                else { output.fill(flags & 16 ? '0' : ' ', width - 1); output.put(value); }
                ++format;
                break;
            }
            goto invalid;
        case 's': {
            const char* value = va_arg(arguments, const char*);
            int length = 0;
            if (precision < 0) { while (value[length]) ++length; }
            else { while (length < precision && value[length]) ++length; }
            int spaces = width - length;
            if (flags & 8) { output.copy(value, length); output.fill(' ', spaces); }
            else { output.fill(flags & 16 ? '0' : ' ', spaces); output.copy(value, length); }
            ++format;
            break;
        }
        case 'n': {
            int length = output.cursor - output.start;
            // Retail hh suppresses the write and does not consume its argument.
            if (!(flags & 0x100)) {
                if (flags & 0x40) *va_arg(arguments, short*) = length;
                else if (flags & 0x80) *va_arg(arguments, long long*) = length;
                else *va_arg(arguments, int*) = length;
            }
            ++format;
            break;
        }
        case '%':
            if (begin + 1 == format) { output.put(*format++); break; }
        default:
        invalid:
            output.copy(begin, format - begin);
            break;
        }
    }
    if (output.remaining) *output.cursor = 0;
    else if (capacity) buffer[capacity - 1] = 0;
    return output.cursor - output.start;
}

#endif // NON_MATCHING
