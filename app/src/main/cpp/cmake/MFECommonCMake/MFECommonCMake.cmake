####################################
# Usefull functions

# Will append files to listOutput
#
# @param globType      either GLOB or GLOB_RECURSE
# @param listOutput    The output variable to append to
# @param expression    The expression for searching files
function(appendFiles globType listOutput expression)
    file(${globType} files "${expression}")
    list(APPEND ${listOutput} ${files})
    set(${listOutput} "${${listOutput}}" PARENT_SCOPE)
endfunction()


# scan for directories recursively
#
# @param output the output variable which will be populated with a list of directories
# @param dir    The starting top level directory to be scanned.
function(listDirectoriesRecursively output dir)
    file(GLOB_RECURSE list "${dir}/*")
    set(headerList "")
    foreach(path ${list})
        get_filename_component(headerDir ${path} PATH)
        list(APPEND headerList ${headerDir})
    endforeach()
    list(APPEND headerList "${dir}")
    list(REMOVE_DUPLICATES headerList)
    set(${output} "${headerList}" PARENT_SCOPE)
endfunction()


# sets up ccache if possible. If not found does nothing
# ccache helps compile faster. I don't know if we should have it
# as default.
function(setupCcacheIfPossible)
	if(${CCACHE_SETUP_SUCCESS})
		return()
	endif()
    set(logFile ${CMAKE_CURRENT_BINARY_DIR}/ccache.log)
    set(ENV{CCACHE_LOGFILE} ${logFile})
    find_program(CCACHE_COMMAND ccache PATHS /usr/local/bin)
    if(EXISTS ${CCACHE_COMMAND})
        set(CCACHE_WRAPPER ${CMAKE_CURRENT_BINARY_DIR}/ccache-wrapper)
        file(WRITE ${CCACHE_WRAPPER}
"#!/usr/bin/env bash
export CCACHE_LOGFILE=${logFile}
${CCACHE_COMMAND} \"\$@\"
")
        execute_process(COMMAND chmod a+x ${CCACHE_WRAPPER})

        set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_WRAPPER} PARENT_SCOPE)
        set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_WRAPPER} PARENT_SCOPE)
		set(CCACHE_SETUP_SUCCES YES PARENT_SCOPE)
    endif()
    message(STATUS "ccache ... ${CCACHE_COMMAND} log = ${logFile}")
endfunction()


# if file doesn't exist then gives a message FATAL_ERROR.
#
# @param file   The file to check if it exists
function(assertFileExists file)
    if(NOT EXISTS ${file})
        message(FATAL_ERROR "file not exist: ${file}")
    endif()
endfunction()

####################################
# Import libs helper

# imports a dynamic shared library and adds it to be included into AAR or APK.
#
# to use this add add_dependencies(yourTarget importedLibs)
# @param name           the name to give. e.g. someLib, then in target_link_libraries you can add
#                       someLib.
# @param sharedFile     The path to the dynamic library
function(importSharedLibrary name sharedFile)
    if(NOT TARGET importedLibs)
        # created necessary targets for the first invocation
        add_custom_target(importedLibs)
        add_custom_command(OUTPUT "${ANDROID_ABI}_mkdir"
            COMMAND mkdir -p "${CMAKE_BINARY_DIR}/../obj/${ANDROID_ABI}"
            COMMAND touch "${ANDROID_ABI}_mkdir"
        )
        add_custom_target(abiDirTarget
            DEPENDS "${ANDROID_ABI}_mkdir"
        )
    endif()
    if(NOT EXISTS ${sharedFile})
        message(FATAL_ERROR "file not exist ${sharedFile}")
    endif()
    add_library(${name} SHARED IMPORTED)
    set_target_properties(${name} PROPERTIES IMPORTED_LOCATION "${sharedFile}")

    get_filename_component(fileName "${sharedFile}" NAME)
    set(outputDir "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
    get_filename_component(outputDir "${outputDir}" ABSOLUTE)

    add_custom_command(OUTPUT "${outputDir}/${fileName}"
        COMMAND cp "${sharedFile}" "${outputDir}"
        DEPENDS abiDirTarget
    )
    add_custom_target(${name}CopyTarget DEPENDS "${outputDir}/${fileName}")
    add_dependencies(importedLibs ${name}CopyTarget)
endfunction()



# Runs cocoapod
#
# @param pod_dir    	location to execute the pod command
# @param command      	command to run with pod
function(cmakePod pod_dir command)
	find_program( pod_cmd_path pod PATHS ENV{PATH})
	find_program( pod_cmd_bin pod /usr/local/bin )

	if(pod_cmd_path)
		execute_process(
			COMMAND pod ${command} WORKING_DIRECTORY ${pod_dir}
		)
	elseif(pod_cmd_bin)
		set( ENV{PATH} "$ENV{PATH}:/usr/local/bin" )
		execute_process(
			COMMAND pod ${command} WORKING_DIRECTORY ${pod_dir}
		)
	else()
		message(FATAL_ERROR "pod command not found!")
    endif()

endfunction()





####################################
# Common for all projects setup

function(mfeCommonInit)
    if(${MFE_COMMON_INIT})
        return()
    endif()
    set(CMAKE_CXX_STANDARD 11 PARENT_SCOPE)
    set(CMAKE_FIND_ROOT_PATH "${PROJECT_SOURCE_DIR};${CMAKE_FIND_ROOT_PATH}" PARENT_SCOPE)

    # common flags
    set(cAndCppFlags "-Wall -Wextra -Wno-unknown-pragmas -Wno-unused-parameter -fvisibility=hidden")
    set(cAndCppFlags "${cAndCppFlags} -Wfloat-equal")

    # for removing unused functions and etc.. in c++
    set(cAndCppFlags "${cAndCppFlags} -ffunction-sections -fdata-sections")

    # All 3 just to be complete.
    # If other languages are involved then target properties will need to be set.
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections" PARENT_SCOPE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections" PARENT_SCOPE)

    # Tell compiler to treat these warnings as errors
    set(cppErrors
        "return-type" "init-self" "uninitialized" maybe-uninitialized "bad-function-cast"
        parentheses sequence-point return-local-addr address
    )
    foreach(error ${cppErrors})
        list(APPEND cAndCppFlags -Werror=${error})
    endforeach()

    # build specific compiler flags
    if(CMAKE_BUILD_TYPE STREQUAL Debug)
        list(APPEND cAndCppFlags -DDEBUG=1)
    else()
        list(APPEND cAndCppFlags -DRELEASE=1)
    endif()

    foreach(option ${cAndCppFlags})
        string(FIND "${CMAKE_CXX_FLAGS}" ${option} found)
        if(${found} EQUAL -1)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${option}")
        endif()
        string(FIND "${CMAKE_C_FLAGS}" ${option} found)
        if(${found} EQUAL -1)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${option}")
        endif()
    endforeach()

    set(flagsToRemove -fno-rtti -fno-exceptions)
    foreach(toRemove ${flagsToRemove})
        string(REPLACE ${toRemove} "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    endforeach()

    # fix androids non-standard setup
    foreach(option -frtti -fexceptions -std=c++14)
        string(FIND "${CMAKE_CXX_FLAGS}" ${option} found)
        if(${found} EQUAL -1)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${option}")
        endif()
    endforeach()

    # show it for parent scope
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" PARENT_SCOPE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" PARENT_SCOPE)
    set(MFE_COMMON_INIT YES)
endfunction()

if(EXISTS ${PROJECT_SOURCE_DIR})
    mfeCommonInit()
endif()


# find_package(Git) creates these variables

# GIT_EXECUTABLE        - path to git command line client
# GIT_FOUND             - true if the command line client was found
# GIT_VERSION_STRING    - the version of git found (since CMake 2.8.8)

find_package(Git)

function(getGitTagVersion output)
    #
    # Find the project version based on the latest Git tag for this comment
    #

    # Make sure this machine has the Git command
    if (NOT GIT_FOUND)
        message( FATAL_ERROR "Git is required. Please install it for your platform and add it to the search paths if needed." )
    endif()

    # Use Git describe to get the recent tag
    execute_process(COMMAND ${GIT_EXECUTABLE} "describe" "--abbrev=0" "--tags" OUTPUT_VARIABLE GIT_DESCRIBE_RESULT WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

    # Remove any trailing spaces that the command returns
    string(STRIP ${GIT_DESCRIBE_RESULT} GIT_DESCRIBE_RESULT)

    # Make sure the tag is in proper version format
    # Assumes the format is MAJOR.MINOR or MAJOR.MINOR.PATCH
    if( GIT_DESCRIBE_RESULT MATCHES "^[0-9]+\\.[0-9]+(\\.[0-9]+)?$" )
        message( STATUS "Got the following version Git tag: \"${GIT_DESCRIBE_RESULT}\"" )
    else()
        message( FATAL_ERROR "Error, Git tag is not in a valid version format: \"${GIT_DESCRIBE_RESULT}\"" )
    endif()
    
    set(${output} ${GIT_DESCRIBE_RESULT} PARENT_SCOPE)
endfunction()
