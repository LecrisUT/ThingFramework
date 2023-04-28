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
	/**
	 * Concept that checks if Key type can be used to order T type.
	 *
	 * Used for std::set ordering
	 *
	 * @tparam Key Key type
	 * @tparam T Primary type
	 */
	template<class Key, class T>
	concept TransparentLessKey = std::same_as<Key, T> ||
	                             requires(const std::less<T>& comparator, const Key& key, const T& item) {
		                             typename std::less<T>::is_transparent;
		                             comparator(key, item);
		                             comparator(item, key);
	                             };
	/**
	 * Concept that checks if Key type can be used to check equality of T type.
	 *
	 * @tparam Key Key type
	 * @tparam T Primary type
	 */
	template<class Key, class T>
	concept TransparentEqualKey = std::same_as<Key, T> ||
	                              requires(const std::equal_to<T>& comparator, const Key& key, const T& item) {
		                              typename std::equal_to<T>::is_transparent;
		                              comparator(key, item);
		                              comparator(item, key);
	                              };
	/**
	 * Concept that checks if Key type is an overall transparent key of T type
	 *
	 * @tparam Key Key type
	 * @tparam T Primary type
	 */
	template<class Key, class T>
	concept TransparentKey = std::same_as<Key, T> ||
	                         (TransparentLessKey<Key, T> && TransparentEqualKey<Key, T> &&
	                          requires(const std::hash<T>& hasher, const Key& key) {
		                          typename std::hash<T>::is_transparent;
		                          hasher(key);
	                          });
}// namespace ThingFramework
