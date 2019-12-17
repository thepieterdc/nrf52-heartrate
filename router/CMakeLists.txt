cmake_minimum_required(VERSION 3.15)
project(router)

# Build type.
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

message("Build type: ${CMAKE_BUILD_TYPE}")

# Compilation flags.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb -DIS_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -s")
add_compile_options(-Wall -Wextra -Wpedantic -Wno-missing-field-initializers -Wno-unused-parameter)

# Packages.
find_package(CURL REQUIRED)
find_package(Json REQUIRED)
find_package(BlueZ REQUIRED)

# Sources.
file(GLOB util_SRC
        "src/util/*.h"
        "src/util/*.cpp")

file(GLOB web_SRC
        "src/web/*.h"
        "src/web/*.cpp")

file(GLOB bt_SRC
        "src/bt/*.h"
        "src/bt/*.cpp")

add_executable(router src/main.cpp ${util_SRC} ${web_SRC} ${bt_SRC})
target_link_libraries(router curl)

set(BT_HRS_ADDRESS DD:BF:23:31:C0:B8)
add_compile_definitions(BT_HRS_ADDRESS="${BT_HRS_ADDRESS}")

message("${BLUEZ_FOUND} ${BLUEZ_INCLUDE_DIRS}")

include_directories(${JSON_INCLUDE_DIRS} ${BLUEZ_INCLUDE_DIRS})