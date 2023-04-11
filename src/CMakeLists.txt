target_sources(ThingFramework_Lib PUBLIC
		FILE_SET cxx_modules
		BASE_DIRS
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		)

foreach (dir IN ITEMS
		Utility
		)
	add_subdirectory(${dir})
endforeach ()
