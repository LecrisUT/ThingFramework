target_sources(ThingFramework_Lib PRIVATE
		IExposable.cpp
		)
target_sources(ThingFramework_Lib PUBLIC
		FILE_SET cxx_modules
		FILES
		Concepts.cppm
		)
