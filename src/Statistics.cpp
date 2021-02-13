#include <vdlib/Statistics.h>
#include <vdlib/Box.h>

using namespace vdlib;

void Statistics::average( vr::vec3f& avg, const float* vertices, int size )
{
	// Average equation:
	// Average = 1/n * SUM( all vertices )
	avg.set( 0.0f, 0.0f, 0.0f );

	// For each vertex
	for( int i = 0; i < size; i+=3 )
		avg += vr::vec3f( &vertices[i] );

	// 1/n = 1 / (size/3)
	avg *= ( 3.0f / (float)size );
}

void Statistics::covariance( float covariance[3][3], const vr::vec3f& average, const float* vertices, int size )
{
	vr::vec3f vertex;

	// Elements of the Covariance matrix have the following closed form:
	// C[i][j] = 1/n * SUM( vm[i] * vm[j] )
	// m = average
	// vm = v - m
	float sumXX = 0;
	float sumXY = 0;
	float sumXZ = 0;
	float sumYY = 0;
	float sumYZ = 0;
	float sumZZ = 0;

	// For each vertex
	for( int k = 0; k < size; k+=3 )
	{
		vertex.set( &vertices[k] );
		vertex -= average;

		sumXX += vertex.x * vertex.x;
		sumXY += vertex.x * vertex.y;
		sumXZ += vertex.x * vertex.z;
		sumYY += vertex.y * vertex.y;
		sumYZ += vertex.y * vertex.z;
		sumZZ += vertex.z * vertex.z;
	}

	// 1/n = 1 / (size/3)
	float invCount = ( 3.0f / (float)size );

	// Now calculate the covariance matrix
	covariance[0][0] = sumXX * invCount;
	covariance[0][1] = sumXY * invCount;
	covariance[0][2] = sumXZ * invCount;
	covariance[1][0] = sumXY * invCount;
	covariance[1][1] = sumYY * invCount;
	covariance[1][2] = sumYZ * invCount;
	covariance[2][0] = sumXZ * invCount;
	covariance[2][1] = sumYZ * invCount;
	covariance[2][2] = sumZZ * invCount;
}

void Statistics::minMaxVertices( MinMax& result, const float* vertices, int size )
{
	// Get initial values
	result.minValues.set( &vertices[0] );
	result.maxValues.set( &vertices[0] );

	// For each vertex beyond first
	for( int i = 3; i < size; i+=3 )
	{
		// X
		if( vertices[i] < result.minValues[0] )
			result.minValues[0] = vertices[i];
		else if( vertices[i] > result.maxValues[0] )
			result.maxValues[0] = vertices[i];

		// Y
		if( vertices[i+1] < result.minValues[1] )
			result.minValues[1] = vertices[i+1];
		else if( vertices[i+1] > result.maxValues[1] )
			result.maxValues[1] = vertices[i+1];

		// Z
		if( vertices[i+2] < result.minValues[2] )
			result.minValues[2] = vertices[i+2];
		else if( vertices[i+2] > result.maxValues[2] )
			result.maxValues[2] = vertices[i+2];
	}
}

void Statistics::axisMinMaxValues( MinMax& result, const vr::vec3f axis[3], const vr::vec3f& center, 
				                  const float* vertices, int size )
{
	// Get initial values
	vr::vec3f vertex( &vertices[0] );

	const vr::vec3f diff = vertex - center;
	float di = diff.dot( axis[0] );
	float dj = diff.dot( axis[1] );
	float dk = diff.dot( axis[2] );

	result.minValues[0] = di;
	result.maxValues[0] = di;
	result.minValues[1] = dj;
	result.maxValues[1] = dj;
	result.minValues[2] = dk;
	result.maxValues[2] = dk;

	// For each vertex beyond first
	for( int i = 3; i < size; i+=3 )
	{
		vertex.set( &vertices[i] );
		vr::vec3f vertexMinusCenter = vertex - center;

		// I
		di = vertexMinusCenter.dot( axis[0] );
		if( di < result.minValues[0] )
			result.minValues[0] = di;
		else if( di > result.maxValues[0] )
			result.maxValues[0] = di;

		// J
		dj = vertexMinusCenter.dot( axis[1] );
		if( dj < result.minValues[1] )
			result.minValues[1] = dj;
		else if( dj > result.maxValues[1] )
			result.maxValues[1] = dj;

		// K
		dk = vertexMinusCenter.dot( axis[2] );
		if( dk < result.minValues[2] )
			result.minValues[2] = dk;
		else if( dk > result.maxValues[2] )
			result.maxValues[2] = dk;
	}
}
