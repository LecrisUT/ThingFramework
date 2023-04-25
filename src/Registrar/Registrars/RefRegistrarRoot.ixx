module;

// clang-format off
#include <memory>
// clang-format on
#include <optional>
#include <set>
#include <unordered_map>

export module ThingFramework.Registrar:RefRegistrarRoot_h;
export import :IRegistrar;
import :Concepts;
import :Traits;

namespace ThingFramework::Registrar {
	/**
	 * Basic Registrar of shared objects
	 *
	 * @tparam T Type of the objects in registrar
	 */
	export template<class T>
	class RefRegistrarRoot
	    : public IRegistrar<T, RefRegistrarRoot<T>> {
		friend IRegistrar<T, RefRegistrarRoot<T>>;
		// Export useful aliases
		using base = IRegistrar<T, RefRegistrarRoot<T>>;

	public:
		using ref_type = std::reference_wrapper<T>;
		using typename base::registrar_type;
		using typename base::value_type;

	protected:
		using map_type = std::unordered_map<std::string, ref_type>;
		using set_type = std::set<ref_type>;

		// Internal fields
	protected:
		/**
		 * Dictionary of item name and shared pointer
		 */
		map_type map{};
		/**
		 * Set storing pointers to all items
		 */
		set_type set{};
	};

	/**
	 * Specialization of RefRegistrar Traits
	 * @tparam T
	 */
	template<class T>
	struct RegistrarTraits<RefRegistrarRoot<T>> {
		using registrar_type = RefRegistrarRoot<T>;
		using value_type = T;
		using ref_type = std::reference_wrapper<value_type>;
		using map_type = std::unordered_map<std::string, ref_type>;
		using set_type = std::set<ref_type>;
		static constexpr bool ref_registrar = true;
		static constexpr bool ptr_registrar = false;
	};
}// namespace ThingFramework::Registrar
