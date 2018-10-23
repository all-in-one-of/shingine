cmake_minimum_required(VERSION 3.12)
project(shingine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(External/include)
include_directories(Include)

file (GLOB_RECURSE cpp_files ${CMAKE_SOURCE_DIR}/Source/*.cpp ${CMAKE_SOURCE_DIR}/Source/*.c ${CMAKE_SOURCE_DIR}/Include/*.cpp)
file (GLOB_RECURSE h_files ${CMAKE_SOURCE_DIR}/Source/*.h)

set (source_files ${cpp_files} ${h_files})
add_executable(shingine ${source_files})

if(WIN32)
target_link_libraries(shingine ${CMAKE_SOURCE_DIR}/External/lib/x64/glfw3.lib)
endif()

if(APPLE)
target_link_libraries(shingine "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo" "${CMAKE_SOURCE_DIR}/External/lib/x64/libglfw3.a")
endif()