#include "Vector.h"
#include <math.h>
#include "Debugger.h"
#include "StringHelper.h"

#define PI 3.14159265
#define DegToRad 0.0174533
#define RadToDeg 57, 2958

Vector2D::Vector2D(Vector2I v) : x((double)v.x), y((double)v.y) {};
Vector2D::Vector2D(Vector2ST v) : x((double)v.x), y((double)v.y) {};


Vector2I::Vector2I(Vector2D v) : x((int)v.x), y((int)v.y) {};
Vector2I::Vector2I(Vector2ST v) : x((int)v.x), y((int)v.y) {};

Vector2ST::Vector2ST(Vector2D v) : x((size_t)v.x), y((size_t)v.y) {};
Vector2ST::Vector2ST(Vector2I v) : x((size_t)v.x), y((size_t)v.y) {};




Vector2D Vector2D::copy(Vector2D v) {
	return Vector2D(v.x, v.y);
}
Vector2D Vector2D::normalize() {
	*this /= this->magnitude();
	return copy(*this);
}
double Vector2D::magnitude() {
	return sqrt(pow(x, 2) + pow(y, 2));
}
Vector2D Vector2D::getRotatedDeg(double angle) {
	double rotAng = getAngleRad();
	rotAng -= angle * DegToRad; //Y is inverted

	Vector2D rotated;
	double mag = magnitude();
	double Sin, Cos;
	if (mag != 0) {
		Cos = cos(rotAng) * mag;
		Sin = sin(rotAng) * mag;
		rotated.y = Cos;
		rotated.x = Sin;
	}
	else return Vector2D(0, 0);
	return rotated;
}
double Vector2D::getAngleDeg() {
	return getAngleRad() * RadToDeg;
}
double Vector2D::getAngleRad() {
	double angle;
	if (x == 0) {
		if (y > 0) return 0;
		else if (y < 0) return PI;
		else return 0;
	}
	else {
		angle = atan(y / x);
	}
	if (y < 0) angle += PI;
	angle = fmod(angle + 2.5 * PI, 2 * PI); //Top of circle is angle = 0
	angle *= -1; //Clockwise direction angle
	angle = fmod(angle + 2 * PI, 2 * PI);
	return angle;
}

bool Vector2D::isWithin(Vector2D pos, Vector2D size) {
	bool result = pos.x <= x && x < pos.x + size.x && pos.y <= y && y < pos.y + size.y;
	return result;
}


Vector2I Vector2I::copy(Vector2I v) {
	return Vector2I(v.x, v.y);
}
double Vector2I::magnitude() {
	return sqrt(pow((double)x, 2) + pow((double)y, 2));
}

bool Vector2I::isWithin(Vector2I pos, Vector2I size) {
	bool result = pos.x <= x && x < pos.x + size.x && pos.y <= y && y < pos.y + size.y;
	return result;
}


Vector2ST Vector2ST::copy(Vector2ST v) {
	return Vector2ST(v.x, v.y);
}
double Vector2ST::magnitude() {
	return sqrt(pow((double)x, 2) + pow((double)y, 2));
}

bool Vector2ST::isWithin(Vector2ST pos, Vector2ST size) {
	bool result = pos.x <= x && x < pos.x + size.x && pos.y <= y && y < pos.y + size.y;
	return result;
}