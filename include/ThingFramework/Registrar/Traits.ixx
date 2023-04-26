module;

#include <type_traits>

export module ThingFramework.Registrar:Traits;

export namespace ThingFramework::Registrar {
	/**
	 * Static traits for the Registrar type T
	 *
	 * @tparam T Registrar type
	 */
	template<class T>
	struct RegistrarTraits {
		using map_type = void;
		using set_type = void;
		using registrar_type = T;
		using value_type = void;
		static constexpr bool ref_registrar = false;
		static constexpr bool ptr_registrar = false;
	};
}// namespace ThingFramework::Registrar
