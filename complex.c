/*
 * complex.c
 *
 * Author - Robert Horswell
 *
 * C file containing my functions for my complex struct.
 *
 * Last rewritten 31/03/2012
 * 
 */

#include "complex.h"
#include <math.h>

Complex newComplex (double real, double imaginary) {

	Complex out = {real, imaginary};
	return out;
}

Complex addComplex (Complex *a, Complex *b) {

	Complex out = {a->real + b->real, a->imaginary + b->imaginary};
	return out;
}

Complex minusComplex (Complex *a, Complex *b) {

	Complex out = {a->real - b->real, a->imaginary - b->imaginary};
	return out;
}

Complex cmultComplex (Complex *a, Complex *b) {

	Complex out = {a->real * b->real - a->imaginary * b->imaginary, a->real * b->imaginary + a->imaginary * b->real};
	return out;
}

Complex rmultComplex (double r, Complex *a) {

	Complex out = {a->real * r, a->imaginary * r};
	return out;
}

Complex imultComplex (double i, Complex *a) {

	Complex out = {-a->imaginary * i, a->real * i};
	return out;
}

double modulus (Complex *a) {

	return sqrt (a->real * a->real + a->imaginary * a->imaginary);
}

Complex sinComplex (Complex *a) {

	Complex out = {sin (a->real) * cosh (a->imaginary), cos (a->real) * sinh (a->imaginary)};
	return out;
}

Complex cosComplex (Complex *a) {

	Complex out = {cos (a->real) * cosh (a->imaginary), -(sin (a->real) * sinh (a->imaginary))};
	return out;
}

Complex sinhComplex (Complex *a) {

	Complex out = {sinh (a->real) * cos (a->imaginary), cosh (a->real) * sin (a->imaginary)};
	return out;
}

Complex coshComplex (Complex *a) {

	Complex out = {cosh (a->real) * cos (a->imaginary), sinh (a->real) * sin (a->imaginary)};
	return out;
}
