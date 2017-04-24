#include "Vector.h"
#include <math.h>
#include "Debugger.h"
#include "StringHelper.h"

#define PI 3.14159265
#define DegToRad 0.0174533
#define RadToDeg 57, 2958

Vector2 Vector2::copy(Vector2 v) {
	return Vector2(v.x, v.y);
}
Vector2 Vector2::normalize() {
	*this /= this->magnitude();
	return copy(*this);
}
double Vector2::magnitude() {
	return sqrt(pow(x, 2) + pow(y, 2));
}
Vector2 Vector2::getRotatedDeg(double angle) {
	double rotAng = getAngleRad();
	rotAng -= angle * DegToRad; //Y is inverted

	Vector2 rotated;
	double mag = magnitude();
	double Sin, Cos;
	if (mag != 0) {
		Cos = cos(rotAng) * mag;
		Sin = sin(rotAng) * mag;
		rotated.y = Cos;
		rotated.x = Sin;
	}
	else return Vector2(0, 0);
	return rotated;
}
double Vector2::getAngleDeg() {
	return getAngleRad() * RadToDeg;
}
double Vector2::getAngleRad() {
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

bool Vector2::isWithin(Vector2 pos, Vector2 size) {
	bool result = pos.x < x && x < pos.x + size.x && pos.y < y && y < pos.y + size.y;
	return result;
}