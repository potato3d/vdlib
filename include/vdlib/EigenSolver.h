/**
*	Inspired on the eigen solver class from Geometric Tools version 3.9.
*	author: Paulo Ivson <psantos@tecgraf.puc-rio.br>
*	date:   3-Mar-2008
*	
*	Geometric Tools, Inc.
*	http://www.geometrictools.com
*	Copyright(c) 1998-2006.  All Rights Reserved
*	The Wild Magic Library(WM3) source code is supplied under the terms of	the license agreement
*	http://www.geometrictools.com/License/WildMagic3License.pdf
*	and may not be copied or disclosed except in accordance with the terms of that agreement.
*/
#ifndef _VDLIB_EIGENSOLVER_H_
#define _VDLIB_EIGENSOLVER_H_

#include <vdlib/Common.h>

namespace vdlib {

/**
*	Extracts eigenvectors and eigenvalues from a symmetric 3x3 matrix.
*/
class EigenSolver
{
public:
	EigenSolver();

	// Set the matrix for eigen decomposition
	float& operator()( int row, int col );

	// Solve eigensystem
	void eigenSolve3();

	// Solve eigensystem, results are sorted by decreasing eigenvalues
	void decrSortEigenSolve3();

	// Solve eigensystem, results are sorted by increasing eigenvalues
	void incrSortEigenSolve3();

	// Get the eigenvalues
	float getEigenValue( int i ) const;

	// Get eigenvectors
	vr::vec3f getEigenVector( int i ) const;

private:
	float _matrix[3][3];
	float _diag[3];
	float _subd[3];

	// For odd size matrices, the Householder reduction involves an odd
	// number of reflections.  The product of these is a reflection.  The
	// QL algorithm uses rotations for further reductions.  The final
	// orthogonal matrix whose columns are the eigenvectors is a reflection,
	// so its determinant is -1.  For even size matrices, the Householder
	// reduction involves an even number of reflections whose product is a
	// rotation.  The final orthogonal matrix has determinant +1.  Many
	// algorithms that need an eigen decomposition want a rotation matrix.
	// We want to guarantee this is the case, so _isRotation keeps track of
	// this.  The DecrSort and IncrSort further complicate the issue since
	// they swap columns of the orthogonal matrix, causing the matrix to
	// toggle between rotation and reflection.  The value _isRotation must
	// be toggled accordingly.
	bool _isRotation;
	void guaranteeRotation();

	// Householder reduction to tridiagonal form
	void tridiagonal3();

	// QL algorithm with implicit shifting, applies to tridiagonal matrices
	bool qlAlgorithm();

	// sort eigenvalues from largest to smallest
	void decreasingSort();

	// sort eigenvalues from smallest to largest
	void increasingSort();
};

} // namespace vdlib

#endif // _VDLIB_EIGENSOLVER_H_
