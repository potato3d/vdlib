/**
*	Factory pattern for building different types of boxes.
*	author: Paulo Ivson <psantos@tecgraf.puc-rio.br>
*	date:   3-Mar-2008
*/
#ifndef _VDLIB_BOXFACTORY_H_
#define _VDLIB_BOXFACTORY_H_

#include <vdlib/Common.h>

namespace vdlib {

class BoxFactory
{
public:
	enum BoxType
	{
		Type_Aabb,
		Type_Obb
	};

	// Set default box type to be built
	static void setDefaultBoxType( BoxType type );

	// Create box according to default type
	static void createBox( Box& result, const float* vertices, int size );

	// Create box of a specific type
	static void createBox( Box& result, const float* vertices, int size, BoxType BoxType );

private:
	static BoxType s_defaultType;
};

} // namespace vdlib

#endif // _VDLIB_BOXFACTORY_H_
