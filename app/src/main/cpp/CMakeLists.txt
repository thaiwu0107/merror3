# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

# make sure a specific build type is setup
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "only Release/Debug" FORCE)
endif()

# include the ModiFace helpers
include(cmake/MFECommonCMake/MFECommonCMake.cmake)


###############################
# Project Specific setup bellow

project(mfemakeupsdkdemo)
set(demoLibName mfemakeupsdkdemo)

set(dependenciesDir "${CMAKE_CURRENT_LIST_DIR}/../../../dependencies")
get_filename_component(dependenciesDir ${dependenciesDir} ABSOLUTE)

set(mfeMakeupSDKDir "${dependenciesDir}/MFEMakeupSDK")
get_filename_component(mfeMakeupSDKDir ${mfeMakeupSDKDir} ABSOLUTE)

# get OpenCV
include(${dependenciesDir}/opencv/install/${ANDROID_ABI}/sdk/native/jni/OpenCVConfig.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__ANDROID__=1")

# compile flags
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -ggdb -Wno-error=format-security -O2"
)

# Enable c++11
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -std=c++11"
)

appendFiles(GLOB_RECURSE srcFiles "src/**.cpp")
appendFiles(GLOB_RECURSE srcFiles "src/**.c")


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             ${demoLibName}

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             # Associated headers in the same location as their source
             # file are automatically included.
             ${srcFiles})

# populate with various include directories.
target_include_directories(${demoLibName}
    PUBLIC
        # your include directories
        src
)

target_include_directories(${demoLibName}
    SYSTEM PUBLIC
        # include directories for libraries. After SYSTEM warnings in these includes will be
        # suppressed

        # MFEMakeupSDK
        ${mfeMakeupSDKDir}/include
)

# static libraries
appendFiles(GLOB staticLibs ${mfeMakeupSDKDir}/lib/${ANDROID_NDK_ABI_NAME}/*.a)
appendFiles(GLOB staticLibs ${dependenciesDir}/curl/lib/${ANDROID_NDK_ABI_NAME}/*.a)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in the
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries(
    # Specifies the target library.
   ${demoLibName} m log GLESv2 EGL jnigraphics android dl z

   # Links the target library to the log library
   # included in the NDK.
   ${staticLibs}

   ${OpenCV_LIBS}
)
