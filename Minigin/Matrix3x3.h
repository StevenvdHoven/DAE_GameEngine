#pragma once
#include <cmath>
#include "Vector2.h"

namespace Engine
{
	struct Matrix3x3
	{
		float matrix[3][3];
		Matrix3x3()
		{
			for (int i = 0; i < 3; ++i)
				for (int j = 0; j < 3; ++j)
					matrix[i][j] = (i == j) ? 1.0f : 0.0f;
		}

		Matrix3x3(float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22)
		{
			matrix[0][0] = m00; matrix[0][1] = m01; matrix[0][2] = m02;
			matrix[1][0] = m10; matrix[1][1] = m11; matrix[1][2] = m12;
			matrix[2][0] = m20; matrix[2][1] = m21; matrix[2][2] = m22;
		}

		Matrix3x3 operator*(const Matrix3x3& other) const
		{
			Matrix3x3 result;
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					result.matrix[i][j] = matrix[i][0] * other.matrix[0][j] +
						matrix[i][1] * other.matrix[1][j] +
						matrix[i][2] * other.matrix[2][j];
				}
			}
			return result;
		}

		Engine::Vector2 operator*(const Engine::Vector2& vec) const
		{
			return Engine::Vector2(
				matrix[0][0] * vec.x + matrix[0][1] * vec.y + matrix[0][2],
				matrix[1][0] * vec.x + matrix[1][1] * vec.y + matrix[1][2]);
		}

		static Matrix3x3 CreateTranslationMatrix(const Engine::Vector2& translation)
		{
			return Matrix3x3(1, 0, translation.x,
				0, 1, translation.y,
				0, 0, 1);
		}

		static Matrix3x3 CreateRotationMatrix(float angle)
		{
			float cosTheta = std::cos(angle);
			float sinTheta = std::sin(angle);
			return Matrix3x3(cosTheta, -sinTheta, 0,
				sinTheta, cosTheta, 0,
				0, 0, 1);
		}
	};
}



