// SPDX-License-Identifier: MIT
#define _BSD_SOURCE
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

void arc4random_buf(void *buffer, size_t bytes);
int __real_getentropy(void *buffer, size_t bytes);
static _Atomic unsigned entropy_calls;
static _Atomic unsigned injected_failures;
static _Atomic int force_entropy_failure;

int __wrap_getentropy(void *buffer, size_t bytes) {
    atomic_fetch_add(&entropy_calls, 1);
    if (atomic_load(&force_entropy_failure)) {
        atomic_fetch_add(&injected_failures, 1);
        puts("{\"case\":\"injected_entropy_failure\",\"stage\":\"entropy_source_refused\"}");
        fflush(stdout);
        errno = EIO;
        return -1;
    }
    return __real_getentropy(buffer, bytes);
}

enum { thread_count = 2, samples_per_thread = 32768, sample_bytes = 64 };
static unsigned char samples[thread_count][samples_per_thread][sample_bytes];
static _Atomic int worker_failed;

static void *generate_samples(void *argument) {
    const size_t index = *(const size_t *)argument;
    for (size_t sample = 0; sample < samples_per_thread; ++sample) {
        if (sample & 1) {
            if (RAND_bytes(samples[index][sample], sample_bytes) != 1)
                atomic_store(&worker_failed, 1);
        } else {
            arc4random_buf(samples[index][sample], sample_bytes);
        }
    }
    return NULL;
}

static int compare_sample(const void *left, const void *right) {
    return memcmp(left, right, sample_bytes);
}

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "--force-entropy-failure") == 0) {
        atomic_store(&force_entropy_failure, 1);
        puts("{\"case\":\"injected_entropy_failure\",\"stage\":\"before_random_call\"}");
        fflush(stdout);
        unsigned char bytes[32];
        arc4random_buf(bytes, sizeof(bytes));
        puts("{\"case\":\"injected_entropy_failure\",\"unexpected_success\":true}");
        return 90;
    }
    if (argc != 1) return 2;

    unsigned char first[256], second[256], refused[257];
    memset(refused, 0xa5, sizeof(refused));
    const int first_result = getentropy(first, sizeof(first));
    const int second_result = getentropy(second, sizeof(second));
    errno = 0;
    const int refused_result = getentropy(refused, sizeof(refused));
    const int refused_errno = errno;
    int refused_unchanged = 1;
    for (size_t index = 0; index < sizeof(refused); ++index)
        if (refused[index] != 0xa5) refused_unchanged = 0;
    if (first_result || second_result || memcmp(first, second, sizeof(first)) == 0 ||
        refused_result != -1 || refused_errno != EIO || !refused_unchanged) return 3;

    unsigned char digest[SHA256_DIGEST_LENGTH];
    static const unsigned char expected_digest[SHA256_DIGEST_LENGTH] = {
        0xba,0x78,0x16,0xbf,0x8f,0x01,0xcf,0xea,0x41,0x41,0x40,0xde,0x5d,0xae,0x22,0x23,
        0xb0,0x03,0x61,0xa3,0x96,0x17,0x7a,0x9c,0xb4,0x10,0xff,0x61,0xf2,0x00,0x15,0xad
    };
    if (SHA256((const unsigned char *)"abc", 3, digest) != digest ||
        memcmp(digest, expected_digest, sizeof(digest))) return 4;

    pthread_t threads[thread_count];
    size_t indices[thread_count];
    for (size_t index = 0; index < thread_count; ++index) {
        indices[index] = index;
        if (pthread_create(&threads[index], NULL, generate_samples, &indices[index])) return 5;
    }
    for (size_t index = 0; index < thread_count; ++index)
        if (pthread_join(threads[index], NULL)) return 6;
    if (atomic_load(&worker_failed)) return 7;

    const size_t count = thread_count * samples_per_thread;
    qsort(samples, count, sample_bytes, compare_sample);
    const unsigned char *flat = &samples[0][0][0];
    size_t duplicate_count = 0, zero_samples = 0;
    for (size_t index = 0; index < count; ++index) {
        if (index && !memcmp(flat + (index - 1) * sample_bytes,
                             flat + index * sample_bytes, sample_bytes)) ++duplicate_count;
        int nonzero = 0;
        for (size_t byte = 0; byte < sample_bytes; ++byte)
            nonzero |= flat[index * sample_bytes + byte];
        if (!nonzero) ++zero_samples;
    }
    const unsigned calls = atomic_load(&entropy_calls);
    printf("{\"complete\":true,\"getentropy_success_calls\":2,\"getentropy_range_refused\":true,"
           "\"refused_buffer_unchanged\":true,\"sha256_known_vector\":true,\"threads\":%u,"
           "\"random_samples\":%zu,\"random_bytes\":%zu,\"duplicate_samples\":%zu,"
           "\"zero_samples\":%zu,\"observed_entropy_calls\":%u,\"injected_failures\":%u}\n",
           thread_count,count,count*sample_bytes,duplicate_count,zero_samples,calls,
           atomic_load(&injected_failures));
    return duplicate_count || zero_samples || calls < 5;
}
