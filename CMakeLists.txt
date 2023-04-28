cmake_minimum_required(VERSION 3.22)
if (POLICY CMP0140)
	cmake_policy(SET CMP0140 NEW)
endif ()

### Project preparations ###
# Get DynamicVersion utility
include(FetchContent)
FetchContent_Declare(CMakeExtraUtils
		GIT_REPOSITORY https://github.com/LecrisUT/CMakeExtraUtils
		GIT_TAG v0.3.0
		FIND_PACKAGE_ARGS CONFIG)
FetchContent_MakeAvailable(CMakeExtraUtils)
include(DynamicVersion)
dynamic_version(PROJECT_PREFIX ThingFramework_)

### Begin project definition ###
project(ThingFramework
		VERSION ${PROJECT_VERSION}
		LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

## Define options
option(THINGFRAMEWORK_INSTALL "ThingFramework: Install project files" ${PROJECT_IS_TOP_LEVEL})
option(THINGFRAMEWORK_TEST "ThingFramework: Build unit tests" ${PROJECT_IS_TOP_LEVEL})
option(THINGFRAMEWORK_SHARED_LIBS "ThingFramework: Build as a shared library" ${PROJECT_IS_TOP_LEVEL})
option(THINGFRAMEWORK_DOCS "ThingFramework: Build documentations" OFF)
option(THINGFRAMEWORK_TEST_COVERAGE "ThingFramework: Test with coverage" OFF)
mark_as_advanced(THINGFRAMEWORK_TEST_COVERAGE)

## Configure cmake modules and basic defaults
# Configure package installation
if (THINGFRAMEWORK_INSTALL)
	# Basic cmake packaging tools
	include(CMakePackageConfigHelpers)
	include(GNUInstallDirs)
	# Component exporting tool from CMakeExtraUtils
	include(PackageComps)
endif ()
# Default to Release build type
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif ()
# Set library type
set(BUILD_SHARED_LIBS ${THINGFRAMEWORK_SHARED_LIBS})
if (BUILD_SHARED_LIBS)
	set(ThingFramework_LIB_TYPE "shared")
else ()
	set(ThingFramework_LIB_TYPE "static")
endif ()
# Set test coverage flags
if (THINGFRAMEWORK_TEST_COVERAGE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif ()

## C++20 modules
# TODO: Remove when no longer experimental
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API 2182bf5c-ef0d-489a-91da-49dbc3090d2a)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP TRUE)
if (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
	message(WARNING
			"ThingFramework: Compiler not supported: ${CMAKE_CXX_COMPILER_ID}")
	set(CMAKE_EXPERIMENTAL_CXX_MODULE_MAP_FORMAT gcc)
	set(CMAKE_EXPERIMENTAL_CXX_MODULE_MAP_FLAG
			"${compiler_flags_for_module_map} -fmodule-mapper=<MODULE_MAP_FILE>")
	string(CONCAT CMAKE_EXPERIMENTAL_CXX_SCANDEP_SOURCE
			"<CMAKE_CXX_COMPILER> <DEFINES> <INCLUDES> <FLAGS> <SOURCE>"
			" -MT <DYNDEP_FILE> -MD -MF <DEP_FILE>"
			" ${flags_to_scan_deps} -fdep-file=<DYNDEP_FILE> -fdep-output=<OBJECT>"
			)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
	set(CMAKE_CXX_EXTENSIONS OFF)
	set(CMAKE_EXPERIMENTAL_CXX_MODULE_MAP_FORMAT clang)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL IntelLLVM)
	message(WARNING "Intel compiler not tested")
	set(CMAKE_EXPERIMENTAL_CXX_MODULE_MAP_FORMAT clang)
else ()
	message(WARNING
			"ThingFramework: Compiler not supported: ${CMAKE_CXX_COMPILER_ID}")
endif ()

## External dependencies
set(external_deps "")
# Project dependencies
FetchContent_Declare(nlohmann_json
		GIT_REPOSITORY https://github.com/nlohmann/json.git
		GIT_TAG v3.11.2
		FIND_PACKAGE_ARGS CONFIG)
list(APPEND external_deps nlohmann_json)
# Testing framework
if (THINGFRAMEWORK_TEST)
	FetchContent_Declare(Catch2
			GIT_REPOSITORY https://github.com/catchorg/Catch2.git
			GIT_TAG v3.3.2
			FIND_PACKAGE_ARGS CONFIG)
	list(APPEND external_deps Catch2)
endif ()
# Documentation builder
if (THINGFRAMEWORK_DOCS)
	find_package(Doxygen REQUIRED)
endif ()
# Get all external dependencies
FetchContent_MakeAvailable(${external_deps})

### Main CMake configuration ###
## Main exported targets
add_library(ThingFramework_Lib)
set_target_properties(ThingFramework_Lib PROPERTIES
		VERSION ${PROJECT_VERSION}
		# TODO: Currently unstable api, change when v1.0 is released
		SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
		EXPORT_NAME thing_framework
		OUTPUT_NAME ThingFramework)
add_library(ThingFramework::ThingFramework ALIAS ThingFramework_Lib)

# Public interface module units are in include
add_subdirectory(include)
# Main implementation is in src
add_subdirectory(src)

## Testing
if (THINGFRAMEWORK_TEST)
	enable_testing()
	add_subdirectory(test)
endif ()

## Documentation builder
if (THINGFRAMEWORK_DOCS)
	add_subdirectory(doc)
endif ()


### Export project ###
# Install package files
if (THINGFRAMEWORK_INSTALL)
	# Package cmake export files
	write_basic_package_version_file(
			ThingFrameworkConfigVersion.cmake
			VERSION ${PROJECT_VERSION}
			# TODO: Currently unstable api, change when v1.0 is released
			COMPATIBILITY SameMinorVersion
	)
	configure_package_config_file(
			cmake/ThingFrameworkConfig.cmake.in
			ThingFrameworkConfig.cmake
			INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ThingFramework
	)
	install(FILES
			${CMAKE_CURRENT_BINARY_DIR}/ThingFrameworkConfigVersion.cmake
			${CMAKE_CURRENT_BINARY_DIR}/ThingFrameworkConfig.cmake
			DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ThingFramework
			)
endif ()

# Make project available for FetchContent
if (NOT PROJECT_IS_TOP_LEVEL)
	# Propagate variables for FetchContent
	# All variables have to be consistent with CMakeExtraUtilsConfig.cmake
	if (CMAKE_VERSION VERSION_LESS 3.25)
		# TODO: Remove when cmake 3.25 is commonly distributed
		set(ThingFramework_VERSION ${ThingFramework_VERSION} PARENT_SCOPE)
		set(ThingFramework_VERSION_MAJOR ${ThingFramework_VERSION_MAJOR} PARENT_SCOPE)
		set(ThingFramework_VERSION_MINOR ${ThingFramework_VERSION_MINOR} PARENT_SCOPE)
		set(ThingFramework_VERSION_PATCH ${ThingFramework_VERSION_PATCH} PARENT_SCOPE)
		set(ThingFramework_VERSION_TWEAK ${ThingFramework_VERSION_TWEAK} PARENT_SCOPE)
	endif ()
	return(PROPAGATE
			ThingFramework_VERSION
			ThingFramework_VERSION_MAJOR
			ThingFramework_VERSION_MINOR
			ThingFramework_VERSION_PATCH
			ThingFramework_VERSION_TWEAK
			)
endif ()
