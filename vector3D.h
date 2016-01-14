#ifndef VECTOR_3D_H
#define VECTOR_3D_H

#include <iostream>
#include <tuple>
#include <cmath>


class vector3D {
private:
	double size;
	double x, y, z;
public:
	vector3D() : x{0}, y{0}, z{0} {};
	vector3D(double x_, double y_, double z_) : x{x_}, y{y_}, z{z_} {
		size = sqrt(x_*x_ + y_*y_ + z_*z_); 
	}

	vector3D(const vector3D&) = delete;
	vector3D(vector3D&&) = default;

	vector3D& operator=(vector3D&&) = default;
	vector3D& operator=(const vector3D&) = delete;

	// length in standard L2 metric
	double length() const 
	{
		return size;
	}
	
	friend vector3D operator+(const vector3D& v, const vector3D& w) noexcept;
	friend vector3D operator-(const vector3D& v, const vector3D& w) noexcept;
	friend double operator*(const vector3D& v, const vector3D& w) noexcept;
	friend vector3D operator*(const double c, const vector3D& w) noexcept;
	friend std::ostream& operator <<(std::ostream& out, const vector3D& point); 
	
	template<unsigned int Index>
	friend double cross_product(const vector3D& v, const vector3D& w) noexcept;

	template<unsigned int Index>
	friend double& get(vector3D& v) noexcept;

	template<unsigned int Index>
	friend double get(const vector3D& v) noexcept;
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
double cross_product(const vector3D& v, const vector3D& w) noexcept
{
	static_assert(Index < 3, "Cross product is only defined in three dimensions");
	switch(Index) {
		case 0:	return v.y * w.z - v.z * w.y;
		case 1:	return v.z * w.x - v.x * w.z;
		case 2: return v.x * w.y - v.y * w.x;
	}
}

template<unsigned int Index>
double& get(vector3D& v) noexcept 
{
	static_assert(Index < 3, "vector3D is a 3 dimensional vector");
	switch(Index) {
		case 0: return v.x;
		case 1: return v.y;
		case 2: return v.z;
	}
}

template<unsigned int Index>
double get(const vector3D& v) noexcept 
{
	static_assert(Index < 3, "vector3D is a 3 dimensional vector");
	switch(Index) {
		case 0: return v.x;
		case 1: return v.y;
		case 2: return v.z;
	}
}

#endif // VECTOR_3D_H
