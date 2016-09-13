#include "Matrix4.h"
#include "Assert.h"

namespace vengine {
/**************************************************************************
*
*								Static constants
*
***************************************************************************/
const Matrix4 Matrix4::identity = {
	{ 1.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f }
};

const Matrix4 Matrix4::zeroes = {
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f }
};

/**************************************************************************
*
*								Constructors
*
***************************************************************************/

Matrix4::Matrix4()
{
}


Matrix4::Matrix4(float value)
{
	(*this) = value;
}


Matrix4::Matrix4(const Matrix4& matrix)
{
	(*this) = matrix;
}


Matrix4::Matrix4(const Vector4* vectors)
{
	(*this) = vectors;
}


Matrix4::Matrix4(const Vector4& c1, const Vector4& c2, const Vector4& c3, const Vector4& c4)
{
	_columns[0] = c1;
	_columns[1] = c2;
	_columns[2] = c3;
	_columns[3] = c4;
}



/**************************************************************************
*
*								Operators
*
***************************************************************************/

/**************************************************************************
*								Operator =
***************************************************************************/

Matrix4&
Matrix4::operator=(const Matrix4& source)
{
	for (int w = 0; w < 4; ++w)
		_columns[w] = source[w];

	return *this;
}


Matrix4&
Matrix4::operator=(const Vector4* vectors)
{
	for (int w = 0; w < 4; ++w)
		_columns[w] = vectors[w];

	return *this;
}


Matrix4&
Matrix4::operator=(float value)
{
	for (int w = 0; w < 4; ++w)
		_columns[w] = value;

	return *this;
}


/**************************************************************************
*								Operator *=
***************************************************************************/

Matrix4&
Matrix4::operator*=(const Matrix4& matrixR)
{
	*this = *this * matrixR;

	return *this;
}


Matrix4&
Matrix4::operator*=(float value)
{
	for (int w = 0; w < 4; ++w)
		_columns[w] *= value;

	return *this;
}


/**************************************************************************
*								Operator +=
***************************************************************************/

Matrix4&
Matrix4::operator+=(const Matrix4& matrix)
{
	for (int w = 0; w < 4; ++w)
		_columns[w] += matrix[w];

	return *this;
}


Matrix4&
Matrix4::operator+=(float value)
{
	for (int w = 0; w < 4; ++w)
		_columns[w] += value;

	return *this;
}


/**************************************************************************
*								Operator -=
***************************************************************************/

Matrix4&
Matrix4::operator-=(const Matrix4& matrix)
{
	for (int w = 0; w < 4; ++w)
		_columns[w] -= matrix[w];

	return *this;
}


Matrix4&
Matrix4::operator-=(float value)
{
	for (int w = 0; w < 4; ++w)
		_columns[w] -= value;

	return *this;
}


Matrix4&
Matrix4::operator/=(float value)
{
	assert(value != 0.0f, "Matrix 4: operator /= - dividing by 0");
	for (int w = 0; w < 4; ++w)
		_columns[w] /= value;

	return *this;
}
/**************************************************************************
*							Get/set operators
***************************************************************************/

Vector4&
Matrix4::operator[](int item)
{

	assert(between(item, 0, 3), "Matrix 4: opearator [] - index out of bound");
	return _columns[item];
}


const Vector4&
Matrix4::operator[](int item) const
{
	assert(between(item, 0, 3), "Matrix 4: opearator [] - index out of bound");
	return _columns[item];
}


/**************************************************************************
*							Conversion operators
***************************************************************************/

Matrix4::operator float*()
{
	return (float *)_columns;
}

Matrix4::operator const float*() const
{
	return (float *)_columns;
}

/**************************************************************************
*
*								Methods
*
***************************************************************************/

/**************************************************************************
*							Transformation methods
***************************************************************************/


Matrix4&
Matrix4::Translate(float x, float y, float z)
{
	Matrix4 tmp = GetTranslate(x, y, z) * (*this);
	*this = tmp;

	return *this;
}


Matrix4&
Matrix4::Translate(const Vector3& vector)
{
	Matrix4 tmp = GetTranslate(vector) * (*this);
	*this = tmp;

	return *this;
}


Matrix4&
Matrix4::Scale(float x, float y, float z)
{
	Matrix4 tmp = GetScale(x, y, z) * (*this);
	*this = tmp;

	return *this;
}


Matrix4&
Matrix4::Scale(const Vector3& vector)
{
	Matrix4 tmp = GetScale(vector) * (*this);
	*this = tmp;

	return *this;
}

Matrix4&
Matrix4::LookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	Matrix4 tmp = GetLookAt(eye, target, up) * (*this);
	*this = tmp;

	return *this;
}


/**************************************************************************
*							Other const methods
***************************************************************************/

std::string
Matrix4::ToString() const
{
	std::string vectorString;
	for (int y = 0; y < 4; ++y) {
		vectorString += '|';
		for (int x = 0; x < 4; ++x) {
			vectorString += std::to_string(_columns[x][y]);
			if (x < 3)
				vectorString += ",\t";
		}

		vectorString += "|\n";
	}

	return vectorString;
}


/**************************************************************************
*
*							Static methods
*
***************************************************************************/

/**************************************************************************
*						Get transformation matrix
***************************************************************************/


Matrix4
Matrix4::GetTranslate(float x, float y, float z)
{
	return GetTranslate(Vector3(x, y, z));
}


Matrix4
Matrix4::GetTranslate(const Vector3& vector)
{
	Matrix4 translate = identity;
	translate[3] = Vector4(vector, 1.0f);

	return translate;
}


Matrix4
Matrix4::GetScale(float x, float y, float z)
{
	return Matrix4(
	{ x,		0.0f,	0.0f,	0.0f },
	{ 0.0f,	y,		0.0f,	0.0f },
	{ 0.0f,	0.0f,	z,		0.0f },
	{ 0.0f,	0.0f,	0.0f,	1.0f }
	);
}


Matrix4
Matrix4::GetScale(const Vector3& vector)
{
	return GetScale(vector[0], vector[1], vector[2]);
}

Matrix4
Matrix4::GetLookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	Vector3 zaxis = Vector3::Normalized(eye - target);
	Vector3	xaxis = Vector3::Normalized(Vector3::Cross(up, zaxis));
	Vector3	yaxis = Vector3::Cross(zaxis, xaxis);

	return Matrix4(
	{ xaxis.x, yaxis.x, zaxis.x, 0.0f },
	{ xaxis.y, yaxis.y, zaxis.y, 0.0f },
	{ xaxis.z, yaxis.z, zaxis.z, 0.0f },
	{ -Vector3::Dot(xaxis,eye), -Vector3::Dot(yaxis,eye), -Vector3::Dot(zaxis,eye), 1.0f }
	);

}

Matrix4
Matrix4::GetFrustum(float left, float right, float bottom, float top, float near, float far)
{
	assert(left != right, "Matrix 4: frustum - left == right");
	assert(top != bottom, "Matrix 4: frustum - top == bottom");
	assert(near != far, "Matrix 4: frustum - near == far");

	return Matrix4(
	{ 2.0f*near / (right - left), 0.0f, 0.0f, 0.0f },
	{ 0.0f, 2 * near / (top - bottom), 0.0f, 0.0f },
	{ (right + left) / (right - left), (top + bottom) / (top - bottom), (near + far) / (near - far), -1.0f },
	{ 0.0f, 0.0f, 2 * near*far / (near - far), 0.0f });
}

Matrix4
Matrix4::GetPerspective(float fov, float aspect, float near, float far)
{
	assert(fov != 0.0f, "Matrix 4: frustum - fov == 0");
	assert(aspect != 0.0f, "Matrix 4: frustum - aspect == 0");
	assert(near != far, "Matrix 4: frustum - near == far");

	float s = 1.0f / tan(degToRad(fov / 2.0f));
	float p1 = s / aspect;
	float p2 = (near + far) / (near - far);
	float p3 = (2.0f * near * far) / (near - far);

	return Matrix4(
	{ p1, 0.0f, 0.0f, 0.0f },
	{ 0.0f, s, 0.0f, 0.0f },
	{ 0.0f, 0.0f, p2, -1.0f },
	{ 0.0f, 0.0f, p3, 0.0f });

}

Matrix4 
Matrix4::GetOrtho(float left, float right, float bottom, float top, float near, float far)
{
	assert(left != right, "Matrix 4: frustum - left == right");
	assert(top != bottom, "Matrix 4: frustum - top == bottom");
	assert(near != far, "Matrix 4: frustum - near == far");

	return Matrix4({ 2.0f / (right - left), 0.0f, 0.0f, 0.0f },
	{ 0.0f, 2.0f / (top - bottom), 0.0f, 0.0f },
	{ 0.0f, 0.0f, 2.0f / (near - far), 0.0f },
	{ (left + right) / (left - right), (bottom + top) / (bottom - top), (near + far) / (far - near), 1.0f });
}
/**************************************************************************
*
*							Global operators
*
***************************************************************************/

/**************************************************************************
*								Operator +
***************************************************************************/


/**************************************************************************
*								Operator -
***************************************************************************/


/**************************************************************************
*								Operator *
***************************************************************************/


Matrix4
operator*(const Matrix4& matrixL, const Matrix4& matrixR)
{
	Matrix4 result;
	for (int h = 0; h < 4; ++h) {
		for (int k = 0; k < 4; ++k) {
			float sum = 0;
			for (int w = 0; w < 4; ++w) {
				//FMA for better precise during multiplication
				sum = fma(matrixL[w][h], matrixR[k][w], sum);
			}
			result[k][h] = sum;
		}
	}

	return result;
}



Matrix4
operator*(const Matrix4& matrix, float value)
{
	Matrix4 result = matrix;
	result *= value;

	return result;
}



Vector4
operator*(const Matrix4& matrix, const Vector4& vector)
{
	Vector4 result = Vector4::zeroes;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result[i] += vector[j] * matrix[j][i];
		}
	}

	return result;
}



Matrix4
operator+(const Matrix4& matrixL, const Matrix4& matrixR)
{
	Matrix4 result = matrixL;
	result += matrixR;

	return result;
}

Matrix4
operator+(const Matrix4& matrix, float value)
{
	Matrix4 result = matrix;
	result += value;

	return result;
}


Matrix4
operator-(const Matrix4& matrixL, const Matrix4& matrixR)
{
	Matrix4 result = matrixL;
	result -= matrixR;

	return result;
}

Matrix4
operator-(const Matrix4& matrix, float value)
{
	Matrix4 result = matrix;
	result -= value;

	return result;
}

Matrix4
operator/(const Matrix4& matrix, float value)
{
	assert(value != 0.0f, "Matrix 4: operator / - dividing by 0");
	Matrix4 result = matrix;
	result /= value;

	return result;
}

/**************************************************************************
*							Comparing operators
***************************************************************************/
bool
operator==(const Matrix4& matrixL, const Matrix4& matrixR)
{
	return (matrixL[0] == matrixR[0]) && (matrixL[1] == matrixR[1]) &&
		(matrixL[2] == matrixR[2]) && (matrixL[3] == matrixR[3]);
}
bool
operator!=(const Matrix4& matrixL, const Matrix4& matrixR)
{
	return (matrixL[0] != matrixR[0]) || (matrixL[1] != matrixR[1]) ||
		(matrixL[2] != matrixR[2]) || (matrixL[3] != matrixR[3]);
}

std::ostream&
operator<<(std::ostream &os, const Matrix4& matrix)
{
	return os << matrix.ToString();
}

}