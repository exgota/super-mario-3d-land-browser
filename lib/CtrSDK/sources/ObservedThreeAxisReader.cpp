#include <clean/ObservedThreeAxisReader.h>
extern "C" {
void fn_00297B48(void*, observed_hid::Sample*, int, int*, s64*, int*);
s16 fn_0023B684(s16, s16, s16, s16);
// Reconstructed unused-result contract; original void/output-pointer spelling unknown.
void fn_0023B6CC(nn::math::VEC3*, const nn::math::MTX34*, const nn::math::VEC3*);
}
extern "C" void fn_001F4AD4(observed_hid::Reader* reader, observed_hid::Sample* samples,
                            int* count, int capacity) {
    fn_00297B48(reader->device->GetResource(), samples, capacity, count,
               &reader->timestamp, &reader->index);
    typedef s16 (*Filter)(s16, s16, s16, s16);
    Filter filter = fn_0023B684;
    for (int index = *count - 1; index >= 0; --index) {
        observed_hid::Sample& sample = samples[index];
        sample.axis[0] = filter(sample.axis[0], reader->previous.axis[0],
                                reader->threshold, reader->gain);
        reader->previous.axis[0] = sample.axis[0];
        sample.axis[1] = filter(sample.axis[1], reader->previous.axis[1],
                                reader->threshold, reader->gain);
        reader->previous.axis[1] = sample.axis[1];
        sample.axis[2] = filter(sample.axis[2], reader->previous.axis[2],
                                reader->threshold, reader->gain);
        reader->previous.axis[2] = sample.axis[2];
        if (reader->biasEnabled) {
            sample.axis[0] -= reader->bias.axis[0];
            sample.axis[1] -= reader->bias.axis[1];
            u16 currentZ = static_cast<u16>(sample.axis[2]);
            u16 biasZ = static_cast<u16>(reader->bias.axis[2]);
            sample.axis[2] = static_cast<s16>(currentZ - biasZ);
        }
        if (reader->transformEnabled) {
            if (!(reader->transform.m[0][0] == 1.0f && reader->transform.m[0][1] == 0.0f && reader->transform.m[0][2] == 0.0f &&
                  reader->transform.m[0][3] == 0.0f && reader->transform.m[1][0] == 0.0f && reader->transform.m[1][1] == 1.0f &&
                  reader->transform.m[1][2] == 0.0f && reader->transform.m[1][3] == 0.0f && reader->transform.m[2][0] == 0.0f &&
                  reader->transform.m[2][1] == 0.0f && reader->transform.m[2][2] == 1.0f && reader->transform.m[2][3] == 0.0f)) {
                const nn::math::MTX34* matrix = &reader->transform;
                float x = static_cast<float>(sample.axis[0]);
                float y = static_cast<float>(sample.axis[1]);
                float z = static_cast<float>(sample.axis[2]);
                nn::math::VEC3 value;
                value.x = x;
                value.y = y;
                value.z = z;
                fn_0023B6CC(&value, matrix, &value);
                sample.axis[0] = static_cast<s16>(static_cast<int>(value.x));
                sample.axis[1] = static_cast<s16>(static_cast<int>(value.y));
                sample.axis[2] = static_cast<s16>(static_cast<int>(value.z));
            }
        }
    }
}
