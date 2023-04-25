module;

// clang-format off
#include <memory>
// clang-format on
#include <concepts>
#include <string_view>

export module ThingFramework.Registrar:RefRegistrar_h;
import :RefRegistrarRoot_h;
import :Standard;

export namespace ThingFramework::Registrar {
	/**
	 * Main shared registrar template
	 *
	 * @tparam T Derived saved class
	 * @tparam U Base class
	 */
	template<class T, class U = T>
	class RefRegistrar
	    : public RefRegistrarRoot<T> {
		using base = RefRegistrarRoot<T>;

	public:
		using base_type = U;
		using base::RegisterName;
		using typename base::ref_type;
		using typename base::value_type;

		// Inherit interface of the base
	public:
		using base::TryRegister;
		using base::TryRegisterName;

		/**
		 * Base registrar with base type pointers
		 */
		RefRegistrarRoot<U>& Root;

		/**
		 * Default constructor
		 *
		 * @param root Reference to the base registrar
		 */
		explicit RefRegistrar(RefRegistrarRoot<U>& root) noexcept;
		;
		/**
		 * Special constructor for standard registrar
		 */
		RefRegistrar() noexcept
		    requires stdRefRegistrar<U>;
		;

		// Overrides to call the base root definitions as well
		bool TryRegister(IExposable&& item) override;
		bool TryRegisterName(std::string_view name, const IExposable& item) override;
		bool TryRemove(std::string_view item) override;
		void ResolvePostRegister(std::string_view str) override;
	};
	/**
	 * Main shared registrar template
	 *
	 * Effectively alias to RefRegistrarRoot<T>
	 *
	 * @tparam T Type of the item
	 */
	template<class T>
	class RefRegistrar<T, T>
	    : public RefRegistrarRoot<T> {
		using base = RefRegistrarRoot<T>;

	public:
		using typename base::ref_type;
		using typename base::registrar_type;
		using typename base::value_type;
	};
}// namespace ThingFramework::Registrar
