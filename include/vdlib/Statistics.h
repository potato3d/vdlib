/**
*	Statistical analysis of geometric information.
*	author: Paulo Ivson <psantos@tecgraf.puc-rio.br>
*	date:   3-Mar-2008
*/
#ifndef _VDLIB_STATISTICS_H_
#define _VDLIB_STATISTICS_H_

#include <vdlib/Common.h>

namespace vdlib {

// Return struct for Min-Max operations.
// Stores computed minimum and maximum vertex values for each axis.
class MinMax
{
public:
	vr::vec3f minValues;
	vr::vec3f maxValues;
};

// Main algorithms
class Statistics
{
public:
	// Geometric mean of vertices.
	// Average equation:
	// Average = 1/n * SUM( all vertices )
	static void average( vr::vec3f& average, const float* vertices, int size );

	// Covariance around average parameter.
	// Elements of the Covariance matrix have the following closed form:
	// C[i][j] = 1/n * SUM( vm[i] * vm[j] )
	// m = average
	// vm = v - m
	static void covariance( float covariance[3][3], const vr::vec3f& average, const float* vertices, int size );

	// Computes the minimum and maximum vertex values, along each axis X, Y and Z.
	static void minMaxVertices( MinMax& result, const float* vertices, int size );

	// Computes the minimum and maximum vertex values along three specified axis, using center as reference point.
	static void axisMinMaxValues( MinMax& result, const vr::vec3f axis[3], const vr::vec3f& center, 
		                          const float* vertices, int size );
};

} // namespace vdlib

#endif // _VDLIB_STATISTICS_H_
