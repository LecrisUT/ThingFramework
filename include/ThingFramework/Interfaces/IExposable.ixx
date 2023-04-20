module;

#include <memory>
#include <string>

export module ThingFramework.Interfaces:IExposable;

namespace ThingFramework {
	namespace Interfaces {
		class ScribeDriver;
		class Scriber;
	}// namespace Interfaces
	namespace Utility {
		struct TypeInfo;
	}
}// namespace ThingFramework

// End of header

using namespace ThingFramework::Utility;
export namespace ThingFramework::Interfaces {
	class IExposable {
	public:
		std::shared_ptr<ScribeDriver> format;

	public:
		IExposable();
		explicit IExposable(std::string_view format);

	public:
		virtual void ExposeData(Scriber& scriber);
		virtual std::string_view GetName() const;
		virtual const TypeInfo& GetType() const;
		virtual void RegisterName(std::string_view name = "");
	};
}// namespace ThingFramework::Interfaces
