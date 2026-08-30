cmake_minimum_required(VERSION 3.20)

if(NOT WIN32)
    message(FATAL_ERROR "This project supports Windows only.")
endif()

project(OpenGL-Model-Loader LANGUAGES CXX)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "This project requires MinGW GCC.")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages from vcpkg
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(fastgltf CONFIG REQUIRED)

# Get all source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
)

# Build executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
)

# Enable experimental features for GLM
target_compile_definitions(${PROJECT_NAME} PRIVATE GLM_ENABLE_EXPERIMENTAL)

# Add all include files from include directory
target_include_directories(${PROJECT_NAME} PRIVATE
    include
)

# Link dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glad::glad
    glm::glm
    fastgltf::fastgltf
)

# Link the static GCC so it runs on every Windows system
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_options(${PROJECT_NAME} PRIVATE
        "-static"
        "-static-libgcc"
        "-static-libstdc++"
    )
endif()

# stb is header-only
target_include_directories(${PROJECT_NAME} PRIVATE
    ${Stb_INCLUDE_DIR}
)

# OpenGL
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        opengl32
    )
endif()


# Copy assets and shaders

add_custom_target(CopyAssets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Shaders"
        "${CMAKE_CURRENT_BINARY_DIR}/Shaders"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "${CMAKE_CURRENT_BINARY_DIR}/assets"

    COMMENT "Syncing Shaders and Assets..."
)

add_dependencies(${PROJECT_NAME} CopyAssets)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION final_build
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/Shaders DESTINATION final_build)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets DESTINATION final_build)
