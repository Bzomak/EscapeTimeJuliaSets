/*
 * complex.h
 *
 * Author - Robert Horswell
 *
 * Header file containing my functions for my complex struct.
 *
 * Last rewritten 31/03/2012
 * 
 */

#ifndef COMPLEX_H
#define COMPLEX_H

typedef struct {
	double real;
	double imaginary;
} Complex;

Complex newComplex (double, double);
Complex addComplex (Complex*, Complex*);
Complex minusComplex (Complex*, Complex*);
Complex cmultComplex (Complex*, Complex*);
Complex rmultComplex (double, Complex*);
Complex imultComplex (double, Complex*);
double modulus (Complex*);
Complex sinComplex (Complex*);
Complex cosComplex (Complex*);
Complex sinhComplex (Complex*);
Complex coshComplex (Complex*);

#endif
