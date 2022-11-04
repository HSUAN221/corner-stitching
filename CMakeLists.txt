# declare cmake version
cmake_minimum_required (VERSION 3.15)

# set main project infomation
project (lcs VERSION 0.0.1 LANGUAGES CXX)

# add module search path
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/tools/cmake)

# import utilities
include(utilities)

# set global cxx standard as C++14
set_global_cxx_standard(14)

# set global windows export all symbols
set_global_windows_export_all_symbols()

# set global build output name same as install
set_global_build_output_name_same_as_install()

# set global rpath
set_global_rpath()

# set global option
set_option()

# set global coverage flags
# set_global_coverage_flags()

# set global tools
include_directories(${CMAKE_SOURCE_DIR}/tools)

#----------------------------------------------------------------------------
# Find googletest
#----------------------------------------------------------------------------
find_googletest()

#----------------------------------------------------------------------------
# Find OpenMP
#----------------------------------------------------------------------------
find_package(OpenMP)

#----------------------------------------------------------------------------
# add source
#----------------------------------------------------------------------------
add_subdirectory(src)

#----------------------------------------------------------------------------
# add tests
#----------------------------------------------------------------------------
add_subdirectory(tests)

#----------------------------------------------------------------------------
# Build examples
#----------------------------------------------------------------------------
add_subdirectory(examples)
