module;

#include <string>

export module ThingFramework.Utility:Standard;
export import :Standard_h;

export namespace ThingFramework {
	// Utility definitions for stdNamed types
	// Make stdNamed string comparable. See `strComparable`
	template<stdNamed T>
	auto operator<=>(const T& a, std::string_view str) {
		return a.GetName() <=> str;
	}
	template<stdNamed T>
	bool operator==(const T& a, std::string_view str) {
		return a.GetName() == str;
	}
	template<stdNamed T>
	auto operator<=>(const T& a, const T& b) {
		return a.GetName() <=> b.GetName();
	}
	template<stdNamed T>
	bool operator==(const T& a, const T& b) {
		return a.GetName() == b.GetName();
	}
}// namespace ThingFramework
