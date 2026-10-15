cmake_minimum_required(VERSION 3.10.2)
project(RetroEmulatorAndroid)

set(CMAKE_CXX_STANDARD 17)

# Procurar a biblioteca SDL2 para renderização gráfica e eventos
find_package(SDL2 REQUIRED)

# Executável principal do emulador
add_executable(retro_emulator
    main.cpp
)

# Ligar as bibliotecas necessárias
target_link_libraries(retro_emulator
    SDL2::SDL2
)
