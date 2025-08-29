#pragma once
#include <string>

inline std::string hash_pass(const std::string& pass, const std::string& salt)
{
    std::string r = salt;
    for (char c: pass)
    {
        r.push_back(static_cast<char> ((c*131)%251));
        return r;
    }    
}