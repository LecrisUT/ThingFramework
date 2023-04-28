module;

#include <concepts>

export module ThingFramework.Registrar:Standard;

namespace ThingFramework {
	namespace Registrar {
		class IRegistrarBase;
		template<class T, class R>
		class IRegistrar;
		template<class T>
		class SharedRegistrarRoot;
		template<class T>
		class RefRegistrarRoot;
		template<class T>
		class ObjectRegistrarRoot;
	}// namespace Registrar
}// namespace ThingFramework

export namespace ThingFramework::Registrar {

	/**
	 * Standard defined registrar types.
	 *
	 * Must have a static member T::registrar derived from IRegistrarBase
	 *
	 * @tparam T Type of the object
	 */
	template<class T>
	concept stdIRegistrar = std::derived_from<decltype(T::registrar), IRegistrarBase> &&
	                        requires {
		                        // Static registrar member is an IRegistrar and it has a member type to retrieve the template variable
		                        typename decltype(T::registrar)::value_type;
	                        } &&
	                        std::derived_from<T, typename decltype(T::registrar)::value_type>;
	/**
	 * Standard defined shared pointer registrar types.
	 *
	 * Must be stdIRegistrar and the registrar is of type SharedRegistrar
	 *
	 * @tparam T Type of the object
	 */
	template<class T>
	concept stdSharedRegistrar = stdIRegistrar<T> &&
	                             std::derived_from<decltype(T::registrar), SharedRegistrarRoot<T>>;
	/**
	 * Standard defined reference registrar types.
	 *
	 * Must be stdIRegistrar and the registrar is of type RefRegistrar
	 *
	 * @tparam T Type of the object
	 */
	template<class T>
	concept stdRefRegistrar = stdIRegistrar<T> &&
	                          std::derived_from<decltype(T::registrar), RefRegistrarRoot<T>>;
	/**
	 * Standard defined object registrar types.
	 *
	 * Must be stdIRegistrar and the registrar is of type ObjectRegistrar
	 *
	 * @tparam T Type of the object
	 */
	template<class T>
	concept stdObjRegistrar = stdIRegistrar<T> &&
	                          std::derived_from<decltype(T::registrar), ObjectRegistrarRoot<T>>;
	/**
	 * Standard defined registrar types.
	 *
	 * Must be either stdRefRegistrar, stdSharedRegistrar, or stdObjRegistrar
	 *
	 * @tparam T Type of the object
	 */
	template<class T>
	concept stdRegistrar = stdRefRegistrar<T> || stdSharedRegistrar<T> || stdObjRegistrar<T>;
}// namespace ThingFramework::Registrar
