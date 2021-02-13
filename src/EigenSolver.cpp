#include <vdlib/EigenSolver.h>
#include <cmath>

using namespace vdlib;

static const int Matrix_Size = 3;

EigenSolver::EigenSolver()
{
	_isRotation = false;
}

float& EigenSolver::operator()( int row, int col )
{
	return _matrix[row][col];
}

void EigenSolver::eigenSolve3()
{
	tridiagonal3();
	qlAlgorithm();
	guaranteeRotation();
}

void EigenSolver::decrSortEigenSolve3()
{
	tridiagonal3();
	qlAlgorithm();
	decreasingSort();
	guaranteeRotation();
}

void EigenSolver::incrSortEigenSolve3()
{
	tridiagonal3();
	qlAlgorithm();
	increasingSort();
	guaranteeRotation();
}

float EigenSolver::getEigenValue( int i ) const
{
	return _diag[i];
}

vr::vec3f EigenSolver::getEigenVector( int i ) const
{
	return vr::vec3f( _matrix[0][i], _matrix[1][i], _matrix[2][i] );
}

//////////////////////////////////////////////////////////////////////////
// Private
//////////////////////////////////////////////////////////////////////////
void EigenSolver::guaranteeRotation()
{
	if(!_isRotation)
	{
		// change sign on the first column
		for(int iRow = 0; iRow < Matrix_Size; iRow++)
		{
			_matrix[iRow][0] = -_matrix[iRow][0];
		}
	}
}

void EigenSolver::tridiagonal3()
{
	float fM00 = _matrix[0][0];
	float fM01 = _matrix[0][1];
	float fM02 = _matrix[0][2];
	float fM11 = _matrix[1][1];
	float fM12 = _matrix[1][2];
	float fM22 = _matrix[2][2];

	_diag[0] = fM00;
	_subd[2] =(float)0.0;

	if(abs(fM02) > 1e-6)
	{
		float fLength = sqrt(fM01*fM01+fM02*fM02);
		float fInvLength =((float)1.0)/fLength;
		fM01 *= fInvLength;
		fM02 *= fInvLength;
		float fQ =((float)2.0)*fM01*fM12+fM02*(fM22-fM11);
		_diag[1] = fM11+fM02*fQ;
		_diag[2] = fM22-fM02*fQ;
		_subd[0] = fLength;
		_subd[1] = fM12-fM01*fQ;
		_matrix[0][0] =(float)1.0;
		_matrix[0][1] =(float)0.0;
		_matrix[0][2] =(float)0.0;
		_matrix[1][0] =(float)0.0;
		_matrix[1][1] = fM01;
		_matrix[1][2] = fM02;
		_matrix[2][0] =(float)0.0;
		_matrix[2][1] = fM02;
		_matrix[2][2] = -fM01;
		_isRotation = false;
	}
	else
	{
		_diag[1] = fM11;
		_diag[2] = fM22;
		_subd[0] = fM01;
		_subd[1] = fM12;
		_matrix[0][0] =(float)1.0;
		_matrix[0][1] =(float)0.0;
		_matrix[0][2] =(float)0.0;
		_matrix[1][0] =(float)0.0;
		_matrix[1][1] =(float)1.0;
		_matrix[1][2] =(float)0.0;
		_matrix[2][0] =(float)0.0;
		_matrix[2][1] =(float)0.0;
		_matrix[2][2] =(float)1.0;
		_isRotation = true;
	}
}

bool EigenSolver::qlAlgorithm()
{
	const int iMaxIter = 32;

	for(int i0 = 0; i0 < Matrix_Size; i0++)
	{
		int i1;
		for(i1 = 0; i1 < iMaxIter; i1++)
		{
			int i2;
			for(i2 = i0; i2 <= Matrix_Size-2; i2++)
			{
				float fTmp = abs(_diag[i2]) + abs(_diag[i2+1]);
				if( abs(_subd[i2]) + fTmp == fTmp )
					break;
			}
			if(i2 == i0)
			{
				break;
			}

			float fG =(_diag[i0+1] - _diag[i0])/(((float)2.0) *	_subd[i0]);
			float fR = sqrt(fG*fG+(float)1.0);
			if(fG <(float)0.0)
			{
				fG = _diag[i2]-_diag[i0]+_subd[i0]/(fG-fR);
			}
			else
			{
				fG = _diag[i2]-_diag[i0]+_subd[i0]/(fG+fR);
			}
			float fSin =(float)1.0, fCos =(float)1.0, fP =(float)0.0;
			for(int i3 = i2-1; i3 >= i0; i3--)
			{
				float fF = fSin*_subd[i3];
				float fB = fCos*_subd[i3];
				if(abs(fF) >= abs(fG))
				{
					fCos = fG/fF;
					fR = sqrt(fCos*fCos+(float)1.0);
					_subd[i3+1] = fF*fR;
					fSin =((float)1.0)/fR;
					fCos *= fSin;
				}
				else
				{
					fSin = fF/fG;
					fR = sqrt(fSin*fSin+(float)1.0);
					_subd[i3+1] = fG*fR;
					fCos =((float)1.0)/fR;
					fSin *= fCos;
				}
				fG = _diag[i3+1]-fP;
				fR =(_diag[i3]-fG)*fSin+((float)2.0)*fB*fCos;
				fP = fSin*fR;
				_diag[i3+1] = fG+fP;
				fG = fCos*fR-fB;

				for(int i4 = 0; i4 < Matrix_Size; i4++)
				{
					fF = _matrix[i4][i3+1];
					_matrix[i4][i3+1] = fSin*_matrix[i4][i3]+fCos*fF;
					_matrix[i4][i3] = fCos*_matrix[i4][i3]-fSin*fF;
				}
			}
			_diag[i0] -= fP;
			_subd[i0] = fG;
			_subd[i2] =(float)0.0;
		}
		if(i1 == iMaxIter)
		{
			return false;
		}
	}

	return true;
}

void EigenSolver::decreasingSort()
{
	// sort eigenvalues in decreasing order, e[0] >= ... >= e[iSize-1]
	for(int i0 = 0, i1; i0 <= Matrix_Size-2; i0++)
	{
		// locate maximum eigenvalue
		i1 = i0;
		float fMax = _diag[i1];
		int i2;
		for(i2 = i0+1; i2 < Matrix_Size; i2++)
		{
			if(_diag[i2] > fMax)
			{
				i1 = i2;
				fMax = _diag[i1];
			}
		}

		if(i1 != i0)
		{
			// swap eigenvalues
			_diag[i1] = _diag[i0];
			_diag[i0] = fMax;

			// swap eigenvectors
			for(i2 = 0; i2 < Matrix_Size; i2++)
			{
				float fTmp = _matrix[i2][i0];
				_matrix[i2][i0] = _matrix[i2][i1];
				_matrix[i2][i1] = fTmp;
				_isRotation = !_isRotation;
			}
		}
	}
}

void EigenSolver::increasingSort()
{
	// sort eigenvalues in increasing order, e[0] <= ... <= e[iSize-1]
	for(int i0 = 0, i1; i0 <= Matrix_Size-2; i0++)
	{
		// locate minimum eigenvalue
		i1 = i0;
		float fMin = _diag[i1];
		int i2;
		for(i2 = i0+1; i2 < Matrix_Size; i2++)
		{
			if(_diag[i2] < fMin)
			{
				i1 = i2;
				fMin = _diag[i1];
			}
		}

		if(i1 != i0)
		{
			// swap eigenvalues
			_diag[i1] = _diag[i0];
			_diag[i0] = fMin;

			// swap eigenvectors
			for(i2 = 0; i2 < Matrix_Size; i2++)
			{
				float fTmp = _matrix[i2][i0];
				_matrix[i2][i0] = _matrix[i2][i1];
				_matrix[i2][i1] = fTmp;
				_isRotation = !_isRotation;
			}
		}
	}
}
