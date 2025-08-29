#pragma once
#include <cstdint>
#include <string>
#include <chrono>


enum class TokenKind : uint8_t {Access, Refresh};

struct Token
{
    std::string value;
    std::chrono::steady_clock::time_point expire;
    TokenKind kind{};
    uint32_t user_id{};
};

