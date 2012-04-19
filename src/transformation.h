/*
 * transformation.h
 *
 *  Created on: Apr 17, 2012
 *      Author: thomas
 */

#ifndef TRANSFORMATION_H_
#define TRANSFORMATION_H_


#include "data.h"

// physical domain
double xleftof(double xi, double eta);
double xrightof(double xi, double eta);
double ytopof(double xi, double eta);
double ybottomof(double xi, double eta);


// transformation between coordinate system
double xiof(double x );
double etaof(double x, double y);
double xof( double xi);
double yof( double eta, double xi);

// dxi deta ddxi ddeta as void compute the gradient for  the complete domain
void dxi(sData* data, double** dxidx, double** dxidy);
void deta(sData* data, double** detadx, double** detady);
void ddxi(sData* data, double** ddxidx, double** ddxidy);
void ddeta(sData* data, double** ddetadx, double** ddetady);

#endif /* TRANSFORMATION_H_ */
