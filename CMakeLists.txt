cmake_minimum_required(VERSION 3.8)
set(CMAKE_GENERATOR_PLATFORM "x64" CACHE STRING "" FORCE)
PROJECT(YohaneAPMod)
set(CMAKE_CXX_STANDARD 17)
ENABLE_LANGUAGE(C CXX ASM_MASM)

include_directories(${PROJECT_SOURCE_DIR})
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.h ${PROJECT_SOURCE_DIR}/src/YOHANE_BID/*.h ${PROJECT_SOURCE_DIR}/src/YOHANE_BID/*.cpp ${PROJECT_SOURCE_DIR}/src/ModLoader/*.h ${PROJECT_SOURCE_DIR}/src/ModLoader/*.cpp ${PROJECT_SOURCE_DIR}/src/Archipelago/*.cpp ${PROJECT_SOURCE_DIR}/src/Archipelago/*.h)
ADD_LIBRARY(YohaneAPMod SHARED ${SRC_FILES})

# APCpp
add_subdirectory(lib/APCpp lib/APCpp/build)
include_directories(lib/APCpp)
target_link_libraries(YohaneAPMod APCpp)
