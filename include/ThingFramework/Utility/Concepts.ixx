module;

#include <memory>
#include <string>
#include <vector>

export module ThingFramework.Utility:Concepts_h;

namespace ThingFramework {
	// Private utilities helpers
	template<template<class...> class Template, class... Args>
	void derived_from_specialization_impl(const Template<Args...>&);
}// namespace ThingFramework

export namespace ThingFramework {
	/**
	 * Concept that checks if `This` is a specialization of `Template<>`.
	 *
	 * Example:
	 *   template<SpecializationOf<std::vector> T>
	 *     -> T can be std::vector<int>, but not std::list<int>, etc.
	 *
	 * Reference: https://stackoverflow.com/a/70130881
	 *
	 * @tparam This Class/Type to check
	 * @tparam Template Template type that we want to check for
	 * @ingroup utility
	 */
	template<class This, template<class...> class Template>
	concept SpecializationOf = requires(const This& t) {
		derived_from_specialization_impl<Template>(t);
	};

	/**
	 * Concept that checks if `This` is an equivalent smart pointer of `T`
	 *
	 * @tparam This Class/Type to check
	 * @tparam T Type of the smart pointer's object
	 * @ingroup utility
	 */
	template<class This, class T>
	concept sptr = std::derived_from<This, std::unique_ptr<T>> ||
	               std::derived_from<This, std::shared_ptr<T>>;
	/**
	 * Concept that checks if `This` is an equivalent vector of smart pointers of `T`
	 *
	 * @tparam This Class/Type to check
	 * @tparam T Type of the smart pointer's object
	 * @ingroup utility
	 */
	template<class This, class T>
	concept sptrVec = std::derived_from<This, std::vector<std::unique_ptr<T>>> ||
	                  std::derived_from<This, std::vector<std::shared_ptr<T>>>;

	/**
	 * Concept that check if `This` can be compared against a string
	 *
	 * @tparam This Class/Type to check
	 * @ingroup utility
	 */
	template<class This>
	concept strComparable = requires(const This& a, std::string_view str) {
		a <=> str;
		a == str;
	};

	/**
	 * Concept that check if `This` can be hashed
	 *
	 * @tparam This Class/Type to check
	 * @ingroup utility
	 */
	template<class This>
	concept Hashable = requires(const This& a) {
		{ std::hash<This>{}(a) } -> std::convertible_to<std::size_t>;
	};
}// namespace ThingFramework
