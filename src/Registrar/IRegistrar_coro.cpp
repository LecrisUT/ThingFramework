module;

// clang-format off
#include <memory>
// clang-format on
#include <cassert>
#include <coroutine>
#include <future>
#include <string>
#include <vector>

module ThingFramework.Registrar;
import :IRegistrar_h;
import :IRegistrar_coro;
import :Exceptions;
import ThingFramework.Utility;

using namespace ThingFramework::Registrar;

std::future<std::reference_wrapper<IExposable>> RegistrarPromise::AwaitGetRef(IRegistrarBase& registrar, std::string_view name) {
	co_await *registrar.awaiter;
	assert(registrar.Contains(name));
	auto& value = registrar.GetRef(name);
	co_return value;
}
std::future<std::shared_ptr<IExposable>> RegistrarPromise::AwaitGetPtr(IRegistrarBase& registrar, std::string_view name) {
	co_await *registrar.awaiter;
	assert(registrar.Contains(name));
	auto value = registrar.GetPtr(name);
	co_return value;
}
