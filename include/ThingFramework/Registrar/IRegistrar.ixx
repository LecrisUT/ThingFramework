module;

// clang-format off
#include <memory>
// clang-format on
#include <coroutine>
#include <future>
#include <list>
#include <map>
#include <optional>
#include <string>
#include <vector>

export module ThingFramework.Registrar:IRegistrar_h;
import :Concepts;
import :Traits;

namespace ThingFramework {
	namespace Interfaces {
		struct IExposable;
	}
	namespace Registrar {
		struct RegistrarAwaiter;
		struct RegistrarPromise;
	}// namespace Registrar
}// namespace ThingFramework

using namespace ThingFramework::Interfaces;
namespace ThingFramework::Registrar {
	/**
	 * Registrar base interface
	 *
	 * @ingroup registrar
	 */
	class IRegistrarBase {
		friend RegistrarPromise;

	public:
		/**
		 * Try to resolve any registrations in the queue
		 */
		static void ResolveDanglingRegisters();

	protected:
		using queue_type = std::map<std::string, std::list<std::coroutine_handle<>>, std::less<>>;
		/**
		 * Collection of registrars.
		 *
		 * Members are purely managed by the constructor/destructor of IRegistrar.
		 */
		inline static std::list<IRegistrarBase*> registrars;
		/**
		 * Registration queue waiting on other items to be registered.
		 */
		queue_type registrationQueue{};
		/**
		 * Coroutine awaiter keeping track of
		 */
		std::unique_ptr<RegistrarAwaiter> awaiter;

		IRegistrarBase() noexcept;
		virtual ~IRegistrarBase();

	public:
		// Main interface that needs to be implemented downstream
		// Ill defined interfaces should be deleted downstream (e.g. TryGetPtr of RefRegistrar)
		/**
		 * Check if named item is in the registrar.
		 *
		 * @param str Name of the item
		 * @return Result of check
		 */
		virtual bool Contains(std::string_view str) const = 0;
		/**
		 * Try to get the named item from the registrar as a reference.
		 *
		 * @param str Name of the item
		 * @return Optional reference to the item
		 */
		virtual std::optional<std::reference_wrapper<IExposable>> TryGetRef(std::string_view str) const;
		/**
		 * Try to get the named item from the registrar as a shared pointer.
		 *
		 * @param str Name of the item
		 * @return Pointer to the item
		 */
		virtual std::shared_ptr<IExposable> TryGetPtr(std::string_view str) const;
		/**
		 * Try to register an item into the registrar.
		 *
		 * @param item Shared pointer to the item
		 * @return Registration result
		 */
		virtual bool TryRegister(std::shared_ptr<IExposable> item);
		/**
		 * Try to register an item into the registrar.
		 *
		 * @param item Reference to the item
		 * @return Registration result
		 */
		virtual bool TryRegister(IExposable&& item);
		/**
		 * Try to register a searchable name for the item.
		 *
		 * @param name Name of the item
		 * @param[in] item Item to register to
		 * @return
		 */
		virtual bool TryRegisterName(std::string_view name, const IExposable& item) = 0;
		/**
		 * Try to register a searchable name for the item.
		 *
		 * @param name Name of the item
		 * @param[in] item Item to register to
		 * @return
		 */
		virtual bool TryRegisterName(std::string_view name, std::shared_ptr<IExposable> item);
		/**
		 * Try to remove named item from the registrar.
		 *
		 * @param item Name of the item
		 * @return Remove result
		 */
		virtual bool TryRemove(std::string_view item) = 0;

		// Derived interface from the above
		/**
		 * Get the named item from the registrar as a reference.
		 *
		 * Throws if item is not found.
		 *
		 * @param str Name of the item
		 * @return Reference to the item
		 */
		IExposable& GetRef(std::string_view str) const;
		/**
		 * Get the named item from the registrar as a shared_ptr
		 *
		 * Throws if item is not found.
		 *
		 * @param str Name of the item
		 * @return Pointer to the item
		 */
		std::shared_ptr<IExposable> GetPtr(std::string_view str) const;
		/**
		 * Register an item to the registrar.
		 *
		 * Throws if unsuccessful.
		 *
		 * @param item Shared pointer to the item
		 */
		void Register(std::shared_ptr<IExposable> item);
		/**
		 * Register an item to the registrar.
		 *
		 * Throws if unsuccessful.
		 *
		 * @param item Shared pointer to the item
		 */
		void Register(IExposable&& item);
		/**
		 * Register a searchable name for the item.
		 *
		 * Throws if unsuccessful.
		 *
		 * @param name Name of the item
		 * @param[in] item Item to register to
		 */
		void RegisterName(std::string_view name, const IExposable& item);
		/**
		 * Register a searchable name for the item.
		 *
		 * Throws if unsuccessful.
		 *
		 * @param name Name of the item
		 * @param[in] item Item to register to
		 */
		void RegisterName(std::string_view name, std::shared_ptr<IExposable> item);

		/**
		 * Check registration queue for all items
		 */
		void ResolvePostRegister();
		/**
		 * Check registration queue after an item is successfully added.
		 *
		 * @param str Name of the item registered
		 */
		void ResolvePostRegister(std::string_view str);
		/**
		 * Get the named Exposable object and place it in an appropriate container
		 *
		 * Throws exception if not found.
		 *
		 * @param name Name of the object
		 * @param[out] value Reference where to place the item
		 */
		void Get(std::string_view name, std::reference_wrapper<IExposable>& value);
		/// @overload
		void Get(std::string_view name, std::vector<std::reference_wrapper<IExposable>>& value);
		/// @overload
		void Get(std::string_view name, std::shared_ptr<IExposable>& value);
		/// @overload
		void Get(std::string_view name, std::vector<std::shared_ptr<IExposable>>& value);
		/**
		 * Get a placeholder of the named object
		 *
		 * Object is populated after it is successfully registered.
		 *
		 * @param name Name of the object
		 * @param[out] value Reference where to place the future value
		 */
		void Get(std::string_view name, std::future<std::reference_wrapper<IExposable>>& value);
		/// @overload
		void Get(std::string_view name, std::vector<std::future<std::reference_wrapper<IExposable>>>& value);
		/// @overload
		void Get(std::string_view name, std::future<std::shared_ptr<IExposable>>& value);
		/// @overload
		void Get(std::string_view name, std::vector<std::future<std::shared_ptr<IExposable>>>& value);
		/**
		 * Coroutine operator providing the awaiter object
		 */
		RegistrarAwaiter& operator co_await() const;
	};
	/**
	 * Main registrar interface
	 *
	 * Uses Curiously Recurring Template Pattern via R type
	 *
	 * @tparam T Type of the objects in registrar
	 * @tparam R Registrar type
	 */
	template<class T, class R>
	// Note: Cannot use requires because of recursion. These are moved to constructor
	class IRegistrar
	    : public IRegistrarBase {
		// Export useful aliases
	public:
		using value_type = T;
		using registrar_type = R;
		using traits = RegistrarTraits<R>;

	protected:
		IRegistrar() noexcept;


		// Export the base definitions that will be reused
	public:
		using IRegistrarBase::Contains;

		// Define additional typed interfaces
		virtual bool Contains(const T& item) const;
		bool Contains(std::shared_ptr<T> item) const
		    requires R::traits::ptr_registrar;

		// Default definitions using map and set typed name
		// Delete these in actual implementation if incompatible
		bool Contains(std::string_view str) const override;
		bool TryRegister(std::shared_ptr<IExposable> item) override;
		bool TryRegister(IExposable&& item) override;
		bool TryRegisterName(std::string_view name, const IExposable& item) override;
		bool TryRegisterName(std::string_view name, std::shared_ptr<IExposable> item) override;
		bool TryRemove(std::string_view item) override;
		std::optional<std::reference_wrapper<IExposable>> TryGetRef(std::string_view str) const override;
		std::shared_ptr<IExposable> TryGetPtr(std::string_view str) const override;
	};
}// namespace ThingFramework::Registrar
