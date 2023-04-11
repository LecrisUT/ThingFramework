module;

#include <string>

export module ThingFramework.Utility:Exceptions;

export namespace ThingFramework {
	/**
	 * Stub exception for functions not implemented
	 */
	class NotImplemented : std::exception {
		std::string_view operation;
		std::string message;
		const char* what() const noexcept override;

	public:
		NotImplemented(std::string_view operation);
	};
}// namespace ThingFramework
