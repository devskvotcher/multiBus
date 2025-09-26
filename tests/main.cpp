#include <iostream>
#include <iomanip>
#include "repo/mem_user_repo.hpp"
#include "service/auth_service.hpp"

int main() {
    MemUserRepo repo;
    // Готовим пользователя alex:1234
    User u{1, "alex", "NaCl", hash_pass("1234","NaCl"), true};
    repo.by_login.emplace(u.login, u);

    TokenStore store;
    AuthConfig cfg;
    // Для наглядности уменьшим TTL:
    cfg.access_ttl  = std::chrono::seconds(15);
    cfg.refresh_ttl = std::chrono::seconds(90);

    AuthService auth{repo, store, cfg};

    auto p = auth.login("alex","1234");
    auto head = [](const std::string& s){
        return s.substr(0, std::min<size_t>(32, s.size()));
    };
    std::cout << "ACCESS len="  << p.access.value.size()
              << " head=" << head(p.access.value)   << "\n";
    std::cout << "REFRESH len=" << p.refresh.value.size()
              << " head=" << head(p.refresh.value)  << "\n";

std::cout <<"Test hash function"<<std::endl;
std::cout << "hash('1234', 'NaCl') = " << hash_pass("1234", "NaCl") << std::endl;
std::cout << "hash('1', 'NaCl') = " << hash_pass("1", "NaCl") << std::endl;
std::cout << "hash('abcd', 'NaCl') = " << hash_pass("abcd", "NaCl") << std::endl;


try {
    auto p2 = auth.login("alex", "1999");  // первый символ '1'
    std::cout << "ВНИМАНИЕ: login с паролем '1999' прошел!" << std::endl;
} catch (const std::runtime_error& e) {
    std::cout << "Пароль '1999' отклонен: " << e.what() << std::endl;
}
    // Проверка доступа
    try {
        auto uid = store.check_access(p.access.value);
        std::cout << "check_access OK, user_id=" << uid << "\n";
    } catch (const std::exception& e) {
        std::cout << "check_access FAIL: " << e.what() << "\n";
    }

    // Негатив 1: неверный пароль
    try {
        auth.login("alex","BAD");
        std::cout << "ERROR: ожидалось исключение (неверный пароль)\n";
    } catch (const std::runtime_error& e) {
        std::cout << "ОК (ожидаемо): " << e.what() << "\n";
    }

    // Негатив 2: несуществующий логин
    try {
        auth.login("nope","1234");
        std::cout << "ERROR: ожидалось исключение (нет такого пользователя)\n";
    } catch (const std::runtime_error& e) {
        std::cout << "ОК (ожидаемо): " << e.what() << "\n";
    }

    return 0;
}
