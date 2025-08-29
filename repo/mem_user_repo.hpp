#pragma once
#include "repo/iuser_repo.hpp"
#include <unordered_map>

struct MemUserRepo final : IUserRepo
{
    std::unordered_map<std::string, User> by_login;
    std::optional<User> find_by_login(const std::string& login) const override
    {
        if(auto it = by_login.find(login); it != by_login.end())
        return it->second;
    return std::nullopt;
    }
};
