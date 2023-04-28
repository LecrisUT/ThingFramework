module;

// clang-format off
#include <memory>
// clang-format on
#include <optional>
#include <set>
#include <unordered_map>

export module ThingFramework.Registrar:SharedRegistrarRoot_h;
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
	class SharedRegistrarRoot
	    : public IRegistrar<T, SharedRegistrarRoot<T>> {
		friend IRegistrar<T, SharedRegistrarRoot<T>>;
		// Export useful aliases
		using base = IRegistrar<T, SharedRegistrarRoot<T>>;

	public:
		using traits = RegistrarTraits<SharedRegistrarRoot<T>>;
		using ptr_type = std::shared_ptr<T>;
		using typename base::registrar_type;
		using typename base::value_type;

	protected:
		using map_type = std::unordered_map<std::string_view, ptr_type>;
		using set_type = std::set<ptr_type>;

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
	 * Specialization of SharedRegistrar Traits
	 * @tparam T
	 */
	template<class T>
	struct RegistrarTraits<SharedRegistrarRoot<T>> {
		using registrar_type = SharedRegistrarRoot<T>;
		using value_type = T;
		using ptr_type = std::shared_ptr<value_type>;
		using map_type = std::unordered_map<std::string, ptr_type>;
		using set_type = std::set<ptr_type>;
		static constexpr bool ref_registrar = false;
		static constexpr bool ptr_registrar = true;
	};
}// namespace ThingFramework::Registrar
