cmake_minimum_required(VERSION 3.10)

project(WHYNOT)

set(CMAKE_CXX_STANDARD 20)

# Define paths
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/libs)
set(GLAD_SRC ${INCLUDE_DIR}/glad/src/glad.c)
set(MAIN_SRC ${CMAKE_SOURCE_DIR}/src/main.cpp)

# Include directories
include_directories(
        ${INCLUDE_DIR}
        ${INCLUDE_DIR}/glfw
        ${INCLUDE_DIR}/yaml-cpp
        ${INCLUDE_DIR}/assimp
        ${INCLUDE_DIR}/freetype/include
        ${INCLUDE_DIR}/AL
        ${INCLUDE_DIR}/glm
        ${INCLUDE_DIR}/glad/include/glad
        ${INCLUDE_DIR}/glad/include/KHR
        src
)

# Locate source files
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/**/*.cpp)
list(APPEND SOURCES ${MAIN_SRC})
list(APPEND SOURCES ${GLAD_SRC})

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Ensure the linker knows where to look for libraries
link_directories(${LIB_DIR})

# Link libraries (without .lib extensions)
target_link_libraries(${PROJECT_NAME}
        ${LIB_DIR}/glfw3.lib
        ${LIB_DIR}/yaml-cppd.lib
        ${LIB_DIR}/assimp-vc143-mtd.lib
        ${LIB_DIR}/freetyped.lib
        ${LIB_DIR}/OpenAL32.lib
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LIB_DIR}/assimp-vc143-mtd.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})