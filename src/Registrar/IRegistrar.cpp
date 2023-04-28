module;

// clang-format off
#include <memory>
// clang-format on
#include <coroutine>
#include <future>
#include <optional>
#include <string>
#include <utility>
#include <vector>

module ThingFramework.Registrar;
import :IRegistrar_h;
import :IRegistrar_coro;
import :Exceptions;
import ThingFramework.Utility;
import ThingFramework.Interfaces;

using namespace ThingFramework;
using namespace ThingFramework::Registrar;
using namespace ThingFramework::Interfaces;

void IRegistrarBase::ResolveDanglingRegisters() {
	for (auto& registrar: registrars)
		registrar->ResolvePostRegister();
}

IRegistrarBase::~IRegistrarBase() {
	registrars.remove(this);
}
IRegistrarBase::IRegistrarBase() noexcept
    : awaiter{std::make_unique<RegistrarAwaiter>(*this)} {
	registrars.push_back(this);
}

// Stubs
std::shared_ptr<IExposable> IRegistrarBase::TryGetPtr(std::string_view str) const {
	throw NotImplemented("TryGetPtr");
}
std::optional<std::reference_wrapper<IExposable>> IRegistrarBase::TryGetRef(std::string_view str) const {
	throw NotImplemented("TryGetRef");
}
bool IRegistrarBase::TryRegister(std::shared_ptr<IExposable> item) {
	throw NotImplemented("TryRegister");
}
bool IRegistrarBase::TryRegister(IExposable&& item) {
	throw NotImplemented("TryRegister");
}
bool IRegistrarBase::TryRegisterName(std::string_view name, std::shared_ptr<IExposable> item) {
	throw NotImplemented("TryRegisterName");
}

// Derived interfaces
IExposable& IRegistrarBase::GetRef(std::string_view str) const {
	// Try to find the item
	auto val = TryGetRef(str);
	if (!val.has_value())
		// If failed to find throw exception
		throw NotFound(*this, str);
	return val.value();
}
std::shared_ptr<IExposable> IRegistrarBase::GetPtr(std::string_view str) const {
	// Try to find the item
	auto val = TryGetPtr(str);
	if (val == nullptr)
		// If failed to find throw exception
		throw NotFound(*this, str);
	return val;
}
void IRegistrarBase::Register(std::shared_ptr<IExposable> item) {
	if (!TryRegister(std::move(item)))
		throw RegistrationFailed(*this);
}
void IRegistrarBase::Register(IExposable&& item) {
	if (!TryRegister(std::forward<IExposable>(item)))
		throw RegistrationFailed(*this);
}
void IRegistrarBase::RegisterName(std::string_view name, const IExposable& item) {
	if (!TryRegisterName(name, item))
		throw RegistrationFailed(*this);
}
void IRegistrarBase::RegisterName(std::string_view name, std::shared_ptr<IExposable> item) {
	if (!TryRegisterName(name, std::move(item)))
		throw RegistrationFailed(*this);
}

void IRegistrarBase::Get(std::string_view name, std::reference_wrapper<IExposable>& value) {
	value = GetRef(name);
}
void IRegistrarBase::Get(std::string_view name, std::vector<std::reference_wrapper<IExposable>>& value) {
	value.emplace_back(GetRef(name));
}
void IRegistrarBase::Get(std::string_view name, std::shared_ptr<IExposable>& value) {
	value = GetPtr(name);
}
void IRegistrarBase::Get(std::string_view name, std::vector<std::shared_ptr<IExposable>>& value) {
	value.emplace_back(GetPtr(name));
}
void IRegistrarBase::Get(std::string_view name, std::future<std::reference_wrapper<IExposable>>& value) {
	value = RegistrarPromise::AwaitGetRef(*this, name);
}
void IRegistrarBase::Get(std::string_view name, std::vector<std::future<std::reference_wrapper<IExposable>>>& value) {
	value.emplace_back(RegistrarPromise::AwaitGetRef(*this, name));
}
void IRegistrarBase::Get(std::string_view name, std::future<std::shared_ptr<IExposable>>& value) {
	value = RegistrarPromise::AwaitGetPtr(*this, name);
}
void IRegistrarBase::Get(std::string_view name, std::vector<std::future<std::shared_ptr<IExposable>>>& value) {
	value.emplace_back(RegistrarPromise::AwaitGetPtr(*this, name));
}
void IRegistrarBase::ResolvePostRegister() {
	for (auto& queue: registrationQueue) {
		// Not necessary
		//
		//	// Check if the item was succesfully registered
		//	if (!Contains(queue.first))
		//		continue;

		// Create a queue copy because handles can get automatically deleted
		auto queueCopy = queue.second;
		// Check handles
		for (auto& handle: queueCopy)
			handle.resume();
	}
}
void IRegistrarBase::ResolvePostRegister(std::string_view str) {
	// Check if there is a queue for the current item
	auto queue = registrationQueue.find(str);
	if (queue == registrationQueue.end())
		// No queue was formed
		return;
	// Calling handle.resume() does not call awaiter.await_suspend.
	// Making sure the item is registered
	if (!Contains(str))
		return;
	// Create a queue copy because handles can get automatically deleted
	auto queueCopy = queue->second;
	// Check handles
	for (auto& handle: queueCopy)
		handle.resume();
}

RegistrarAwaiter& IRegistrarBase::operator co_await() const {
	return *awaiter;
}
