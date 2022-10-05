cmake_minimum_required (VERSION 3.16.0)

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest/
  GIT_TAG 0320f517fd920866d918e564105d68fd4362040a
)


option(HAS_STL "has_std" ON)

set(BUILD_GMOCK ON CACHE BOOL "" FORCE)

# -----------------------------------------------------------------------------
# Project
# -----------------------------------------------------------------------------
project(ctd VERSION 0.0.1 DESCRIPTION "Commander sTandard Library")
include(GNUInstallDirs)
include(GoogleTest)
FetchContent_MakeAvailable(googletest)
#add_compile_options(-Wall -Wextra -pedantic -Werror)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

if(${HAS_STL})
	add_compile_options(-DHAS_STL=1)
endif()

file(GLOB CTD_INCLUDE include/ctd/*.hpp)
file(GLOB CTD_SRCS src/*.cpp)
file(GLOB CTD_TEST_SRCS test/*.cpp)

# -----------------------------------------------------------------------------
# Library
# -----------------------------------------------------------------------------
#add_library(ctd ${CTD_SRCS})
#set_target_properties(ctd PROPERTIES VERSION ${PROJECT_VERSION})
#set_target_properties(ctd PROPERTIES SOVERSION 1)
# set_target_properties(ctd PROPERTIES PUBLIC_HEADER ${CTD_INCLUDE})
#target_include_directories(ctd PRIVATE include)
#target_include_directories(ctd PRIVATE src)

#install(TARGETS ctd
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# -----------------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------------
enable_testing()
add_executable(UnitTests ${CTD_SRCS} ${CTD_TEST_SRCS})
target_include_directories(UnitTests PRIVATE include)
target_include_directories(UnitTests PRIVATE src/)
target_compile_definitions(UnitTests PUBLIC GTEST_LINKED_AS_SHARED_LIBRARY)
target_link_libraries(UnitTests GTest::gmock_main GTest::gmock)

if (WIN32)
#    add_custom_command(
#        TARGET UnitTests POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:UnitTests> $<TARGET_FILE_DIR:UnitTests>
#        COMMAND_EXPAND_LISTS
#    )

#    add_custom_command(
#        TARGET ctd POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:ctd> $<TARGET_FILE_DIR:ctd>
#        COMMAND_EXPAND_LISTS
#    )
endif ()

gtest_discover_tests(UnitTests)