target_sources(ThingFramework_Lib PRIVATE
		Exceptions.cpp
		IRegistrar.cpp
		IRegistrar_coro.cpp
		)
target_sources(ThingFramework_Lib PUBLIC
		FILE_SET cxx_modules
		FILES
		Concepts.cppm
		IRegistrar.cppm
		IRegistrar_coro.cppm
		)
add_subdirectory(Registrars)
