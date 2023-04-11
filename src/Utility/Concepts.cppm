module;

#include <memory>
#include <string>
#include <vector>

export module ThingFramework.Utility:Concepts;
export import :Concepts_h;

export namespace ThingFramework {
	// Helper template definitions for reference_wrappers
	// required to use std::set with std::less<>
	template<class T>
	auto operator<=>(const std::reference_wrapper<T>& t1, const std::reference_wrapper<T>& t2) {
		return t1.get() <=> t2.get();
	}
	template<class T>
	bool operator==(const std::reference_wrapper<T>& t1, const std::reference_wrapper<T>& t2) {
		return t1.get() == t2.get();
	}

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
