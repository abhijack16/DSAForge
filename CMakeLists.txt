cmake_minimum_required(VERSION 3.16)
project(DSAForge VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(dsaforge INTERFACE)
add_library(DSAForge::dsaforge ALIAS dsaforge)

target_compile_features(dsaforge INTERFACE cxx_std_17)

target_include_directories(dsaforge INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

option(DSAFORGE_BUILD_TESTS "Build DSAForge tests" ON)
option(DSAFORGE_BUILD_EXAMPLES "Build DSAForge examples" ON)

if(DSAFORGE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(DSAFORGE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS dsaforge EXPORT DSAForgeTargets)
install(EXPORT DSAForgeTargets
    FILE DSAForgeTargets.cmake
    NAMESPACE DSAForge::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DSAForge
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/DSAForgeConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/DSAForgeConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DSAForge
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/DSAForgeConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/DSAForgeConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/DSAForgeConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DSAForge
)

message(STATUS "DSAForge ${PROJECT_VERSION}: C++17 header-only library")
