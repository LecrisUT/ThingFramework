target_sources(ThingFramework_Lib PRIVATE
		Exceptions.cpp
		)
target_sources(ThingFramework_Lib PUBLIC
		FILE_SET cxx_modules
		FILES
		Concepts.cppm
		Standard.cppm
		)
foreach (dir IN ITEMS
		DynamicLibrary
		)
	add_subdirectory(${dir})
endforeach ()
