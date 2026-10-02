// SPDX-License-Identifier: GPL-2.0-or-later
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <openssl/sha.h>

namespace {

enum class BrowserInputIdentityStatus : std::uint32_t {
    Valid = 0,
    InvalidArgument = 1,
    MalformedSha256 = 2,
    OpenFailed = 3,
    ReadFailed = 4,
    ByteCountMismatch = 5,
    HashFailed = 6,
    CloseFailed = 7,
    HashMismatch = 8,
    BufferConfigurationFailed = 9,
};

constexpr std::size_t MaximumPathBytes = 4096;
constexpr std::size_t ReadBufferBytes = 64 * 1024;
constexpr std::size_t Sha256HexadecimalCharacters = SHA256_DIGEST_LENGTH * 2;

bool IsBoundedAbsolutePath(const char* path) noexcept {
    if (path == nullptr || path[0] != '/') {
        return false;
    }
    for (std::size_t index = 1; index < MaximumPathBytes; ++index) {
        if (path[index] == '\0') {
            return index > 1;
        }
    }
    return false;
}

int DecodeLowercaseHexadecimal(char character) noexcept {
    if (character >= '0' && character <= '9') {
        return character - '0';
    }
    if (character >= 'a' && character <= 'f') {
        return character - 'a' + 10;
    }
    return -1;
}

bool DecodeSha256(const char* text,
                  std::array<unsigned char, SHA256_DIGEST_LENGTH>& bytes) noexcept {
    for (std::size_t index = 0; index < bytes.size(); ++index) {
        const int high = DecodeLowercaseHexadecimal(text[index * 2]);
        if (high < 0) {
            return false;
        }
        const int low = DecodeLowercaseHexadecimal(text[index * 2 + 1]);
        if (low < 0) {
            return false;
        }
        bytes[index] = static_cast<unsigned char>((high << 4) | low);
    }
    return text[Sha256HexadecimalCharacters] == '\0';
}

BrowserInputIdentityStatus HashFile(
    std::FILE* stream, std::uint32_t expected_byte_count,
    std::array<unsigned char, SHA256_DIGEST_LENGTH>& digest) noexcept {
    SHA256_CTX context{};
    if (SHA256_Init(&context) != 1) {
        return BrowserInputIdentityStatus::HashFailed;
    }

    std::array<unsigned char, ReadBufferBytes> buffer;
    std::uint32_t consumed_bytes = 0;
    while (consumed_bytes < expected_byte_count) {
        const std::uint32_t remaining_bytes = expected_byte_count - consumed_bytes;
        const std::size_t requested_bytes = remaining_bytes < buffer.size()
                                                ? remaining_bytes
                                                : buffer.size();
        const std::size_t read_bytes = std::fread(buffer.data(), 1, requested_bytes, stream);
        if (std::ferror(stream) != 0) {
            return BrowserInputIdentityStatus::ReadFailed;
        }
        if (read_bytes == 0) {
            return std::feof(stream) != 0 ? BrowserInputIdentityStatus::ByteCountMismatch
                                         : BrowserInputIdentityStatus::ReadFailed;
        }
        if (SHA256_Update(&context, buffer.data(), read_bytes) != 1) {
            return BrowserInputIdentityStatus::HashFailed;
        }
        consumed_bytes += static_cast<std::uint32_t>(read_bytes);
        if (read_bytes < requested_bytes && std::feof(stream) != 0) {
            return BrowserInputIdentityStatus::ByteCountMismatch;
        }
    }

    // Read one extra byte to distinguish an exact file from a matching prefix.
    const int trailing_byte = std::fgetc(stream);
    if (std::ferror(stream) != 0) {
        return BrowserInputIdentityStatus::ReadFailed;
    }
    if (trailing_byte != EOF) {
        return BrowserInputIdentityStatus::ByteCountMismatch;
    }
    if (std::feof(stream) == 0) {
        return BrowserInputIdentityStatus::ReadFailed;
    }
    if (SHA256_Final(digest.data(), &context) != 1) {
        return BrowserInputIdentityStatus::HashFailed;
    }
    return BrowserInputIdentityStatus::Valid;
}

} // namespace

// The worker supplies valid, NUL-terminated string storage, an explicitly
// read-only WORKERFS path, and a checked File.size representable as uint32_t.
// This function validates bytes only. It does not initialize the game runtime.
extern "C" std::uint32_t BrowserInputIdentityValidateSha256(
    const char* absolute_path, const char* lowercase_sha256,
    std::uint32_t expected_byte_count) noexcept {
    if (lowercase_sha256 == nullptr || !IsBoundedAbsolutePath(absolute_path)) {
        return static_cast<std::uint32_t>(BrowserInputIdentityStatus::InvalidArgument);
    }
    std::array<unsigned char, SHA256_DIGEST_LENGTH> expected_digest{};
    if (!DecodeSha256(lowercase_sha256, expected_digest)) {
        return static_cast<std::uint32_t>(BrowserInputIdentityStatus::MalformedSha256);
    }

    std::FILE* stream = std::fopen(absolute_path, "rb");
    if (stream == nullptr) {
        return static_cast<std::uint32_t>(BrowserInputIdentityStatus::OpenFailed);
    }
    // Prevent stdio read-ahead beyond each explicit bounded read request.
    if (std::setvbuf(stream, nullptr, _IONBF, 0) != 0) {
        std::fclose(stream);
        return static_cast<std::uint32_t>(BrowserInputIdentityStatus::BufferConfigurationFailed);
    }
    std::array<unsigned char, SHA256_DIGEST_LENGTH> actual_digest{};
    const BrowserInputIdentityStatus status = HashFile(stream, expected_byte_count, actual_digest);
    const int close_result = std::fclose(stream);
    if (status != BrowserInputIdentityStatus::Valid) {
        return static_cast<std::uint32_t>(status);
    }
    if (close_result != 0) {
        return static_cast<std::uint32_t>(BrowserInputIdentityStatus::CloseFailed);
    }
    if (actual_digest != expected_digest) {
        return static_cast<std::uint32_t>(BrowserInputIdentityStatus::HashMismatch);
    }
    return static_cast<std::uint32_t>(BrowserInputIdentityStatus::Valid);
}
