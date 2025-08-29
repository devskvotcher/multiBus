cmake_minimum_required(VERSION 3.20)
project(auth_demo LANGUAGES CXX)

# Стандарт языка
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Если хочешь строго стандарт без расширений компилятора — оставь OFF
# Если целишься в gnu++20 — поставь ON
set(CMAKE_CXX_EXTENSIONS OFF)

# Предупреждения
if(MSVC)
  add_compile_options(/W4 /permissive- /utf-8)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Исполняемый файл (у нас только main.cpp, остальное — заголовки)
add_executable(auth_demo
    tests/main.cpp
)

# Заголовки ищем от корня проекта
target_include_directories(auth_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)