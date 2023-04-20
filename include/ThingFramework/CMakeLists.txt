configure_file(Metadata.ixx.in Metadata.ixx @ONLY)

target_sources(ThingFramework_Lib PUBLIC
		FILE_SET cxx_modules
		FILES
		${CMAKE_CURRENT_BINARY_DIR}/Metadata.ixx
		)

foreach (dir IN ITEMS
		Interfaces
		Utility
		)
	add_subdirectory(${dir})
endforeach ()
