#pragma once
#include <cmath>
#include "json.hpp"

namespace Engine
{
	struct Vector2
	{
		float x;
		float y;

		Vector2() : x(0), y(0) {}
		Vector2(float x, float y) : x(x), y(y) {}

		Vector2 operator+(const Vector2& other) const
		{
			return Vector2(x + other.x, y + other.y);
		}

		Vector2 operator-(const Vector2& other) const
		{
			return Vector2(x - other.x, y - other.y);
		}

		Vector2 operator*(float scalar) const
		{
			return Vector2(x * scalar, y * scalar);
		}

		Vector2 operator/(float scalar) const
		{
			return Vector2(x / scalar, y / scalar);
		}

		Vector2& operator+=(const Vector2& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		Vector2& operator-=(const Vector2& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		Vector2& operator*=(float scalar)
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}

		Vector2& operator/=(float scalar)
		{
			x /= scalar;
			y /= scalar;
			return *this;
		}

		bool operator==(const Vector2& other) const
		{
			return x == other.x && y == other.y;
		}

		bool operator!=(const Vector2& other) const
		{
			return !(*this == other);
		}

		float SquaredMagnitude() const
		{
			return x * x + y * y;
		}

		float Magnitude() const
		{
			return std::sqrt(x * x + y * y);
		}

		Vector2 Normalize() const
		{
			float magnitude = Magnitude();
			if (magnitude == 0)
				return Vector2(0, 0);
			return *this / magnitude;
		}

		float Dot(const Vector2& other) const
		{
			return x * other.x + y * other.y;
		}

		float Cross(const Vector2& other) const
		{
			return x * other.y - y * other.x;
		}

		Vector2 Reflect(const Vector2& normal) const
		{
			return *this - normal * 2 * Dot(normal);
		}

		nlohmann::json Serialize() const
		{
			nlohmann::json seriliazedVector2;
			seriliazedVector2["x"] = x;
			seriliazedVector2["y"] = y;
			return seriliazedVector2;
		}

		void Deserialize(nlohmann::json& json)
		{
			x = json["x"].get<float>();
			y = json["y"].get<float>();
		}
		

		static float Distance(const Vector2& a, const Vector2& b)
		{
			Vector2 dir{ a - b };
			return dir.Magnitude();
		}

		static Vector2 Lerp(const Vector2& a, const Vector2& b, float t)
		{
			return a + (b - a) * t;
		}

		static Vector2 Zero() { return Vector2(0, 0); }
		static Vector2 One() { return Vector2(1, 1); }
		static Vector2 Up() { return Vector2(0, 1); }
		static Vector2 Down() { return Vector2(0, -1); }
		static Vector2 Left() { return Vector2(-1, 0); }
		static Vector2 Right() { return Vector2(1, 0); }

	};
}



