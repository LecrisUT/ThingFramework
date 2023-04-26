module;

// clang-format off
#include <memory>
// clang-format on
#include <filesystem>

export module ThingFramework.Utility:DynamicLibrary;
import ThingFramework.Registrar;
import ThingFramework.Interfaces;

namespace ThingFramework::Utility {
	export class DynamicLibrary;
}
using namespace ThingFramework::Utility;
using namespace ThingFramework::Interfaces;

using namespace ThingFramework::Registrar;
using namespace ThingFramework::Interfaces;
export namespace ThingFramework::Utility {
	/**
	 * Wrapper around dynamic library including metadata
	 *
	 * @ingroup utility
	 */
	class DynamicLibrary
	    : public IExposable {
		/**
		 * Wrapper around dlclose
		 *
		 * @param ptr Raw pointer of the library
		 */
		static void CloseDL(void* ptr);
		/**
		 * Unique pointer holding the dynamic library raw C pointer
		 */
		using ptr_type = std::unique_ptr<void, decltype(&CloseDL)>;
		/**
		 * Wrapper around dlopen
		 *
		 * @param location Path to the dynamic library
		 */
		static ptr_type OpenDL(const std::filesystem::path& location);

		// Fields
	protected:
		/**
		 * Registrar of all (opened) dynamic libraries
		 */
		inline static SharedRegistrar<DynamicLibrary> registrar;
		/**
		 * Filepath location of the dynamic library
		 */
		const std::filesystem::path location;
		/**
		 * Smart pointer to the dynamic library
		 */
		const ptr_type dll;

	private:
		/**
		 * Private constructor that opens the dynamic library.
		 *
		 * Use `Create` to ensure there is a shared_ptr
		 *
		 * @param library Path to the library
		 */
		explicit DynamicLibrary(std::string_view library);

	public:
		/**
		 * Proper constructor API to be used
		 *
		 * @param library Path to the dynamic library
		 * @return Shared pointer of the created item
		 */
		static std::shared_ptr<DynamicLibrary> Create(std::string_view library);
		/**
		 * Helper API to make DynamicLibrary string comparable
		 *
		 * @return Value of `location`
		 */
		std::string_view GetName() const;
		/**
		 * Comparator of DynamicLibraries
		 *
		 * @param t2 DynamicLibrary to compare with
		 * @return Result of comparison
		 */
		bool operator==(const DynamicLibrary& t2) const;
	};

}// namespace ThingFramework::Utility
