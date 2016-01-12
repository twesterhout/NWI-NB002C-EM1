#ifndef VECTOR_3D_H
#define VECTOR_3D_H

#include <iostream>
#include <tuple>

#include <cmath>


struct vector3D {
	double x, y, z;

	vector3D() : x{0}, y{0}, z{0} {};
	vector3D(double x_, double y_, double z_) : x{x_}, y{y_}, z{z_} {}

	// length in standard L2 metric
	double length() const 
	{
		return sqrt(x*x + y*y + z*z);
	}

};

vector3D operator+(const vector3D& v, const vector3D& w) noexcept 
{
	return vector3D(v.x + w.x, v.y + w.y, v.z + w.z); 
}

vector3D operator-(const vector3D& v, const vector3D& w) noexcept 
{
	return vector3D(v.x - w.x, v.y - w.y, v.z - w.z); 
}

double operator*(const vector3D& v, const vector3D& w) noexcept 
{
	return v.x * w.x + v.y * w.y + v.z * w.z; 
}

vector3D operator*(const double c, const vector3D& w) noexcept 
{
	return vector3D(c * w.x, c * w.y, c * w.z);
}

std::ostream& operator <<(std::ostream& out, const vector3D& point) 
{
	out << point.x << '\t' << point.y << '\t' << point.z;
	return out;
}

template<unsigned int Index>
double cross_product(const vector3D& v, const vector3D& w) 
{
	static_assert(Index < 3, "Cross product is only defined in three dimensions");
	switch(Index) {
		case 0:	return v.y * w.z - v.z * w.y;
		case 1:	return v.z * w.x - v.x * w.z;
		case 2: return v.x * w.y - v.y * w.x;
	}
}


#endif // VECTOR_3D_H
