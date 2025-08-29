#pragma once
#include <string>
#include <cstdint>

struct User {
    uint32_t id{};
    std::string login;
    std::string salt;
    std::string pass_hash;
    bool is_active{true};
};