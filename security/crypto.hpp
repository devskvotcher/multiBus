#pragma once
#include <string>

std::string to_hex(unsigned char byte) {
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
    return ss.str();
}

inline std::string hash_pass(const std::string& pass, const std::string& salt)
{
    std::string combined = salt + pass;
    std::string result;
    
    // Ваша текущая логика, но с hex-выводом
//    for (char c : combined) {
//        unsigned char hashed = (c * 131) % 251;
//        result += to_hex(hashed);  // теперь "a5" вместо непечатного символа
//    }
    for (size_t i = 0; i < combined.length(); i++) 
    {
        
            int j = std::srand(i);
        char c = combined[i];
        unsigned char hashed = (c + std::srand(i) %(131)) % 251;
        result += to_hex(hashed);
    }
    
    return result;
}