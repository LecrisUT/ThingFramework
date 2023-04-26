target_sources(ThingFramework_Lib PUBLIC
		FILE_SET cxx_modules
		FILES
		Concepts.ixx
		Exceptions.ixx
		Utility.ixx
		Standard.ixx
		)
foreach (dir IN ITEMS
		DynamicLibrary
		)
	add_subdirectory(${dir})
endforeach ()
