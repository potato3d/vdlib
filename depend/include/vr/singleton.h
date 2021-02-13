#ifndef _VR_SINGLETON_H_
#define _VR_SINGLETON_H_

#include <vr/common.h>

namespace vr {

/************************************************************************/
/* Lifetime Policies                                                    */
/************************************************************************/

/**
 *	@namespace LifetimePolicy
 *	Lifetime policies for Singletons.
 */
namespace LifetimePolicy
{
	//! Never destroys the object.
	struct VRBASE_EXPORT None
	{
		template<typename T>
		static void scheduleDestruction( T*, void(*)() )
		{;}
	};

	//! Destroys the object when the program is about to exit.
	struct VRBASE_EXPORT AtExit
	{
		template<typename T>
		static void scheduleDestruction( T*, void( *funct )() )
		{
			atexit( funct );
		}
	};
}

//! This macro must be invoked once from the singleton's .cpp file.
#define VR_DEFINE_SINGLETON(ClassName) \
	ClassName * ClassName::sm_instance = NULL; \
	bool ClassName::sm_destroyed = false;

/**
 *	Singleton template class.
 *	Provides an unified interface for handling the uniqueness of a Singleton.
 *	Usage: simply inherit from Singleton<YourClassName>. You may optionally
 *		specify Policies as the second template argument.
 */
template<typename T, typename Policy = LifetimePolicy::None>
class Singleton
{
public:
	inline static T * instance();
	inline static bool wasDestroyed();

protected:
	Singleton()
	{;}

	~Singleton()
	{;}

private:
	inline static void create();
	inline static void destroy();

private:
	// forbid copies
	Singleton( Singleton & );
	Singleton & operator=( Singleton & );

private:
	static T *sm_instance;		// singleton instance
	static bool sm_destroyed;	// has the Singleton been destroyed before?
};

template<typename T, typename Policy>
inline T * Singleton<T, Policy>::instance()
{
	if( !sm_instance )
	{
		create();
	}
	return sm_instance;
}

template<typename T, typename Policy>
inline bool Singleton<T, Policy>::wasDestroyed()
{
	return sm_destroyed;
}

template<typename T, typename Policy>
inline void Singleton<T, Policy>::create()
{
	if( !sm_instance )
	{
		sm_instance = new T();
		Policy::scheduleDestruction( sm_instance, &destroy );
		sm_destroyed = false;
	}
}

template<typename T, class Policy>
inline void Singleton<T, Policy>::destroy()
{
	delete sm_instance;
	sm_instance = NULL;
	sm_destroyed = true;
}

} // namespace vr

#endif
