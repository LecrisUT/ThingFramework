module;

#include <memory>
#include <string>
#include <vector>

export module ThingFramework.Utility:Concepts;
export import :Concepts_h;

export namespace ThingFramework {
	//	// Helper template definitions for reference_wrappers
	//	// required to use std::set with std::less<>
	//	template<class T>
	//	auto operator<=>(const std::reference_wrapper<T>& t1, const std::reference_wrapper<T>& t2) {
	//		return t1.get() <=> t2.get();
	//	}
	//	template<class T>
	//	bool operator==(const std::reference_wrapper<T>& t1, const std::reference_wrapper<T>& t2) {
	//		return t1.get() == t2.get();
	//	}

	// Helper template definitions for string comparable types
	// TODO: Maybe these can be consolidated with `sptr`?
	template<strComparable T>
	auto operator<=>(const std::shared_ptr<T>& t1, std::string_view t2) {
		if (t1 == nullptr)
			return "" <=> t2;
		return *t1 <=> t2;
	}
	template<strComparable T>
	auto operator==(const std::shared_ptr<T>& t1, std::string_view t2) {
		if (t1 == nullptr)
			return t2.empty();
		return *t1 == t2;
	}
	template<strComparable T>
	auto operator<=>(const std::unique_ptr<T>& t1, std::string_view t2) {
		if (t1 == nullptr)
			return "" <=> t2;
		return *t1 <=> t2;
	}
	template<strComparable T>
	auto operator==(const std::unique_ptr<T>& t1, std::string_view t2) {
		if (t1 == nullptr)
			return t2.empty();
		return *t1 == t2;
	}
	template<strComparable T>
	auto operator<=>(const std::reference_wrapper<T>& t1, std::string_view t2) {
		return t1.get() <=> t2;
	}
	template<strComparable T>
	bool operator==(const std::reference_wrapper<T>& t1, std::string_view t2) {
		return t1.get() == t2;
	}
}// namespace ThingFramework

using namespace ThingFramework;
template<Hashable T>
struct std::hash<std::reference_wrapper<T>>
    : std::hash<T> {
	using is_transparent = void;
	constexpr size_t operator()(const std::reference_wrapper<T>& val) const {
		return std::hash<T>::operator()(val.get());
	}
};
template<class T>
struct std::less<std::reference_wrapper<T>>
    : std::less<T*>, std::less<const T*> {
	using is_transparent = void;
	// Allow reference_wrapper to be compared with itself
	constexpr bool operator()(const std::reference_wrapper<T>& lhs, const std::reference_wrapper<T>& rhs) const {
		return std::less<T*>::operator()(&lhs, &rhs);
	}
	// Allow reference_wrapper to be compared with non-const pointers
	constexpr bool operator()(const std::reference_wrapper<T>& lhs, T* const& rhs) const {
		return std::less<T*>::operator()(&lhs, rhs);
	}
	constexpr bool operator()(T* const& lhs, const std::reference_wrapper<T>& rhs) const {
		return std::less<T*>::operator()(lhs, &rhs);
	}
	// Allow reference_wrapper to be compared with const pointers
	constexpr bool operator()(const std::reference_wrapper<T>& lhs, const T* const& rhs) const {
		return std::less<const T*>::operator()(&lhs, rhs);
	}
	constexpr bool operator()(const T* const& lhs, const std::reference_wrapper<T>& rhs) const {
		return std::less<const T*>::operator()(lhs, &rhs);
	}
	// Allow reference_wrapper to be compared with any other key that can compare its pointers
	template<TransparentLessKey<T*> Key>
	constexpr bool operator()(const std::reference_wrapper<T>& lhs, const Key& rhs) const {
		return std::less<T*>::operator()(&lhs, rhs);
	}
	template<TransparentLessKey<T*> Key>
	constexpr bool operator()(const Key& lhs, const std::reference_wrapper<T>& rhs) const {
		return std::less<T*>::operator()(lhs, &rhs);
	}
};
template<class T>
struct std::equal_to<std::reference_wrapper<T>>
    : std::equal_to<T*>, std::equal_to<const T*> {
	using is_transparent = void;
	constexpr bool operator()(const std::reference_wrapper<T>& lhs, const std::reference_wrapper<T>& rhs) const {
		return std::equal_to<T*>::operator()(&lhs, &rhs);
	}
	// Allow reference_wrapper to be compared with non-const pointers
	constexpr bool operator()(const std::reference_wrapper<T>& lhs, T* const& rhs) const {
		return std::equal_to<T*>::operator()(&lhs, rhs);
	}
	constexpr bool operator()(T* const& lhs, const std::reference_wrapper<T>& rhs) const {
		return std::equal_to<T*>::operator()(lhs, &rhs);
	}
	// Allow reference_wrapper to be compared with const pointers
	constexpr bool operator()(const std::reference_wrapper<T>& lhs, const T* const& rhs) const {
		return std::equal_to<const T*>::operator()(&lhs, rhs);
	}
	constexpr bool operator()(const T* const& lhs, const std::reference_wrapper<T>& rhs) const {
		return std::equal_to<const T*>::operator()(lhs, &rhs);
	}
	// Allow reference_wrapper to be compared with any other key that can compare its pointers
	template<TransparentLessKey<T*> Key>
	constexpr bool operator()(const std::reference_wrapper<T>& lhs, const Key& rhs) const {
		return std::equal_to<T*>::operator()(&lhs, rhs);
	}
	template<TransparentLessKey<T*> Key>
	constexpr bool operator()(const Key& lhs, const std::reference_wrapper<T>& rhs) const {
		return std::equal_to<T*>::operator()(lhs, &rhs);
	}
};
template<class T>
struct std::less<std::shared_ptr<T>>
    : std::less<T*>, std::less<const T*> {
	// TODO: Add additional specialization for const type if this errors
	using is_transparent = void;
	// Allow shared_ptr to be compared with itself
	constexpr bool operator()(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs) const {
		return std::less<T*>::operator()(lhs.get(), rhs.get());
	}
	// Allow shared_ptr to be compared with non_const pointers
	constexpr bool operator()(T* const& lhs, const std::shared_ptr<T>& rhs) const {
		return std::less<T*>::operator()(lhs, rhs.get());
	}
	constexpr bool operator()(const std::shared_ptr<T>& lhs, T* const& rhs) const {
		return std::less<T*>::operator()(lhs.get(), rhs);
	}
	// Allow shared_ptr to be compared with const pointers
	constexpr bool operator()(const T* const& lhs, const std::shared_ptr<T>& rhs) const {
		return std::less<const T*>::operator()(lhs, rhs.get());
	}
	constexpr bool operator()(const std::shared_ptr<T>& lhs, const T* const& rhs) const {
		return std::less<const T*>::operator()(lhs.get(), rhs);
	}
	// Allow shared_ptr to be compared with any other key that can compare its pointers
	template<TransparentLessKey<T*> Key>
	constexpr bool operator()(const std::shared_ptr<T>& lhs, const Key& rhs) const {
		return std::less<T*>::operator()(lhs.get(), rhs);
	}
	template<TransparentLessKey<T*> Key>
	constexpr bool operator()(const Key& lhs, const std::shared_ptr<T>& rhs) const {
		return std::less<T*>::operator()(lhs, rhs.get());
	}
};
template<class T>
struct std::equal_to<std::shared_ptr<T>>
    : std::equal_to<T*>, std::equal_to<const T*> {
	// TODO: Add additional specialization for const type if this errors
	using is_transparent = void;
	constexpr bool operator()(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs) const {
		return std::equal_to<T*>::operator()(lhs.get(), rhs.get());
	}
	// Allow shared_ptr to be compared with non_const pointers
	constexpr bool operator()(T* const lhs, const std::shared_ptr<T>& rhs) const {
		return std::equal_to<T*>::operator()(lhs, rhs.get());
	}
	constexpr bool operator()(const std::shared_ptr<T>& lhs, T* const rhs) const {
		return std::equal_to<T*>::operator()(lhs.get(), rhs);
	}
	// Allow shared_ptr to be compared with non_const pointers
	constexpr bool operator()(const T* const lhs, const std::shared_ptr<T>& rhs) const {
		return std::equal_to<const T*>::operator()(lhs, rhs.get());
	}
	constexpr bool operator()(const std::shared_ptr<T>& lhs, const T* const rhs) const {
		return std::equal_to<const T*>::operator()(lhs.get(), rhs);
	}
	// Allow shared_ptr to be compared with any other key that can compare its pointers
	template<TransparentEqualKey<T*> Key>
	constexpr bool operator()(const std::shared_ptr<T>& lhs, const Key& rhs) const {
		return std::equal_to<T*>::operator()(lhs.get(), rhs);
	}
	template<TransparentEqualKey<T*> Key>
	constexpr bool operator()(const Key& lhs, const std::shared_ptr<T>& rhs) const {
		return std::equal_to<T*>::operator()(lhs, rhs.get());
	}
};
