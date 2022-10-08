cmake_minimum_required(VERSION 3.5)

project(PixelUI)

file(GLOB_RECURSE PixelUISrc
"Widget/*.h"
"Widget/*.cpp"
"*.cpp")

add_executable(PixelUI ${PixelUISrc})

target_link_libraries(PixelUI PUBLIC glfw bgfx bx)

# target_include_directories(PixelUI PUBLIC
# "../ThirdParty/glfw/include")