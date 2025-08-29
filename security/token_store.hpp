#pragma once
#include "security/token.hpp"
#include <unordered_map>
#include <random>
#include <stdexcept>

struct TokenStore
{
    std::unordered_map<std::string, Token> db;
    static std::string random_string(size_t n = 48)
    {
        static thread_local std::mt19937_64 rng(std::random_device{}());
        static const char alf[] =
            "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";
            std::string s;
            s.reserve(n);
            for (size_t i = 0; i < n; i++)
            {
                s.push_back(alf[rng()%(sizeof(alf)-1)]);
            }  
            return s;
    }

    Token issue(uint32_t user_id, TokenKind kind, std::chrono::seconds ttl)
    {
        Token t;
        do {
            t.value = random_string();
        } while (db.find(t.value) != db.end());
        t.expire = std::chrono::steady_clock::now() + ttl;
        t.kind = kind;
        t.user_id = user_id;
        db[t.value] = t;
        return t;
    }

    uint32_t check_access(const std::string& access)
    {
        auto it = db.find(access);
        if(it == db.end() || it->second.kind != TokenKind::Access)
            throw std::runtime_error("Недействительный токен доступа");
        if(std::chrono::steady_clock::now() >= it->second.expire)
            throw std::runtime_error("Токен доступа истек");
        return it->second.user_id;

    }

    uint32_t consume_refresh(const std::string & refresh)
    {
        auto it = db.find(refresh);
        if(it == db.end() || it->second.kind != TokenKind::Refresh)
            throw std::runtime_error("Недействительный токен обновления");
        if(std::chrono::steady_clock::now() >= it->second.expire)
        {
            db.erase(it);
            throw std::runtime_error("Токен обновления истек");
        }
        uint32_t uid = it->second.user_id;
        db.erase(it);
        return uid;        
    }
};