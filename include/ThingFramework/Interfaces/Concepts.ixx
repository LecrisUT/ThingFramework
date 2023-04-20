module;

#include <concepts>

export module ThingFramework.Interfaces:Concepts_h;

namespace ThingFramework::Interfaces {
	export class IExposable;
	//	export class ICloneable;
}// namespace ThingFramework::Interfaces

export namespace ThingFramework::Interfaces {
	template<class T>
	concept Exposable = std::derived_from<T, IExposable>;
	//	template<class T>
	//	concept Cloneable = std::derived_from<T, ICloneable>;
}// namespace ThingFramework::Interfaces
