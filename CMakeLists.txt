cmake_minimum_required (VERSION 3.4)
project(coursework)

#######################
# CMake configuration #
#######################

# C++ 11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ELF RPATH
set(CMAKE_INSTALL_RPATH "$ORIGIN")

# Default install dir
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/output/ )

#####################
# Libraries finding #
#####################

find_package(CUDA)      #CUDA
find_package(OpenMP)    #OpenMP
find_package(Threads)   #Threads
find_package(ArrayFire) #ArrayFire

###################
# Project options #
###################

#filters:CPU
option(ENABLE_FILTER_CPU "Enable CPU filter" ON)

#filters:CUDA
#use CUDA on MSVS 2015 only with CUDA toolkit 8.0 or greater
#don't use CUDA on Intel Compiler
if(CUDA_FOUND)
    if(NOT "${CMAKE_CXX_COMPILER}" MATCHES "icl")
        if((NOT MSVC) OR (MSVC_VERSION LESS 1900) OR ( CUDA_VERSION.MAJOR GREATER 7) )
            option(ENABLE_FILTER_CUDA "Enable filter based on CUDA technology" ON)
        endif() 
    endif()
endif()

#CUDA Linux fixes
if(CUDA_FOUND AND NOT WIN32)
    # enable C++11 for CUDA on non-Windows platforms
    list(APPEND CUDA_NVCC_FLAGS "-std=c++11")

    # force inlines to fix build in Release mode
    list(APPEND CUDA_NVCC_FLAGS "-D_FORCE_INLINES")
endif()

#filters:ArrayFire
if(CUDA_FOUND AND ArrayFire_FOUND)
   option(ENABLE_FILTER_ARRAYFIRE "Enable ArrayFire median filtering implementation" ON)
endif()

#openMP feature
if(OPENMP_FOUND)
    option(ENABLE_FEATURE_OPENMP "Enable OpenMP acceleration for CPU filter" ON)
endif()
if(ENABLE_FEATURE_OPENMP)
    set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# arch optimizations
include(${CMAKE_SOURCE_DIR}/cmake/arch_detect.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/arch_configure.cmake)

# config generation
configure_file("src/config.h.cmake" "config.h")

# echo all settings
include(${CMAKE_SOURCE_DIR}/cmake/echo_settings.cmake)

##################
# Project files  #
##################

#include directories
include_directories(
   "${CMAKE_SOURCE_DIR}/src/libraries/libcoursework"
   "${CMAKE_BINARY_DIR}"
)
if(ENABLE_FILTER_CPU)
    include_directories("${CMAKE_SOURCE_DIR}/src/libraries/libcoursework_cpu")
endif()
if(ENABLE_FILTER_CUDA)
    include_directories("${CMAKE_SOURCE_DIR}/src/libraries/libcoursework_cuda")
    include_directories("${CMAKE_SOURCE_DIR}/src/libraries/libcoursework_cuda/nvidia_helpers")
endif()
if(ENABLE_FILTER_ARRAYFIRE)
    include_directories("${CMAKE_SOURCE_DIR}/src/libraries/libcoursework_arrayfire")
endif()

# files: libraries
file(GLOB_RECURSE LIBCOURSEWORK_SRC "src/libraries/libcoursework/*.cpp")
file(GLOB_RECURSE LIBCOURSEWORK_HDR "src/libraries/libcoursework/*.h")

file(GLOB_RECURSE LIBCOURSEWORK_CPU_SRC "src/libraries/libcoursework_cpu/*.cpp")
file(GLOB_RECURSE LIBCOURSEWORK_CPU_HDR "src/libraries/libcoursework_cpu/*.h")

file(GLOB_RECURSE LIBCOURSEWORK_CUDA_SRC "src/libraries/libcoursework_cuda/*.cpp" "src/libraries/libcoursework_cuda/*.cu")
file(GLOB_RECURSE LIBCOURSEWORK_CUDA_HDR "src/libraries/libcoursework_cuda/*.h" "src/libraries/libcoursework_cuda/nvidia_helpers/*.h")

file(GLOB_RECURSE LIBCOURSEWORK_ARRAYFIRE_SRC "src/libraries/libcoursework_arrayfire/*.cpp")
file(GLOB_RECURSE LIBCOURSEWORK_ARRAYFIRE_HDR "src/libraries/libcoursework_arrayfire/*.h")



# files: utils
file(GLOB_RECURSE UTILS_ADDNOISE_SRC  "src/utils/addnoise/*.cpp")
file(GLOB_RECURSE UTILS_ANALYZE_SRC   "src/utils/analyze/*.cpp")
file(GLOB_RECURSE UTILS_GRAYSCALE_SRC "src/utils/grayscale/*.cpp")
file(GLOB_RECURSE UTILS_FILTER_SRC    "src/utils/filter/*.cpp")

# files: tests
file(GLOB_RECURSE TEST_MAIN_SRC "src/tests/test_main/*.cpp")

############
# Building #
############


# Library: main
add_library(libcoursework SHARED 
    ${LIBCOURSEWORK_SRC} 
    ${LIBCOURSEWORK_HDR} 
)
set_target_properties(libcoursework PROPERTIES COMPILE_DEFINITIONS "LIBCOURSEWORK_EXPORT")
set_target_properties(libcoursework PROPERTIES PREFIX "")
install(TARGETS libcoursework LIBRARY DESTINATION . RUNTIME DESTINATION .)

# Executable: add noise
add_executable(coursework_addnoise ${UTILS_ADDNOISE_SRC})
target_link_libraries(coursework_addnoise libcoursework)
install(TARGETS coursework_addnoise RUNTIME DESTINATION .)

# Executable: analyze
add_executable(coursework_analyze ${UTILS_ANALYZE_SRC} )
target_link_libraries(coursework_analyze libcoursework)
install(TARGETS coursework_analyze RUNTIME DESTINATION .)

# Executable: filter
add_executable(coursework_filter ${UTILS_FILTER_SRC})
target_link_libraries(coursework_filter libcoursework)
install(TARGETS coursework_filter RUNTIME DESTINATION .)

# Executable: grayscale
add_executable(coursework_grayscale ${UTILS_GRAYSCALE_SRC} )
target_link_libraries(coursework_grayscale libcoursework)
install(TARGETS coursework_grayscale RUNTIME DESTINATION .)

# Test: main
add_executable(coursework_test_main ${TEST_MAIN_SRC})
target_link_libraries(coursework_test_main libcoursework)
install(TARGETS coursework_test_main RUNTIME DESTINATION .)

# Library: CPU filter
if(ENABLE_FILTER_CPU)
    add_library(libcoursework_cpu SHARED ${LIBCOURSEWORK_CPU_SRC} ${LIBCOURSEWORK_CPU_HDR})
    set_target_properties(libcoursework_cpu PROPERTIES COMPILE_DEFINITIONS "LIBCOURSEWORK_CPU_EXPORT")
    set_target_properties(libcoursework_cpu PROPERTIES PREFIX "")
    target_link_libraries(libcoursework_cpu libcoursework)
    target_link_libraries(coursework_filter libcoursework_cpu)
    target_link_libraries(coursework_test_main libcoursework_cpu)
    install(TARGETS libcoursework_cpu LIBRARY DESTINATION . RUNTIME DESTINATION .)
endif()


# Library: CUDA filter
if(ENABLE_FILTER_CUDA)
    list(APPEND GENCODE 
        -gencode=arch=compute_20,code=sm_20 
        -gencode=arch=compute_30,code=sm_30
        -gencode=arch=compute_35,code=sm_35
        -gencode=arch=compute_50,code=sm_50
        -gencode=arch=compute_50,code=compute_50
    )
    cuda_add_library(libcoursework_cuda SHARED ${LIBCOURSEWORK_CUDA_SRC} ${LIBCOURSEWORK_CUDA_HDR} OPTIONS ${GENCODE} -DLIBCOURSEWORK_CUDA_EXPORT)
    set_target_properties(libcoursework_cuda PROPERTIES COMPILE_DEFINITIONS "LIBCOURSEWORK_CUDA_EXPORT")
    set_target_properties(libcoursework_cuda PROPERTIES PREFIX "")
    target_link_libraries(libcoursework_cuda libcoursework)
    target_link_libraries(coursework_filter libcoursework_cuda)
    target_link_libraries(coursework_test_main libcoursework_cuda)
    install(TARGETS libcoursework_cuda LIBRARY DESTINATION . RUNTIME DESTINATION .)
endif()

# Library: ArrayFire filter
if(CUDA_FOUND AND ENABLE_FILTER_ARRAYFIRE)
    add_library(libcoursework_arrayfire SHARED ${LIBCOURSEWORK_ARRAYFIRE_SRC} ${LIBCOURSEWORK_ARRAYFIRE_HDR})
    set_target_properties(libcoursework_arrayfire PROPERTIES COMPILE_DEFINITIONS "LIBCOURSEWORK_ARRAYFIRE_EXPORT")
    set_target_properties(libcoursework_arrayfire PROPERTIES PREFIX "")
    target_include_directories(libcoursework_arrayfire PUBLIC ${ArrayFire_INCLUDE_DIRS})
    target_link_libraries(libcoursework_arrayfire libcoursework ${ArrayFire_CUDA_LIBRARIES})
    target_link_libraries(coursework_filter libcoursework_arrayfire)
    target_link_libraries(coursework_test_main libcoursework_arrayfire)
    install(TARGETS libcoursework_arrayfire LIBRARY DESTINATION . RUNTIME DESTINATION .)
endif()
################
# Installation #
################

# assets copy
file(COPY ${PROJECT_SOURCE_DIR}/assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets/)
file(COPY ${PROJECT_SOURCE_DIR}/assets/ DESTINATION ${CMAKE_INSTALL_PREFIX}/assets/)
