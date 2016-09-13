#pragma once


#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

#include "Vector3.h"
#include "Vector4.h"

namespace vengine {

/**
*	Class represating 4x4 square matrix for use in 3D graphics operations.
*	It allows for various operations and transformations associated witd 3D graphics.
*	It is represating data as 4 adjacent vectors, so it can be parsed as long array to 
*	librarys like OpenGL.
*	As for data access: columns goes first, then rows.
*/
class Matrix4 {
public:
	/**
	*	Default constructor doing nothing.
	*/
	Matrix4();
	/**
	*	Constructor initializes matrix one value.
	*
	*	@param value the value to be put into matrix
	*/
	Matrix4(float value);
	/**
	*	Copy constructor.
	*
	*	@param matrix other matrix to be copied to the new one.
	*/
	Matrix4(const Matrix4& matrix);
	/**
	*	Copies array of 4 vector. Each vector represents one column.
	*
	*	@param vectors array of 4 Vector4 vectors to be copied to the matrix.
	*/
	Matrix4(const Vector4* vectors);
	/**
	*	Initializes columns with specified vectors.
	*
	*	@param c1 vector containing data for first column.
	*	@param c2 vector containing data for second column.
	*	@param c3 vector containing data for third column.
	*	@param c4 vector containing data for fourth column. 
	*/
	Matrix4(const Vector4& c1, const Vector4& c2, const Vector4& c3, const Vector4& c4);
	
	/**
	*	Copy operator.
	*
	*	@param source other matrix to be copied.
	*	@return Reference to the object calling this operator.
	*/
	Matrix4&		operator=(const Matrix4& source);
	/**
	*	Copies array of 4 vector. Each vector represents one column.
	*
	*	@param vectors array of 4 Vector4 vectors to be copied to the matrix.
	*	@return Reference to the object calling this operator.
	*/
	Matrix4&		operator=(const Vector4* vectors);
	/**
	*	Copies one value to the whole matrix.
	*
	*	@param value the value to be put into the matrix
	*	@return Reference to the object calling this operator.
	*/
	Matrix4&		operator=(float value);

	/**
	*	Multiply matrixes and store result in the left matrix.
	*
	*	@param matrixR the matrix standing on the right side in the matrix multiplication
	*	@return Reference to the object calling this operator.
	*/
	Matrix4&		operator*=(const Matrix4& matrixR);
	/**
	*	Multiply each element of the matrix by given value and store result in the same matrix.
	*
	*	@param value each element of the matrix will be multiplied by this value.
	*	@return Reference to the object calling this operator.
	*/
	Matrix4&		operator*=(float value);

	/**
	*	Add each corresponding element of the one matrix to the other one and store value in the left matrix.
	*
	*	@param matrix other matrix to be added.
	*	@return Reference to the object calling this operator.
	*/
	Matrix4&		operator+=(const Matrix4& matrix);
	/**
	*	Add to each element of matrix given value and store result in the matrix.
	*
	*	@param value the value to be added.
	*	@return Reference to the object calling this operator.
	*/
	Matrix4&		operator+=(float value);

	/**
	*	Subtract each corresponding element of the right matrix from the left one and store value in the left matrix.
	*
	*	@param matrix other matrix to be subtracted.
	*	@return Reference to the object calling this operator.
	*/
	Matrix4&		operator-=(const Matrix4& matrix);
	/**
	*	Subtract from each element of the matrix given value and store results in the matrix.
	*
	*	@param value the value to be subtracted.
	*	@return Reference to the object calling this operator.
	*/
	Matrix4&		operator-=(float value);

	/**
	*	Divide each element of matrix by given value and store result in the matrix.
	*
	*	@param value dividing value.
	*	@return Reference to the object calling this operator.
	*/
	Matrix4&		operator/=(float value);

	/**
	*	Return given column presented as Vector4.
	*	In addition, when using second [] operator, specified row can be get.
	*	I.e. to get 3rd row from 2nd column, [1][2] should be used.
	*
	*	@param item index of the column.
	*	@return Reference to vector representing given column
	*/
	Vector4&		operator[](int item);
	/**
	*	Return given column presented as Vector4.
	*	In addition, when using second [] operator, specified row can be get.
	*	I.e. to get 3rd row from 2nd column, [1][2] should be used.
	*
	*	@param item index of the column.
	*	@return Const reference to vector representing given column
	*/
	const Vector4&	operator[](int item) const;

	/**
	*	Converts matrix to array of 16 elements. This conversion can be used for some graphic library,
	*	which are storing matrix as 16 elements matrix.
	*
	*	@return Pointer to elements of matrix represented as array of 16 elements.
	*/
	operator float*();
	operator const float*() const;

	/**
	*	Method applying translation matrix by vector (x, y, z),  which is used in 3D graphics.
	*	This operation will be done AFTER all earlier operations, so to do scale and then translation for matrix m,
	*	one can call: m.Scale(...).Translate(...)
	*
	*	@param x component x of the translate vector.
	*	@param y component y of the translate vector.
	*	@param z component z of the translate vector.
	*	@return Reference to the object calling this operator.
	*/
	Matrix4&		Translate(float x, float y, float z);
	/**
	*	Method applying translation matrix by vector, which is used in 3D graphics.
	*	This operation will be done AFTER all earlier operations, so to do scale and then translation for matrix m,
	*	one can call: m.Scale(...).Translate(...)
	*
	*	@param vector translate vector.
	*	@return Reference to the object calling this operator.
	*/
	Matrix4&		Translate(const Vector3& vector);

	/**
	*	Method applying scale matrix by values (x, y, z), which is used in 3D graphics.
	*	This operation will be done AFTER all earlier operations, so to do scale and then translation for matrix m,
	*	one can call: m.Scale(...).Translate(...)
	*
	*	@param x scale factor of axis x.
	*	@param y scale factor of axis y.
	*	@param z scale factor of axis z.
	*	@return Reference to the object calling this operator.
	*/
	Matrix4&		Scale(float x, float y, float z);
	/**
	*	Method applying scale matrix by vector containing values (x, y, z), which is used in 3D graphics.
	*	This operation will be done AFTER all earlier operations, so to do scale and then translation for matrix m,
	*	one can call: m.Scale(...).Translate(...)
	*
	*	@param vector the vector containing scale factors of x, y and z axes.
	*	@return Reference to the object calling this operator.
	*/
	Matrix4&		Scale(const Vector3& vector);

	/**
	*	Method applying to matrix transformations allowing to easily apply camera transformation to current transformations.
	*
	*	@param eye position of the camera.
	*	@param target point at which camera should be looking.
	*	@param up vector determining tilt of the camera.
	*	@return Reference to the object calling this operator.
	*/
	Matrix4&		LookAt(const Vector3& eye, const Vector3& target, const Vector3& up);

	/**
	*	Converts matrix to easily readable text data.
	*
	*	@return std::string object containing text visualization of the matrix.
	*/
	std::string		ToString() const;

	/**
	*	Static method generating translation matrix used in 3D graphics.
	*
	*	@param x component x of the translate vector.
	*	@param y component y of the translate vector.
	*	@param z component z of the translate vector.
	*	@return Translation matrix.
	*/
	static Matrix4	GetTranslate(float x, float y, float z);
	/**
	*	Static method generating translation matrix used in 3D graphics.
	*
	*	@param vector translate vector.
	*	@return Translation matrix.
	*/
	static Matrix4	GetTranslate(const Vector3& vector);
	/**
	*	Static method generating scale matrix used in 3D graphics.
	*
	*	@param x scale factor of axis x.
	*	@param y scale factor of axis y.
	*	@param z scale factor of axis z.
	*	@return Translation matrix.
	*/
	static Matrix4	GetScale(float x, float y, float z);
	/**
	*	Static method generating scale matrix used in 3D graphics.
	*
	*	@param vector the vector containing scale factors of x, y and z axes.
	*	@return Translation matrix.
	*/
	static Matrix4	GetScale(const Vector3& vector);

	/**
	*	Static method generating matrix containing transformations designated specially for camera.
	*
	*	@param eye position of the camera.
	*	@param target point at which camera should be looking.
	*	@param up vector determining tilt of the camera.
	*	@return Matrix with camera transforamtions
	*/
	static Matrix4	GetLookAt(const Vector3& eye, const Vector3& target, const Vector3& up);

	/**
	*	Static method generating perspective projection matrix.
	*
	*	@param left world coordinate of the left edge of the near plane.
	*	@param right world coordinate of the right edge of the near plane.
	*	@param bottom world coordinate of the bottom edge of the near plane.
	*	@param top world coordinate of the top edge of the near plane.
	*	@param near distance of the near clipping plane from viewer.
	*	@param far distance of the far clipping plane from viewer.
	*	@return Pespective projection matrix.
	*/
	static Matrix4	GetFrustum(float left, float right, float bottom, float top, float near, float far);
	/**
	*	Static method generating perspective projection matrix with simplified arguments.
	*
	*	@param fov field of view in degrees.
	*	@param aspect aspect ratio of the screen: height/width.
	*	@param near distance of the near clipping plane.
	*	@param far distance of the far clipping plane.
	*	@return Perspective projection matrix.
	*/
	static Matrix4	GetPerspective(float fov, float aspect, float near, float far);
	/**
	*	Static method generating orthogonal projection matrix.
	*
	*	@param left world coordinate of the left edge of the cube.
	*	@param right world coordinate of the righ tedge of the cube.
	*	@param bottom world coordinate of the bottom edge of the cube.
	*	@param top world coordinate of the top edge of the cube.
	*	@param near distance of the near clipping plane from viewer.
	*	@param far distance of the far clipping plane from viewer.
	*	@return Pespective projection matrix.
	*/
	static Matrix4	GetOrtho(float left, float right, float bottom, float top, float near, float far);


	static const Matrix4 identity;	/* Identity matrix.					*/
	static const Matrix4 zeroes;	/* Matrix filled with zeroes.		*/

private:
	Vector4 _columns[4];			/* Columns goes first, then rows.	*/
};

/**
*	Multiply two matrixes.
*
*	@param matrixL the matrix standing on the left side in the matrix multiplication.
*	@param matrixR the matrix standing on the right side in the matrix multiplication.
*	@return Matrix containing result of multiplication.
*/
Matrix4		operator*(const Matrix4& matrixL, const Matrix4& matrixR);
/**
*	Multiply each element of the matrix by given value.
*
*	@param matrix matrix to be multiplied.
*	@param value each element of the matrix will be multiplied by this value.
*	@return Matrix containing result of multiplication.
*/
Matrix4		operator*(const Matrix4& matrix, float value);
/**
*	Multiply matrix and vector.
*
*	@param matrix matrix to be multiplied.
*	@param vector vector to be multiplied.
*	@return Vector containing result of multiplication
*/
Vector4		operator*(const Matrix4& matrix, const Vector4& vector);

/**
*	Add each corresponding element of the one matrix to the other one.
*
*	@param matrixL first matrix to be added.
*	@param matrixR second matrix to be added.
*	@return Matrix containing result of this operation.
*/
Matrix4		operator+(const Matrix4& matrixL, const Matrix4& matrixR);
/**
*	Add to each element of matrix given value.

*	@param matrixL matrix to which value will be added.
*	@param value the value to be added.
*	@return Matrix containing result of this operation.
*/
Matrix4		operator+(const Matrix4& matrixL, float value);

/**
*	Subtract each corresponding element of the right matrix from the left one.
*
*	@param matrixL matrix from which other one will be subtracted.
*	@param matrixR matrix which will be subtracted.
*	@return Matrix containing result of this operation.
*/
Matrix4		operator-(const Matrix4& matrixL, const Matrix4& matrixR);
/**
*	Subtract from each element of the matrix given value.
*
*	@param matrixL matrix from which the value will be subtracted.
*	@param value the value to be subtracted.
*	@return Matrix containing result of this operation.
*/
Matrix4		operator-(const Matrix4& matrixL, float value);

/**
*	Divide each element of matrix by given value and store result in the matrix.
*
*	@param matrix matrix that will be divided.
*	@param value dividing value.
*	@return Reference to the object calling this operator.
*/
Matrix4		operator/(const Matrix4& matrix, float value);

/**
*	Check if two matrixes are equal.
*
*	@param matrixL first matrix to check.
*	@param matrixL second matrix to check.
*	@return True if matrixes are equal, false if not.
*/
bool		operator==(const Matrix4& matrixL, const Matrix4& matrixR);
/**
*	Check if two matrixes are not equal.
*
*	@param matrixL first matrix to check.
*	@param matrixL second matrix to check.
*	@return True if matrixes are not equal, false if they are.
*/
bool		operator!=(const Matrix4& matrixL, const Matrix4& matrixR);

/**
*	Overload operator << for use in std::cout to conver matrix to string.
*
*	@param os reference to ostream stream.
*	@param matrixL matrix for conversion
*	@return stream with added string.
*/
std::ostream& operator<<(std::ostream &os, const Matrix4& matrix);
}