#include <catch2/catch_all.hpp>

import ThingFramework.Utility;

using namespace ThingFramework;


// Check Specialization concept
// Dummy templates
template<class T>
class T1 {
};
template<class T>
class T2 {
};
struct A {};
struct B {};
// Run test
TEMPLATE_TEST_CASE_SIG("Utility Concepts: Check specialization concept", "[Utility]",
                       ((typename T, template<class...> class Template, bool value), T, Template, value),
                       (T1<A>, T1, true),// T1<A> is a specialization of T1
                       (T1<A>, T2, false)// T1<A> is not a specialization of template T2
) {
	STATIC_CHECK(SpecializationOf<T, Template> == value);
}
// Check smart pointers
TEMPLATE_TEST_CASE_SIG("Utility Concepts: Check smart pointers", "[Utility]",
                       ((typename SPtr, typename T, bool value), SPtr, T, value),
                       (std::shared_ptr<A>, A, true), // shared_ptr is a smart pointer
                       (std::unique_ptr<A>, A, true), // unique_ptr is a smart pointer
                       (std::shared_ptr<A>, B, false),// smart pointers must be of the specified class
                       (std::unique_ptr<A>, B, false) // Complete the list of tests
) {
	STATIC_CHECK(sptr<SPtr, T> == value);
}

// Check smart pointer vectors
TEMPLATE_TEST_CASE_SIG("Utility Concepts: Check smart pointer vectors", "[Utility]",
                       ((typename SPtr, typename T, bool value), SPtr, T, value),
                       (std::shared_ptr<A>, A, false),             // simple smart pointer is not a smart pointer vector
                       (std::vector<std::shared_ptr<A>>, A, true), // shared_ptr is a smart pointer
                       (std::vector<std::unique_ptr<A>>, A, true), // unique_ptr is a smart pointer
                       (std::vector<std::shared_ptr<A>>, B, false),// smart pointers must be of the specified class
                       (std::vector<std::unique_ptr<A>>, B, false) // Complete the list of tests
) {
	STATIC_CHECK(sptrVec<SPtr, T> == value);
}

// Check string comparable
// Test class that is string comparable
struct StringComperable {
	std::string Name;
	StringComperable(std::string_view name) : Name(name) {}
	auto operator<=>(std::string_view str) const {
		return Name <=> str;
	}
	auto operator==(std::string_view str) const {
		return Name == str;
	}
};
struct NotStringComperable {
	std::string Name;
	NotStringComperable(std::string_view name) : Name(name) {}
};
TEMPLATE_TEST_CASE_SIG("Utility Concepts: Check string comparable concepts", "[Utility]",
                       ((typename T, bool is_comparable), T, is_comparable),
                       (StringComperable, true),   // simple smart pointer is not a smart pointer vector
                       (NotStringComperable, false)// simple smart pointer is not a smart pointer vector
) {
	STATIC_REQUIRE(strComparable<T> == is_comparable);
	if constexpr (is_comparable) {
		T item{"Name1"};
		CHECK(item > "Name0");
		CHECK(item != "Name0");
		CHECK(item == "Name1");
		CHECK(item < "Name2");
	}
}


// TODO: Migrate to proper testing
//TEST_CASE("Utility Concepts", "[Utility]") {
//
//	SECTION("Check string comparator") {
//		// A has <=> and == defined
//		STATIC_REQUIRE(strComparable<A>);
//		// B is arbitrary without those definitions
//		STATIC_REQUIRE_FALSE(strComparable<B>);
//		A a{"Name1"}, a2{"Name2"};
//		A &ra = a, ra2 = a2;
//		std::reference_wrapper<A> rwa = a, rwa2 = a2;
//		std::shared_ptr<A> sa = std::make_shared<A>("Name5"), sa2;
//		std::unique_ptr<A> ua = std::make_unique<A>("Name6"), ua2;
//		// Check ordering with respect to strings
//		CHECK(a < a2);
//		CHECK(ra > "Name0");
//		CHECK(ra == "Name1");
//		CHECK(ra < ra2);
//		CHECK(ra < a2);
//		// TODO: Enable missing tests
//		//CHECK(rwa > "Name0");
//		//CHECK(rwa == "Name1");
//		//CHECK(rwa < rwa2);
//		CHECK(rwa < ra2);
//		CHECK(rwa < a2);
//		//CHECK(rwb < rwb2);
//		//CHECK(sa > "Name0");
//		//CHECK(sa == "Name5");
//		//CHECK(sa2 < "Name0");
//		//CHECK(sa2 == "");
//		//CHECK(ua > "Name0");
//		//CHECK(ua == "Name6");
//		//CHECK(ua2 < "Name0");
//		//CHECK(ua2 == "");
//	}
//}
