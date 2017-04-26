#pragma once
#include <string>
#include "StringHelper.h"

class Vector2I;
class Vector2ST;

class Vector2D {
public:
	Vector2D() : x(0), y(0) {}
	Vector2D(double x, double y) : x(x), y(y) {}
	Vector2D(Vector2I v);
	Vector2D(Vector2ST v);
	Vector2D copy(Vector2D);

	Vector2D normalize();
	double magnitude();
	Vector2D getRotatedDeg(double angle);
	double getAngleDeg();
	double getAngleRad();

	bool isWithin(Vector2D pos, Vector2D size);

	double x, y;

	Vector2D operator*(const double& f) {
		Vector2D t;
		t.x = this->x * f;
		t.y = this->y * f;
		return t;
	}
	Vector2D operator*=(const double& f) { return *this = *this * f; }

	Vector2D operator/(const double& f) {
		Vector2D t;
		if (f != 0) {
			t.x = this->x / f;
			t.y = this->y / f;
		}
		else {
			t = *this;
		}
		return t;
	}
	Vector2D operator/=(const double& f) { return (*this = *this / f); }

	Vector2D operator+(const Vector2D& v) {
		Vector2D t;
		t.x = this->x + v.x;
		t.y = this->y + v.y;
		return t;
	}
	Vector2D operator+=(const Vector2D& v) { return *this = *this + v; }
	Vector2D operator-(const Vector2D& v) {
		Vector2D t;
		t.x = this->x - v.x;
		t.y = this->y - v.y;
		return t;
	}

	std::string toString() {
		return strh::toString(x) + "," + strh::toString(y);
	}

};


class Vector2I {
public:
	Vector2I() : x(0), y(0) {}
	Vector2I(int x, int y) : x(x), y(y) {}
	Vector2I(Vector2D v);
	Vector2I(Vector2ST v);

	Vector2I copy(Vector2I);
	double magnitude();

	bool isWithin(Vector2I pos, Vector2I size);

	int x, y;

	Vector2I operator*(const int& f) {
		Vector2I t;
		t.x = this->x * f;
		t.y = this->y * f;
		return t;
	}
	Vector2I operator*=(const int& f) { return *this = *this * f; }

	Vector2I operator/(const int& f) {
		Vector2I t;
		if (f != 0) {
			t.x = this->x / f;
			t.y = this->y / f;
		}
		else {
			t = *this;
		}
		return t;
	}
	Vector2I operator/=(const int& f) { return (*this = *this / f); }

	Vector2I operator+(const Vector2I& v) {
		Vector2I t;
		t.x = this->x + v.x;
		t.y = this->y + v.y;
		return t;
	}
	Vector2I operator+=(const Vector2I& v) { return *this = *this + v; }
	Vector2I operator-(const Vector2I& v) {
		Vector2I t;
		t.x = this->x - v.x;
		t.y = this->y - v.y;
		return t;
	}

	std::string toString() {
		return strh::toString(x) + "," + strh::toString(y);
	}
};

class Vector2ST {
public:
	Vector2ST() : x(0), y(0) {}
	Vector2ST(size_t x, size_t y) : x(x), y(y) {}
	Vector2ST(Vector2D v);
	Vector2ST(Vector2I v);

	Vector2ST copy(Vector2ST);
	double magnitude();

	bool isWithin(Vector2ST pos, Vector2ST size);

	int x, y;

	Vector2ST operator+(const Vector2ST& v) {
		Vector2ST t;
		t.x = this->x + v.x;
		t.y = this->y + v.y;
		return t;
	}
	Vector2ST operator+=(const Vector2ST& v) { return *this = *this + v; }
	Vector2ST operator-(const Vector2ST& v) {
		Vector2ST t;
		t.x = this->x - v.x;
		t.y = this->y - v.y;
		return t;
	}

	std::string toString() {
		return strh::toString(x) + "," + strh::toString(y);
	}

};

