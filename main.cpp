#include <iostream>
#include <fstream>
#include <map>
#include <type_traits>

#include <cmath>
#include <cctype>
extern "C" {
	#include <gsl/gsl_integration.h>
	#include <gsl/gsl_math.h>
	#include <gsl/gsl_errno.h>
}

#include "gnuplot-iostream.h"
#include "vector3D.h"


#define REL_ERROR 1.E-3
#define ABS_ERROR 1.E-5
#define LIMIT 1000
#define MU0_4_PI 1.E-7
#define CONFIG "config.txt"
#define CURVE_DAT "curve.dat"
#define FIELD_DAT "field.dat"

std::ostream& operator <<(std::ostream& out, const std::tuple<vector3D, vector3D>& field) 
{
	out << std::get<0>(field).x << '\t' << std::get<1>(field).x
	    << '\t' << std::get<0>(field).y << '\t' << std::get<1>(field).y
	    << '\t' << std::get<0>(field).z << '\t' << std::get<1>(field).z;
	return out;
}



class Curve {
public:
	double current; // current through the curve
	const double period;

	Curve(double period_, double current_) : period{period_}, current{current_} {}

	virtual vector3D diff_el(double t) const noexcept =0;
	virtual vector3D parametrize(double t) const noexcept =0;
	virtual bool is_singular(const vector3D& r) const noexcept =0;
	virtual ~Curve() noexcept =default;
};


class Circle : public Curve {
/*
 * Class Circle represents a circle of diameter d in the x-y plane.
 */
private:
	double R;
public:
	Circle(double R_, double current_) : Curve{2*M_PI, current_}, R{R_} {}

	virtual vector3D parametrize(double t) const noexcept override
	{
		return vector3D(R * cos(t), R * sin(t), 0);
	}

	virtual vector3D diff_el(double t) const noexcept override
	{
		return vector3D(-R * sin(t), R * cos(t), 0);
	}

	virtual bool is_singular(const vector3D& r) const noexcept override
	{
		double tmp = sqrt(r.x*r.x + r.y*r.y);
		return (-0.01*R < r.z && r.z < 0.01*R)
			&& (0.985*R < tmp && tmp < 1.01*R);
	}
	virtual ~Circle() noexcept =default;

};


class Coil : public Curve {
private:
	double R;
	double length;
public:
	Coil(double R_, double current_, std::size_t n, double length_) 
		: Curve{n*2*M_PI, current_}, R{R_}, length{length_} 
	{}

	virtual vector3D parametrize(double t) const noexcept override
	{
		return vector3D(R * cos(t), R * sin(t), length * t / period);
	}

	virtual vector3D diff_el(double t) const noexcept override
	{
		return vector3D(-R * sin(t), R * cos(t), length / period);
	}

	virtual bool is_singular(const vector3D& r) const noexcept override
	{
		double tmp = sqrt(r.x*r.x + r.y*r.y);
		return (-0.01*length < r.z && r.z < 1.01*length) 
			&& (0.985*R < tmp && tmp < 1.01*R);
	}

	virtual ~Coil() noexcept =default;

};


struct Params {
	const Curve* curve;
	const vector3D* point;

	Params(const Curve* curve_, const vector3D* point_) : curve{curve_}, point{point_} {}
};

template<int Index>
double integrand(double t, void* params) 
{
	Params* p = static_cast<Params*>(params);
	vector3D r = *(p->point) - p->curve->parametrize(t);
	return p->curve->current * cross_product<Index>(p->curve->diff_el(t), r) / pow(r.length(), 3.);
}


std::tuple<vector3D, vector3D> biot_savart(Curve* curve, const vector3D &point) 
{
	
	vector3D result{0, 0, 0};
	vector3D error{0, 0, 0};
	Params params(curve, &point);

	gsl_integration_workspace* workspace = gsl_integration_workspace_alloc(LIMIT);
	
	gsl_function f = {&integrand<0>, static_cast<void*>(&params)};
	gsl_integration_qag (	&f, 0, curve->period, ABS_ERROR, REL_ERROR, LIMIT, 
				GSL_INTEG_GAUSS41, workspace, &result.x, &error.x);
	f.function = &integrand<1>;
	gsl_integration_qag (	&f, 0, curve->period, ABS_ERROR, REL_ERROR, LIMIT, 
				GSL_INTEG_GAUSS41, workspace, &result.y, &error.y);
	f.function = &integrand<2>;
	gsl_integration_qag (	&f, 0, curve->period, ABS_ERROR, REL_ERROR, LIMIT, 
				GSL_INTEG_GAUSS41, workspace, &result.z, &error.z);

	gsl_integration_workspace_free(workspace);

	return std::tuple<vector3D, vector3D>(MU0_4_PI * result, MU0_4_PI * error);
}


void read_circle(std::ifstream& infile, double& distance, Curve* &curve) {
	std::string str;
	
	infile >> str;				
	if(str != "RADIUS:") {
		std::cerr << "expected 'RADIUS:', but" << str << "was found\n"
			  << "terminating...\n";
		infile.close();
		exit(1);
	}
	double radius = 0;
	infile >> radius;
	std::cout << "\tradius = " << radius << "\n";

	infile >> str;				
	if(str != "CURRENT:") {
		std::cerr << "expected 'CURRENT:', but" << str << "was found\n"
			  << "terminating...\n";
		infile.close();
		exit(1);
	}
	double current = 0;
	infile >> current;
	std::cout << "\tcurrent = " << current << "\n";
	
	distance = radius;
	curve = new Circle(radius, current);
}

void read_coil(std::ifstream& infile, double& distance, Curve* &curve) {
	std::string str;
	infile >> str;				
	if(str != "RADIUS:") {
		std::cerr << "expected 'RADIUS:', but" << str << "was found\n"
			  << "terminating...\n";
		infile.close();
		exit(1);
	}
	double radius = 0;
	infile >> radius;
	std::cout << "\tradius = " << radius << "\n";

	infile >> str;				
	if(str != "CURRENT:") {
		std::cerr << "expected 'CURRENT:', but" << str << "was found\n"
			  << "terminating...\n";
		infile.close();
		exit(1);
	}
	double current = 0;
	infile >> current;
	std::cout << "\tcurrent = " << current << "\n";

	infile >> str;				
	if(str != "NR_TURNS:") {
		std::cerr << "expected 'NR_TURNS:', but" << str << "was found\n"
			  << "terminating...\n";
		infile.close();
		exit(1);
	}
	std::size_t nr_turns = 0;
	infile >> nr_turns;
	std::cout << "\tnr_turns = " << nr_turns << "\n";

	infile >> str;				
	if(str != "LENGTH:") {
		std::cerr << "expected 'LENGTH:', but" << str << "was found\n"
			  << "terminating...\n";
		infile.close();
		exit(1);
	}
	double length = 0;
	infile >> length;
	std::cout << "\tlength = " << length << "\n";

	curve = new Coil(radius, current, nr_turns, length);
	distance = radius + 0.5 * length;
}

void read_range(std::ifstream& infile, const std::string& pre, double& min, double& max, double& step) 
{
	std::string str;
	infile >> str;
	if(str != pre + "_MIN:") {
		std::cerr << "expected '"<< pre << "_MIN:', but" << str << "was found\n"
			  << "terminating...\n";
		infile.close();
		exit(1);
	}
	infile >> min;
	std::cout << '\t' << static_cast<char>(tolower(pre[0])) << "_min = " << min << "\n";

	infile >> str;
	if(str != pre + "_MAX:") {
		std::cerr << "expected '"<< pre << "_MAX:', but" << str << "was found\n"
			  << "terminating...\n";
		infile.close();
		exit(1);
	}
	infile >> max;
	std::cout << '\t' << static_cast<char>(tolower(pre[0])) << "_max = " << max << "\n";

	infile >> str;
	if(str != pre + "_STEP:") {
		std::cerr << "expected '"<< pre << "_STEP:', but" << str << "was found\n"
			  << "terminating...\n";
		infile.close();
		exit(1);
	}
	infile >> step;
	std::cout << '\t' << static_cast<char>(tolower(pre[0])) << "_step = " << step << "\n";
}


enum class Shape {Circle, Coil};
const std::map<const std::string, Shape> convert_to_shape{
	{"CIRCLE", Shape::Circle},
	{"COIL", Shape::Coil}
};




int main() 
{
	std::ifstream infile;
	infile.open(CONFIG);
	if(!infile.is_open()) {
		std::cerr << "could not open " << CONFIG << "\n"
			  << "terminating...\n";
	}

	std::cout << "Reading " << CONFIG << " ...\n";
	std::string str;
	infile >> str;
	if(str != "SHAPE:") {
		std::cerr << "expected 'SHAPE:', but" << str << "was found\n"
			  << "terminating...\n";
		infile.close();
		exit(1);
	}

	Curve* curve = nullptr;
	infile >> str;
	double distance, max_len;
	double x_min, x_max, x_step;
	double y_min, y_max, y_step;
	double z_min, z_max, z_step;
	try{
		switch(convert_to_shape.at(str.c_str())) {
			case Shape::Circle:
				std::cout << "\tshape = " << "Shape::Circle\n";
				read_circle(infile, distance, curve);
				break;
			case Shape::Coil:
				std::cout << "\tshape = " << "Shape::Coil\n";
				read_coil(infile, distance, curve);
				break;
		}
		read_range(infile, "X", x_min, x_max, x_step);
		read_range(infile, "Y", y_min, y_max, y_step);
		read_range(infile, "Z", z_min, z_max, z_step);

		infile>>str;
		if(str != "MAX_LEN:") {
			std::cerr << "expected 'MAX_LEN:', but" << str << "was found\n"
				  << "terminating...\n";
			infile.close();
			exit(1);
		}
		infile >> max_len;
		std::cout << "max_len = " << max_len << "\n";
	} catch(const std::out_of_range& e) {
		std::cerr << e.what() << "\n";
		std::cerr << "Unknown shape '" << str << "'\n"
			  << "terminating...\n";
		exit(1);
	}
	infile.close();
	std::cout << "Done reading.\n\n";
	
	std::ofstream outfile;
	outfile.open(FIELD_DAT);
	
	outfile << "#x\ty\tz\tBx\tBx_err\tBy\tBy_err\tBz\tBz_err\n";
	int percent_done = 0;
	std::cout << "\rCalculating field: " << percent_done << "%" << std::flush;
	double step = 0.17*distance;
	double max_field = 0;
	for(double x = x_min; x <= x_max; x += x_step) {
		for(double y = y_min; y <= y_max; y += y_step) {
			for(double z = z_min; z <= z_max; z += z_step) {
				vector3D point{x, y, z};
			
				if( !curve->is_singular(point) ) {
					std::tuple<vector3D, vector3D> 
						field = biot_savart(curve, point);
					if(std::get<0>(field).length() > max_field) 
						max_field = std::get<0>(field).length();
					outfile << point << '\t' << field << '\n';
				}
			}
			if(y_max-y_min)
				percent_done = static_cast<int>(100*(x-x_min)/(x_max-x_min) * (y-y_min)/(y_max-y_min));
			else if(x_max-x_min)
				percent_done = static_cast<int>(100*(x-x_min)/(x_max-x_min));
			std::cout << "\rCalculating field: " << percent_done << "%" << std::flush;
		}
	}
	std::cout << "\rCalculating field: 100%.\n\n";
	outfile.close();

	std::cout << "Saving the curve to " << CURVE_DAT << " ...\n";
	outfile.open(CURVE_DAT);
	for(double t=0; t<curve->period; t+=step) {
		outfile << curve->parametrize(t) << std::endl;
	}
	outfile.close();
	delete curve;
	std::cout << "Done saving.\n\n";

	Gnuplot gp;
	gp << "set terminal wxt size 1200, 1200\n";
	gp << "set xlabel 'x[m]' font ',14'\n"
	   << "set ylabel 'z[m]' font ',14'\n"
	   << "set key font ',14'\n";
	
	gp << "plot[" << -3*distance << ":" << 3*distance << "]" 
		<<"[" << -3*distance << ":" << 3*distance << "] "
		<<"'"<< FIELD_DAT << "' using 1:3:(" << max_len / max_field <<" * $4)"
				   << ":(" << max_len / max_field <<" * $8) with vectors "
		<<"lc rgb 'dark-green' title 'field',"

		<< "'" << CURVE_DAT <<"' using 1:3 with lines lc rgb '#FF763A' title 'curve'\n";
	
	return 0;
}




