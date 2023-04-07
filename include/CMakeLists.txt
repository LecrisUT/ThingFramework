target_sources(ThingFramework_Lib PUBLIC
		FILE_SET cxx_modules
		TYPE CXX_MODULES
		BASE_DIRS
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_LIBDIR}>
		FILES
		ThingFramework.ixx
		)
add_subdirectory(ThingFramework)
