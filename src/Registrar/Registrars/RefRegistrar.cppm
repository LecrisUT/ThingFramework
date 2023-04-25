module;

// clang-format off
#include <memory>
// clang-format on
#include <string>

export module ThingFramework.Registrar:RefRegistrar;
export import :RefRegistrar_h;
import :RefRegistrarRoot;
import :Standard;

using namespace ThingFramework::Registrar;

template<class T, class U>
RefRegistrar<T, U>::RefRegistrar(RefRegistrarRoot<U>& root) noexcept : Root{root} {
	static_assert(std::derived_from<T, U>);
}
template<class T, class U>
RefRegistrar<T, U>::RefRegistrar() noexcept
    requires stdRefRegistrar<U>
    : Root{U::registrar} {
	// Note: Cannot use the default constructor because static_assert would not be triggered
	// Possible if these can be made to be constexpr
	static_assert(std::derived_from<T, U>);
}
template<class T, class U>
bool RefRegistrar<T, U>::TryRemove(std::string_view str) {
	if (!base::TryRemove(str))
		return false;
	Root.Erase(str);
	return true;
}
template<class T, class U>
bool RefRegistrar<T, U>::TryRegisterName(std::string_view name, const IExposable& item) {
	bool res = base::TryRegisterName(name, item);
	if (!res)
		return false;
	bool res2 = Root.TryRegisterName(name, item);
	if (!res2) {
		throw NotRegistered(*this, name);
	}
	return true;
}
template<class T, class U>
bool RefRegistrar<T, U>::TryRegister(IExposable&& item) {
	auto res = base::TryRegister(item);
	if (!res)
		return false;
	auto res2 = Root.TryRegister(item);
	if (!res2) {
		throw NotRegistered(*this, "[No name calculated]");
	}
	return true;
}
template<class T, class U>
void RefRegistrar<T, U>::ResolvePostRegister(std::string_view str) {
	base::ResolvePostRegister(str);
	Root.ResolvePostRegister(str);
}
