module;

#include <memory>
#include <vector>

export module ThingFramework.Registrar:Concepts_h;
import ThingFramework.Utility;

namespace ThingFramework {
	namespace Utility {
		//		struct TypeInfo;
		template<class T>
		struct Resolver;
	}// namespace Utility
}// namespace ThingFramework

using namespace ThingFramework::Utility;
export namespace ThingFramework {
	template<class T>
	concept NameResolvable = requires(const T& item) {
		{ Resolver<T>::Name(item) } -> std::convertible_to<std::string_view>;
	};
}// namespace ThingFramework
