module;

// clang-format off
#include <memory>
// clang-format on
#include <concepts>
#include <string_view>

export module ThingFramework.Registrar:SharedRegistrar_h;
import :SharedRegistrarRoot_h;
import :Standard;

export namespace ThingFramework::Registrar {
	/**
	 * Main shared registrar template
	 *
	 * @tparam T Derived saved class
	 * @tparam U Base class
	 */
	template<class T, class U = T>
	class SharedRegistrar
	    : public SharedRegistrarRoot<T> {
		using base = SharedRegistrarRoot<T>;

	public:
		using base_type = U;
		using base::RegisterName;
		using typename base::ptr_type;
		using typename base::value_type;

		// Inherit interface of the base
	public:
		using base::TryRegister;
		using base::TryRegisterName;

		/**
		 * Base registrar with base type pointers
		 */
		SharedRegistrarRoot<U>& Root;

		/**
		 * Default constructor
		 *
		 * @param root Reference to the base registrar
		 */
		explicit SharedRegistrar(SharedRegistrarRoot<U>& root) noexcept;
		;
		/**
		 * Special constructor for standard registrar
		 */
		SharedRegistrar() noexcept
		    requires stdSharedRegistrar<U>;
		;

		// Overrides to call the base root definitions as well
		bool TryRegister(std::shared_ptr<IExposable> item) override;
		bool TryRegisterName(std::string_view name, const IExposable& item) override;
		bool TryRegisterName(std::string_view name, std::shared_ptr<IExposable> item) override;
		bool TryRemove(std::string_view item) override;
		void ResolvePostRegister(std::string_view str) override;
	};
	/**
	 * Main shared registrar template
	 *
	 * Effectively alias to SharedRegistrarRoot<T>
	 *
	 * @tparam T Type of the item
	 */
	template<class T>
	class SharedRegistrar<T, T>
	    : public SharedRegistrarRoot<T> {
		using base = SharedRegistrarRoot<T>;

	public:
		using typename base::ptr_type;
		using typename base::registrar_type;
		using typename base::value_type;
	};
}// namespace ThingFramework::Registrar
