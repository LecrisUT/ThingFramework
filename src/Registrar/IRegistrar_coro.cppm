module;

// clang-format off
#include <memory>
// clang-format on
#include <coroutine>
#include <future>
#include <list>
#include <map>
#include <string>

module ThingFramework.Registrar:IRegistrar_coro;
import :IRegistrar_h;

namespace ThingFramework {
	namespace Interfaces {
		class IExposable;
	}
}// namespace ThingFramework

using namespace ThingFramework::Interfaces;
namespace ThingFramework::Registrar {
	/**
	 * Registrar promise type.
	 *
	 * Keeps track of the name of the item being requestsed.
	 * Used to sanitize, update final value of std::future, and return the initial std::future in co_return.
	 */
	struct RegistrarPromise {
		IRegistrarBase& registrar;
		/**
		 * Name of the item requested
		 */
		const std::string name;
		RegistrarPromise(IRegistrarBase& registrar, std::string_view name)
		    : registrar{registrar}, name{name} {}
		virtual std::coroutine_handle<> GetHandle() = 0;
		/**
		 * Initial function run before entering coroutine body
		 *
		 * Used to automatically register the coroutine_handle to the Registrar queue
		 * @return Do not wait after this call
		 */
		std::suspend_never initial_suspend() noexcept {
			registrar.registrationQueue[name].push_back(GetHandle());
			return {};
		}
		/**
		 * Final function run after co_return finished (inside the coroutine body) or
		 * when there was an exception.
		 *
		 * Used to automatically de-register the coroutine_handle from the Registrar queue
		 * @return Do not wait after this call
		 */
		std::suspend_never final_suspend() noexcept {
			auto queue = registrar.registrationQueue.find(name);
			if (queue != registrar.registrationQueue.end()) {
				queue->second.remove(GetHandle());
			}
			return {};
		}
		/**
		 * Exception handler
		 *
		 * Used to pass exception to actual promise item
		 */
		virtual void unhandled_exception() {}
		// Implemented coroutine functions
		static std::future<std::reference_wrapper<IExposable>> AwaitGetRef(IRegistrarBase& registrar, std::string_view name);
		static std::future<std::shared_ptr<IExposable>> AwaitGetPtr(IRegistrarBase& registrar, std::string_view name);
	};
	/**
	 * Registrar awaiter. Keeps track of registrar queries.
	 *
	 * Handles when a co_await should continue.
	 */
	struct RegistrarAwaiter {
		/**
		 * Registrar where to search through
		 */
		IRegistrarBase& registrar;
		/**
		 * Check if queried item is available or not.
		 *
		 * If not yet available instruct to co_await. Run once at initialization.
		 *
		 * Used to register the coroutine to IRegistrar.queue.
		 *
		 * @return Whether should co_await or not
		 */
		bool await_ready() const { return false; };
		/**
		 * When coroutine_handle.resume() is called, check if can run the co_await.
		 *
		 * Used to check when queried item has become available.
		 *
		 * @param promise Registrar promise containing the requested item
		 * @return Whether should continue to co_await or not
		 */
		template<class T>
		    requires std::derived_from<T, RegistrarPromise>
		bool await_suspend(std::coroutine_handle<T> handle) const {
			if (registrar.Contains(handle.promise().name)) {
				return false;
			}
			return true;
		}
		/**
		 * Function run after await_suspend to return the item from co_await.
		 *
		 * Not used to do anything here.
		 */
		void await_resume() const {
		}

		RegistrarAwaiter(IRegistrarBase& registrar)
		    : registrar{registrar} {};
	};
}// namespace ThingFramework::Registrar

using namespace ThingFramework;
using namespace ThingFramework::Registrar;

template<class... TArgs>
struct std::coroutine_traits<std::future<std::shared_ptr<IExposable>>, TArgs...> {
	struct promise_type
	    : std::promise<std::shared_ptr<IExposable>>,
	      RegistrarPromise {
		std::future<std::shared_ptr<IExposable>> get_return_object() { return this->get_future(); }
		void return_value(const std::shared_ptr<IExposable>& value) { this->set_value(value); }
		template<class... Args>
		promise_type(IRegistrarBase& registrar, std::string_view name, Args&&...)
		    : RegistrarPromise{registrar, name} {};
		std::coroutine_handle<> GetHandle() override {
			return std::coroutine_handle<promise_type>::from_promise(*this);
		}
		void unhandled_exception() override {
			this->set_exception(std::current_exception());
		}
	};
};
template<class... TArgs>
struct std::coroutine_traits<std::future<std::reference_wrapper<IExposable>>, TArgs...> {
	struct promise_type
	    : std::promise<std::reference_wrapper<IExposable>>,
	      RegistrarPromise {
		std::future<std::reference_wrapper<IExposable>> get_return_object() { return this->get_future(); }
		void return_value(IExposable& value) { this->set_value(value); }
		template<class... Args>
		promise_type(IRegistrarBase& registrar, std::string_view name, Args&&...)
		    : RegistrarPromise{registrar, name} {};
		std::coroutine_handle<> GetHandle() override {
			return std::coroutine_handle<promise_type>::from_promise(*this);
		}
		void unhandled_exception() override {
			this->set_exception(std::current_exception());
		}
	};
};


//namespace ThingFramework {
//	namespace Internal {
//		template<class T>
//		struct RegistrarKeyMap {
//			using map_type = std::map<std::reference_wrapper<const T>, std::list<std::coroutine_handle<>>>;
//			map_type map;
//		};
//		template<class T>
//		    requires std::copy_constructible<T> || std::move_constructible<T>
//		struct RegistrarKeyMap<T> {
//			using map_type = std::map<T, std::list<std::coroutine_handle<>>>;
//			map_type map;
//		};
//		template<class T>
//		struct RegistrarKeyHolder {
//			using has_key_copy = std::false_type;
//			const T& keyRef;
//			explicit RegistrarKeyHolder(const T& key) : keyRef{key} {}
//		};
//		template<class T>
//		    requires std::move_constructible<T> && std::copy_constructible<T>
//		struct RegistrarKeyHolder<T> {
//			using has_key_copy = std::true_type;
//			T key;
//			const T& keyRef;
//			explicit RegistrarKeyHolder(const T& key) : key{key}, keyRef(this->key) {}
//			explicit RegistrarKeyHolder(T&& key) : key{std::move(key)}, keyRef(this->key) {}
//		};
//		template<class T>
//		    requires std::move_constructible<T> && (!std::copy_constructible<T>)
//		struct RegistrarKeyHolder<T> {
//			using has_key_copy = std::true_type;
//			T key;
//			const T& keyRef;
//			explicit RegistrarKeyHolder(const T& key) = delete;
//			explicit RegistrarKeyHolder(T&& key) : key{key}, keyRef(this->key) {}
//		};
//	}// namespace Internal
//}// namespace ThingFramework
