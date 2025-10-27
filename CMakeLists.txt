cmake_minimum_required(VERSION 3.28)
project(dinning_phi C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pipe")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Ofast -march=native -flto -fomit-frame-pointer")

find_package(OpenMP REQUIRED)

add_executable(${PROJECT_NAME}
    src/FIFO.c
    src/dinning_FIFO.c
    src/dinning_locks.c
    src/main.c
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_C)
