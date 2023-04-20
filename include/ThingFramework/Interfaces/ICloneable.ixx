module;

#include <memory>

export module ThingFramework.Interfaces:ICloneable;

export namespace ThingFramework::Interfaces {
	template<class Base>
	struct ICloneable {
		virtual std::unique_ptr<Base> Clone() const = 0;
		template<std::derived_from<Base> Derived>
		std::unique_ptr<Base> CloneThis(const Derived& item) const {
			return std::make_unique<Derived>(item);
		}
	};
}// namespace ThingFramework::Interfaces
