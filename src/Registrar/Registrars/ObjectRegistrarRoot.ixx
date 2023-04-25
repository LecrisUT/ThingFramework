module;

// clang-format off
#include <memory>
// clang-format on
#include <optional>
#include <set>
#include <unordered_map>

export module ThingFramework.Registrar:ObjectRegistrarRoot_h;
export import :IRegistrar;
import :Concepts;
import :Traits;

namespace ThingFramework::Registrar {
	/**
	 * Basic Registrar of objects
	 *
	 * This registrar owns the objects in `set`
	 *
	 * @tparam T Type of the objects in registrar
	 */
	template<class T>
	class ObjectRegistrarRoot
	    : public IRegistrar<T, ObjectRegistrarRoot<T>> {
		friend IRegistrar<T, ObjectRegistrarRoot<T>>;
		// Export useful aliases
		using base = IRegistrar<T, ObjectRegistrarRoot<T>>;

	public:
		using ref_type = std::reference_wrapper<T>;
		using typename base::registrar_type;
		using typename base::value_type;

	protected:
		using map_type = std::unordered_map<std::string_view, ref_type>;
		using set_type = std::set<T>;

		//
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
	 * Specialization of ObjectRegistrar types
	 * @tparam T
	 */
	template<class T>
	struct RegistrarTraits<ObjectRegistrarRoot<T>> {
		using registrar_type = ObjectRegistrarRoot<T>;
		using value_type = T;
		using ref_type = std::reference_wrapper<value_type>;
		using map_type = std::unordered_map<std::string, ref_type>;
		using set_type = std::set<value_type>;
		static constexpr bool ref_registrar = true;
		static constexpr bool ptr_registrar = false;
	};
}// namespace ThingFramework::Registrar
