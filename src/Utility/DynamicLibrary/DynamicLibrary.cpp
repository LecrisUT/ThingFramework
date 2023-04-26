module;

// clang-format off
#include <memory>
// clang-format on
#include <algorithm>
#include <cassert>
#include <dlfcn.h>
#include <filesystem>
#include <string>

module ThingFramework.Utility;
import :DynamicLibrary;

using namespace ThingFramework;

DynamicLibrary::DynamicLibrary(std::string_view library)
    : location(library), dll{OpenDL(location)} {}
bool DynamicLibrary::operator==(const DynamicLibrary& t2) const { return dll == t2.dll; }
std::shared_ptr<DynamicLibrary> DynamicLibrary::Create(std::string_view libray) {
	// Note: Cannot create a shared pointer in the constructor.
	// Using a static create to ensure there is pointer in the registrar
	// TODO: Switch to a Factory approach
	auto ptr = std::shared_ptr<DynamicLibrary>(new DynamicLibrary(libray));
	registrar.Register(ptr);
	// At library loading static members are initialized and registered
	// There can be dangling registration because ResolvePostRegister cannot be called from constructor.
	registrar.ResolveDanglingRegisters();
	return ptr;
}
DynamicLibrary::ptr_type DynamicLibrary::OpenDL(const std::filesystem::path& Location) {
	void* dl = dlopen(Location.c_str(), RTLD_NOLOAD);
	assert(!dl);
	dl = dlopen(Location.c_str(), RTLD_NOW | RTLD_GLOBAL);
	assert(dl);
	return {dl, CloseDL};
}
void DynamicLibrary::CloseDL(void* ptr) {
	if (ptr == nullptr)
		dlclose(ptr);
}
std::string_view DynamicLibrary::GetName() const {
	return location.c_str();
}
