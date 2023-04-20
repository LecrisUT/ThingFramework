module;

#include <stdexcept>
#include <string>
#include <typeinfo>

module ThingFramework.Registrar;
import :Exceptions;

using namespace ThingFramework::Registrar;

RegistrarError::RegistrarError(const IRegistrarBase& registrar) : registrar{registrar} {
}
NotFound::NotFound(const IRegistrarBase& registrar, std::string_view item)
    : RegistrarError(registrar),
      std::out_of_range(typeid(registrar).name()) {
	message = "Item [" + std::string(item) + "] not registered in " + typeid(registrar).name();
}
RegistrationFailed::RegistrationFailed(const IRegistrarBase& registrar)
    : RegistrarError(registrar),
      std::invalid_argument("") {
	message = "Registration failed in " + std::string(typeid(registrar).name());
}
const char* NotFound::what() const noexcept {
	return message.c_str();
}
const char* RegistrationFailed::what() const noexcept {
	return message.c_str();
}
