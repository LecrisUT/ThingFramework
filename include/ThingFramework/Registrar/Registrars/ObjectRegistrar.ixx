module;

// clang-format off
#include <memory>
// clang-format on
#include <concepts>
#include <string_view>

export module ThingFramework.Registrar:ObjectRegistrar_h;
import :ObjectRegistrarRoot_h;
import :Standard;

export namespace ThingFramework::Registrar {
	/**
	 * Main object registrar template
	 *
	 * @tparam T Derived saved class
	 * @tparam U Base class
	 */
	template<class T, class U = T>
	class ObjectRegistrar
	    : public ObjectRegistrarRoot<T> {
		using base = ObjectRegistrarRoot<T>;

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
		ObjectRegistrarRoot<U>& Root;

		/**
		 * Default constructor
		 *
		 * @param root Objecterence to the base registrar
		 */
		explicit ObjectRegistrar(ObjectRegistrarRoot<U>& root) noexcept;
		;
		/**
		 * Special constructor for standard registrar
		 */
		ObjectRegistrar() noexcept
		    requires stdObjRegistrar<U>;
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
	 * Effectively alias to ObjectRegistrarRoot<T>
	 *
	 * @tparam T Type of the item
	 */
	template<class T>
	class ObjectRegistrar<T, T>
	    : public ObjectRegistrarRoot<T> {
		using base = ObjectRegistrarRoot<T>;

	public:
		using typename base::ref_type;
		using typename base::registrar_type;
		using typename base::value_type;
	};
}// namespace ThingFramework::Registrar
