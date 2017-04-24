#pragma once
#include <string>
#include "StringHelper.h"

class Vector2 {
public:
	Vector2() : x(0), y(0) {}
	Vector2(double x, double y) : x(x), y(y) {}
	Vector2 copy(Vector2);

	Vector2 normalize();
	double magnitude();
	Vector2 getRotatedDeg(double angle);
	double getAngleDeg();
	double getAngleRad();

	bool isWithin(Vector2 pos, Vector2 size);

	double x, y;

	Vector2 operator*(const double& f) {
		Vector2 t;
		t.x = this->x * f;
		t.y = this->y * f;
		return t;
	}
	Vector2 operator*=(const double& f) { return *this = *this * f; }

	Vector2 operator/(const double& f) {
		Vector2 t;
		if (f != 0) {
			t.x = this->x / f;
			t.y = this->y / f;
		}
		else {
			t = *this;
		}
		return t;
	}
	Vector2 operator/=(const double& f) { return (*this = *this / f); }

	Vector2 operator+(const Vector2& v) {
		Vector2 t;
		t.x = this->x + v.x;
		t.y = this->y + v.y;
		return t;
	}
	Vector2 operator+=(const Vector2& v) { return *this = *this + v; }
	Vector2 operator-(const Vector2& v) {
		Vector2 t;
		t.x = this->x - v.x;
		t.y = this->y - v.y;
		return t;
	}

	std::string toString() {
		return strh::toString(x) + "," + strh::toString(y);
	}

};