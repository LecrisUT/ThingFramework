target_sources(ThingFramework_Lib PUBLIC
		FILE_SET cxx_modules
		FILES
		Concepts.ixx
		Exceptions.ixx
		IRegistrar.ixx
		Registrar.ixx
		Standard.ixx
		Traits.ixx
		)
add_subdirectory(Registrars)
