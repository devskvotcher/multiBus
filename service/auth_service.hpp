#pragma once
#include "repo/iuser_repo.hpp"
#include "security/crypto.hpp"
#include "security/token_store.hpp"

struct AuthConfig
{
    std::chrono::seconds access_ttl {std::chrono::minutes(5)};
    std::chrono::seconds refresh_ttl {std::chrono::days(30)};
};
struct AuthService
{
    const IUserRepo& repo;
    TokenStore& store;
    AuthConfig cfg;

    struct Pair {Token access; Token refresh;};

    Pair login(const std::string& login, const std::string& pass)
    {
        auto u = repo.find_by_login(login);
        if(!u || !u->is_active)
            throw std::runtime_error("Нет такого пользователя");
        if(hash_pass(pass, u->salt) != u->pass_hash)
            throw std::runtime_error("Неверный пароль");
        return 
        {
            store.issue(u->id, TokenKind::Access, cfg.access_ttl),
            store.issue(u->id, TokenKind::Refresh, cfg.refresh_ttl)
        };
    }

    Pair refresh(const std::string& refresh_token)
    {
        uint32_t uid = store.consume_refresh(refresh_token);
        return
        {
            store.issue(uid, TokenKind::Access, cfg.access_ttl),
            store.issue(uid, TokenKind::Refresh, cfg.refresh_ttl)
        };
    }
};
