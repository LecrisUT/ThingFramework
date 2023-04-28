module;

// clang-format off
#include <memory>
// clang-format on
#include <map>
#include <optional>
#include <string>

export module ThingFramework.Registrar:IRegistrar;
export import :IRegistrar_h;
import :Concepts;
import :Exceptions;
import ThingFramework.Utility;

using namespace ThingFramework::Registrar;

template<class T, class R>
IRegistrar<T, R>::IRegistrar() noexcept {
	static_assert(std::derived_from<T, IExposable>,
	              "Item type must be IExposable");
	static_assert(std::derived_from<R, IRegistrar<T, R>>,
	              "Registrar type must be derived from IRegistrar<T,R>");
}
template<class T, class R>
bool IRegistrar<T, R>::Contains(std::string_view str) const {
	return static_cast<const R*>(this)->map.contains(str);
}
template<class T, class R>
bool IRegistrar<T, R>::Contains(const T& item) const {
	if constexpr (R::traits::ref_registrar)
		return static_cast<const R*>(this)->set.contains(item);
	else
		return static_cast<const R*>(this)->set.contains(&item);
}
template<class T, class R>
bool IRegistrar<T, R>::Contains(std::shared_ptr<T> item) const
    requires R::traits::ptr_registrar
{
	if constexpr (!R::traits::ptr_registrar)
		throw IllDefined("Contains");
	else
		return static_cast<const R*>(this)->set.contains(item);
}

template<class T, class R>
bool IRegistrar<T, R>::TryRegisterName(std::string_view name, const IExposable& item) {
	// Derived type of this with type R (CRTP)
	auto this_derived = static_cast<R*>(this);

	// Check if IExposable item is of type T
	if (dynamic_cast<const T*>(&item) == nullptr)
		throw RegistrationFailed(*this);
	const T& cast_item = dynamic_cast<const T&>(item);

	// If the item name is already present, report if it is the same item or not
	if (this_derived->map.contains(name))
		return &(*this_derived->map[name]) == &cast_item;

	// Otherwise proceed to registration
	// Get the non-const reference/pointer
	typename R::traits::set_type::iterator set_iter;
	if constexpr (R::traits::ref_registrar)
		set_iter = this_derived->set.find(cast_item);
	else
		set_iter = this_derived->set.find(&cast_item);
	if (set_iter == this_derived->set.end())
		throw RegistrationFailed(*this);

	// Add the item to the name map
	this_derived->map.insert_or_assign(std::string(name), *set_iter);

	// Check for previously registered items with different name
	for (auto iter = this_derived->map.begin(); iter != this_derived->map.end();) {
		if (&(*iter->second) == &cast_item && iter->first != name)
			// Delete from the map
			iter = this_derived->map.erase(iter);
		else
			iter++;
	}

	// Finish successfully
	return true;
}
template<class T, class R>
bool IRegistrar<T, R>::TryRegisterName(std::string_view name, std::shared_ptr<IExposable> item) {
	if constexpr (!R::traits::ptr_registrar)
		throw IllDefined("TryRegisterName");

	// Derived type of this with type R (CRTP)
	auto this_derived = static_cast<R*>(this);

	// Check if a null object was passed
	if (item == nullptr)
		throw RegistrationFailed(*this);

	// Check if object is of appropriate type
	auto item_derived = std::dynamic_pointer_cast<T>(item);
	if (item_derived == nullptr)
		throw RegistrationFailed(*this);

	// If the item name is already present, report if it is the same item or not
	if (this_derived->map.contains(name))
		return this_derived->map[name] == item;

	// Otherwise proceed to registration
	// Check that item is already registered in the set
	if (this_derived->set.find(item_derived) == this_derived->set.end())
		throw RegistrationFailed(*this);

	// Add the item to the name map
	this_derived->map.insert_or_assign(std::string(name), item_derived);

	// Check for previously registered items with different name
	for (auto iter = this_derived->map.begin(); iter != this_derived->map.end();) {
		if (iter->second == item && iter->first != name)
			// Delete from the map
			iter = this_derived->map.erase(iter);
		else
			iter++;
	}

	// Finish successfully
	return true;
}

template<class T, class R>
bool IRegistrar<T, R>::TryRegister(IExposable&& item) {
	if constexpr (!R::traits::ref_registrar)
		throw IllDefined("TryRegister");
	else {
		// Derived type of this with type R (CRTP)
		auto this_derived = static_cast<R*>(this);

		// Add the item to the set
		auto res = this_derived->set.insert(std::forward<IExposable>(item));

		// If failed report failure
		if (!res.second)
			return false;

		// If the item's name can be resolved use it to register the name automatically
		if constexpr (NameResolvable<T>) {
			auto currName = Resolver<T>::Name(item);
			if (!currName.empty()) {
				RegisterName(currName, item);
			}
		}

		// Finish successfully
		return true;
	}
}
template<class T, class R>
bool IRegistrar<T, R>::TryRegister(std::shared_ptr<IExposable> item) {
	if constexpr (!R::traits::ptr_registrar)
		throw IllDefined("TryRegister");
	else {
		// Derived type of this with type R (CRTP)
		auto this_derived = static_cast<R*>(this);

		// Check if object is of appropriate type
		auto item_derived = std::dynamic_pointer_cast<T>(item);
		if (item_derived == nullptr)
			throw std::bad_cast();

		// Add the item to the set
		auto res = this_derived->set.insert(item_derived);

		// If failed report failure
		if (!res.second)
			return false;

		// If the item's name can be resolved use it to register the name automatically
		if constexpr (NameResolvable<T>) {
			auto currName = Resolver<T>::Name(*item);
			if (!currName.empty()) {
				RegisterName(currName, item);
			}
		}

		// Finish successfully
		return true;
	}
}

template<class T, class R>
bool IRegistrar<T, R>::TryRemove(std::string_view name) {
	// Derived type of this with type R (CRTP)
	auto this_derived = static_cast<R*>(this);

	// Check if the item is registered or not
	if (this_derived->map.contains(name))
		return false;

	// Get the registered item
	auto item = this_derived->map.at(name);

	// Delete from both the set and map
	this_derived->set.erase(item);
	this_derived->map.erase(name);

	return true;
}

template<class T, class R>
std::optional<std::reference_wrapper<IExposable>> IRegistrar<T, R>::TryGetRef(std::string_view str) const {
	// Derived type of this with type R (CRTP)
	auto this_derived = static_cast<const R*>(this);

	// Check if item is registered
	auto key = this_derived->map.find(str);
	if (key == this_derived->map.end())
		return std::nullopt;
	// Return the item if found

	if constexpr (R::traits::ref_registrar)
		return key->second;
	else if constexpr (R::traits::ptr_registrar)
		return std::ref(*key->second);
}
template<class T, class R>
std::shared_ptr<IExposable> IRegistrar<T, R>::TryGetPtr(std::string_view str) const {
	if constexpr (!R::traits::ptr_registrar)
		throw IllDefined("TryGetPtr");
	else {

		// Derived type of this with type R (CRTP)
		auto this_derived = static_cast<const R*>(this);

		// Check if item is registered
		auto key = this_derived->map.find(str);
		if (key == this_derived->map.end())
			return nullptr;
		// Return the item if found
		return key->second;
	}
}
