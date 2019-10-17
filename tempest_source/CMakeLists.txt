cmake_minimum_required(VERSION 3.15.0)

project(SupremeToyInc)

# setup policies
cmake_policy(SET CMP0091 NEW) # allow setting of the MSVC runtime library
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") # statically linked Debug/Release

# Specify the available configurations (default has a few more)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE)

set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Configurations
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty
    ${CMAKE_PREFIX_PATH}
    )

# Download Dependencies
find_package(Bullet-2.88 REQUIRED)

# Enable the use of folders in the solution and file structures in IDE
set_property( GLOBAL PROPERTY USE_FOLDERS ON)

# Set all projects to output to the same directory
# Changing this directory will require updating the default path inside Bootstrap.
#   These are synchronized to make debugging the application within VS easier.
if(WIN32)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
else()
    # On Linux, executables don't get extensions, which causes a problem when
    #  the target is written to the same directory as the build files
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Runtime)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Runtime)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Runtime)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdeclspec -fms-extensions")
endif()

set(CMAKE_CXX_STANDARD 17)
link_directories("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# Bring in all modules
# #######################################################
#                BEGIN MODULES REGISTRY
# #######################################################
add_subdirectory(Logger)
add_subdirectory(Math)
add_subdirectory(Configuration)
add_subdirectory(Utility)
add_subdirectory(SerializationCore)
add_subdirectory(MathSerialization)
add_subdirectory(EngineInterfaces)
add_subdirectory(ComponentCore)
add_subdirectory(SystemCore)
add_subdirectory(EventCore)
add_subdirectory(ComponentRepository)
add_subdirectory(EngineRunner)
# #######################################################
#                 END MODULES REGISTRY
# #######################################################

# Bring in Audio engine and tool
add_subdirectory(Engine/AudioEngine)
add_subdirectory(Engine/AudioTool)

# Bring in graphics, physics and the engine
add_subdirectory(Graphics.OpenGL4)
add_subdirectory(Physics.Bullet)
add_subdirectory(Engine/TempestEngine)

# Bring in the google test functionality
set(BUILD_GTEST ON CACHE BOOL "")
set(BUILD_GMOCK OFF CACHE BOOL "")
set(INSTALL_GMOCK OFF CACHE BOOL "")
set(INSTALL_GTEST OFF CACHE BOOL "")
add_subdirectory(ThirdParty/googletest-master)

# Bring in the bootstrap, editor and unittests
add_subdirectory(Bootstrap)
add_subdirectory(Editor)
add_subdirectory(Engine/UnitTests)

# Bring in the TempestPrebuilder if requested
option(INCLUDE_TEMPEST_PREBUILDER "Include the Tempest Prebuilder project and its dependencies (very large)" OFF)
if(INCLUDE_TEMPEST_PREBUILDER)
    add_subdirectory(TempestPrebuilder)
endif(INCLUDE_TEMPEST_PREBUILDER)

# Organize the targets into folders
set_target_properties(
    ImGui
    ImGui_OpenGL3_GLFW_GLEW
    ImGui_Stdlib
    ImGuizmo
    gtest
    gtest_main
    PROPERTIES FOLDER "ThirdParty"
)
