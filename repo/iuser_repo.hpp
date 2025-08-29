#pragma once
#include <string>
#include <optional>
#include "domain/user.hpp"

struct IUserRepo 
{
    virtual ~IUserRepo() = default;
    virtual std::optional<User> find_by_login(const std::string& login) const = 0;   
};