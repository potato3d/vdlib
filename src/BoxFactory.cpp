#include <vdlib/BoxFactory.h>
#include <vdlib/Box.h>
#include <vdlib/Statistics.h>
#include <vdlib/EigenSolver.h>
#include <iostream>

using namespace vdlib;

//////////////////////////////////////////////////////////////////////////
// Main box creation algorithms
void createAABB( Box& result, const float* vertices, int size )
{
	// Get minimum and maximum along X, Y and Z
	MinMax minMax;
	Statistics::minMaxVertices( minMax, vertices, size );

	// Set axis
	result.axis[0] = vr::vec3f::UNIT_X();
	result.axis[1] = vr::vec3f::UNIT_Y();
	result.axis[2] = vr::vec3f::UNIT_Z();

	// Set extents
	result.extents[0] = ( minMax.maxValues[0] - minMax.minValues[0] ) * 0.5f;
	result.extents[1] = ( minMax.maxValues[1] - minMax.minValues[1] ) * 0.5f;
	result.extents[2] = ( minMax.maxValues[2] - minMax.minValues[2] ) * 0.5f;

	// Set center
	result.center = ( minMax.minValues + minMax.maxValues ) * 0.5f;
}

void createOBB( Box& result, const float* vertices, int size )
{
	// Initial OBB center
	vr::vec3f average;

	// Results in OBB axis (eigenvectors) and extents (eigenvalues)
	float covariance[3][3];

	// Compute statistical analysis
	Statistics::average( average, vertices, size );
	Statistics::covariance( covariance, average, vertices, size );

	// Set up the eigen solver
	EigenSolver solver;

	solver( 0, 0 ) = covariance[0][0];
	solver( 0, 1 ) = covariance[0][1];
	solver( 0, 2 ) = covariance[0][2];
	solver( 1, 0 ) = covariance[1][0];
	solver( 1, 1 ) = covariance[1][1];
	solver( 1, 2 ) = covariance[1][2];
	solver( 2, 0 ) = covariance[2][0];
	solver( 2, 1 ) = covariance[2][1];
	solver( 2, 2 ) = covariance[2][2];
	solver.incrSortEigenSolve3();

	// Get eigenVectors
	vr::vec3f axis[3];
	axis[0] = solver.getEigenVector( 0 );
	axis[1] = solver.getEigenVector( 1 );
	axis[2] = solver.getEigenVector( 2 );

	// Now we need to adjust the box center and extents

	// Let C be the box center and let U0, U1, and U2 be the box axes.  Each
	// input point is of the form X = C + y0*U0 + y1*U1 + y2*U2.  The
	// following code computes min(y0), max(y0), min(y1), max(y1), min(y2),
	// and max(y2). The box center is then adjusted to be
	//   C' = C + 0.5*(min(y0)+max(y0))*U0 + 0.5*(min(y1)+max(y1))*U1 + 0.5*(min(y2)+max(y2))*U2
	MinMax minMax;
	Statistics::axisMinMaxValues( minMax, axis, average, vertices, size );

	//////////////////////////////////////////////////////////////////////////
	// We have sufficient data to build the OBB

	// Adjust center
	average +=	axis[0] * ( 0.5f * ( minMax.minValues[0] + minMax.maxValues[0] ) );
	average +=	axis[1] * ( 0.5f * ( minMax.minValues[1] + minMax.maxValues[1] ) );
	average +=	axis[2] * ( 0.5f * ( minMax.minValues[2] + minMax.maxValues[2] ) );
	result.center = average;

	// Set axis
	result.axis[0] = axis[0];
	result.axis[1] = axis[1];
	result.axis[2] = axis[2];

	// Adjust extents
	result.extents[0] = ( minMax.maxValues[0] - minMax.minValues[0] ) * 0.5f;
	result.extents[1] = ( minMax.maxValues[1] - minMax.minValues[1] ) * 0.5f;
	result.extents[2] = ( minMax.maxValues[2] - minMax.minValues[2] ) * 0.5f;
}

//////////////////////////////////////////////////////////////////////////
// BoxFactory implementation

BoxFactory::BoxType BoxFactory::s_defaultType = BoxFactory::Type_Aabb;

void BoxFactory::setDefaultBoxType( BoxType type )
{
	s_defaultType = type;
}

void BoxFactory::createBox( Box& result, const float* vertices, int size )
{
	createBox( result, vertices, size, s_defaultType );
}

void BoxFactory::createBox( Box& result, const float* vertices, int size, BoxType BoxType )
{
	switch( BoxType )
	{
	case Type_Aabb:
		createAABB( result, vertices, size );
		break;

	case Type_Obb:
		createOBB( result, vertices, size );
		break;

	default:
		printf( "Unknown box type!\n" );
		break;
	}
}
