module;

#include <stdexcept>
#include <string>

export module ThingFramework.Registrar:Exceptions;

namespace ThingFramework::Registrar {
	class IRegistrarBase;
}

export namespace ThingFramework::Registrar {
	/**
	 * Base Registrar exception
	 */
	class RegistrarError : virtual std::exception {
	protected:
		const IRegistrarBase& registrar;
		RegistrarError(const IRegistrarBase& registrar);
	};
	/**
	 * Exception thrown when item is not found in registrar
	 */
	class NotFound : RegistrarError, std::out_of_range {
		std::string_view item;
		std::string message;
		const char* what() const noexcept override;

	public:
		NotFound(const IRegistrarBase& registrar, std::string_view item);
	};
	/**
	 * Exception thrown when item could not be registered
	 */
	class RegistrationFailed : RegistrarError, std::invalid_argument {
		std::string_view item;
		std::string message;
		const char* what() const noexcept override;

	public:
		RegistrationFailed(const IRegistrarBase& registrar);
	};
}// namespace ThingFramework::Registrar
