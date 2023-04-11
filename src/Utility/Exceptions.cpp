module;

#include <string>

module ThingFramework.Utility;
import :Exceptions;

using namespace ThingFramework;

NotImplemented::NotImplemented(std::string_view operation) {
	message = "Operation [" + std::string(operation) + "] not implemented ";
}
const char* NotImplemented::what() const noexcept {
	return message.c_str();
}
