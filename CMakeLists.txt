cmake_minimum_required(VERSION 3.20)

project(WaterSimulation LANGUAGES C CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.h"
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
)

# External library paths
set(GLFW_ROOT_DIR "${CMAKE_SOURCE_DIR}/external/glfw")
set(GLAD_ROOT_DIR "${CMAKE_SOURCE_DIR}/external/glad")
set(GLM_ROOT_DIR "${CMAKE_SOURCE_DIR}/external/glm")

# Find OpenGL
find_package(OpenGL REQUIRED)

# Build external libraries
add_subdirectory(${GLFW_ROOT_DIR})
add_subdirectory(${GLAD_ROOT_DIR})

# Create executable
add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

# Include headers
target_include_directories(${PROJECT_NAME} PRIVATE
    "${GLFW_ROOT_DIR}/include"
    "${GLAD_ROOT_DIR}/include"
    "${CMAKE_SOURCE_DIR}/external"
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glad
    OpenGL::GL
)