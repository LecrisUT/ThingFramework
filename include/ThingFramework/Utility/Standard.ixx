module;

#include <string>

export module ThingFramework.Utility:Standard_h;

export namespace ThingFramework {
	/**
	 * Concept for templates that have a standard name
	 *
	 * @tparam This Class/Type to check
	 */
	template<class This>
	concept stdNamed = requires(const This& a) {
		// Const type here to enforce a::GetName() const
		{ a.GetName() } -> std::convertible_to<std::string_view>;
	};
}// namespace ThingFramework
