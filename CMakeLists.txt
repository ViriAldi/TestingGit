cmake_minimum_required(VERSION 3.13)
project(aks_lab3_part2)

set(CMAKE_CXX_STANDARD 17)

if (${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
    add_compile_options(-O2)
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    add_compile_options(-O2)
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES MSVC)
    add_compile_options(/O2)
endif()

find_package(LibArchive REQUIRED)
find_package( ICU REQUIRED COMPONENTS uc i18n )

set (Boost_USE_STATIC_LIBS OFF)
set (Boost_USE_MULTITHREAD ON)
find_package( Boost REQUIRED COMPONENTS filesystem locale system)

message(${Boost_LIBRARIES})
file(GLOB SOURCES src/*.cpp)

add_executable(aks_lab3_part2 ${SOURCES})
target_link_libraries(aks_lab3_part2 ${Boost_LIBRARIES} ${ICU_LIBRARIES} ${LibArchive_LIBRARIES})
target_include_directories(aks_lab3_part2 PUBLIC ${Boost_INCLUDE_DIRS} ${ICU_INCLUDE_DIRS} ${LibArchive_INCLUDE_DIRS} inc)
