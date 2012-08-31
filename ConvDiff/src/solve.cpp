/***************************************************************************
 *   Copyright (C) 2006-2011 by  Institute of Combustion Technology        *
 *   jens.henrik.goebbert@itv.rwth-aachen.de                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <iostream>

#include <stdio.h>
#include <math.h>

#include "solve.h"
#include "data.h"
using namespace std;
//------------------------------------------------------
bool solve(sData* data) {
	sCell* curCell = 0;

	cout << "\nCalculation:\n------------\n";

	double dphidt;
	double dt = data->maxTime / data->numberTimeSteps;

	for (int t = 0; t < data->numberTimeSteps; t++) {
		calcFlux(data);
		if (t % 10000 == 0)
			cout << "i= " << t << endl;
		for (int i = 0; i < data->cellNo; i++) {

			curCell = &data->cells[i];
			if (curCell->bType == 1) {
				// skip
			} else if (curCell->bType == 2) {

			} else {

				//WARNING, FLUX DIRECTION ? always left to right?
				//    cout << " delta xy = "<< curCell->cFaces[WEST]->deltaxy[1]<<endl;
				//    cout << " delta xy = "<< curCell->cFaces[NORTH]->deltaxy[0]<<endl;
				//   cout << "factor 1 " << (curCell->cFaces[WEST]->numFlux[0]-
				//      curCell->cFaces[EAST]->numFlux[0])<< endl;
				// cout << " summand 1 = "<< curCell->cFaces[WEST]->numFlux[0]<< endl;
				//  cout << " summand 2 = "<< curCell->cFaces[EAST]->numFlux[0]<< endl;
				//    cout << "factor 1 " << (curCell->cFaces[NORTH]->numFlux[1]-
				//      curCell->cFaces[SOUTH]->numFlux[1])<< endl;

				dphidt = (curCell->cFaces[WEST]->numFlux[0]
						- curCell->cFaces[EAST]->numFlux[0])
						* ABS(curCell->cFaces[WEST]->deltaxy[1])
						+ (-curCell->cFaces[NORTH]->numFlux[1]
								+ curCell->cFaces[SOUTH]->numFlux[1])
								* ABS(curCell->cFaces[NORTH]->deltaxy[0]);

				dphidt /= (curCell->volume * data->rho);
				//
				//   std::cout << dphidt << "\n";
				//    cout << " dphidt = " << dphidt << endl;

				curCell->phi[0] = curCell->phi[0] + dphidt * dt;

			}

		}
	}

	std::cout << "\n";
	return true;
}

//------------------------------------------------------
void calcFlux(sData* data) {
	//std::cout << "in calcFlux\n";
	static sFace* curFace = 0;
	double vel;

	// numerical flux of each face
	for (int fId = 0; fId < data->faceNo; fId++) {
		// std::cout << fId << "\n";
		curFace = &data->faces[fId];
		// check whether or not to update
		if (curFace->bType == 1) {
			// if bType == 1 SKIP FLUX CALC
			// std::cout << fId << "type 1" << std::endl;
		} else if (curFace->bType == 2) {
			//  std::cout << fId << "type 2 " << std::endl;
			// if bType ==2 CONST FLUX
			//curFace->numFlux[1] = 0; // ?
			//curFace->numFlux[0] = 0;
		} else {
			//     cout << " --------------------" << endl;
			// check orientation
			//      cout << curFace->nCells[0]->phi[0] << " "<< curFace->nCells[1]->phi[0] << endl;

			if (curFace->deltaxy[0] == 0) {
				//    |
				//   std::cout << "| \n";

				vel = (curFace->uv[0] < 0) ?
						curFace->nCells[1]->phi[0] : curFace->nCells[0]->phi[0];

				curFace->numFlux[1] = 0;
				curFace->numFlux[0] = data->rho * curFace->uv[0] *
				// (curFace->nCells[0]->phi[0]+curFace->nCells[1]->phi[0])/2
						vel
						- data->alpha
								* (curFace->nCells[1]->phi[0]
										- curFace->nCells[0]->phi[0])
								/ (curFace->nCells[1]->xy[0]
										- curFace->nCells[0]->xy[0]);
				//       cout << " | " << endl;
				//        cout << (curFace->nCells[1]->phi[0]- curFace->nCells[0]->phi[0]) << "<- is this zero \n";
				//  cout << " flux = " << curFace->numFlux[0] << endl;

			} else {
				//    -
				// std::cout << "| \n";
				vel = (curFace->uv[0] < 0) ?
						curFace->nCells[1]->phi[0] : curFace->nCells[0]->phi[0];

				curFace->numFlux[0] = 0;
				curFace->numFlux[1] = data->rho * curFace->uv[1] *
				//(curFace->nCells[0]->phi[0]+curFace->nCells[1]->phi[0])/2
						vel
						- data->alpha
								* (curFace->nCells[1]->phi[0]
										- curFace->nCells[0]->phi[0])
								/ (curFace->nCells[1]->xy[1]
										- curFace->nCells[0]->xy[1]);
				//    cout << " -" << endl;
				//    cout << (curFace->nCells[1]->phi[0]- curFace->nCells[0]->phi[0]) << "<- is this zero \n";
				//   cout << " flux = " << curFace->numFlux[0] << endl;

			}

		}
		//curFace = &data->faces[16];
		//std::cout << curFace->numFlux[0] << " " << curFace->numFlux[1] << std::endl;
	}
}

bool solve2(sData* data) {
	sCell* curCell = 0;
	sFace* curFace = 0;

	cout << "\nCalculation:\n------------\n";

	for (int t = 0; t < data->numberTimeSteps; t++) {
		if (t % 1 == 0)
			cout << "i= " << t << endl;
		for (int k = 0; k < 10; k++) //todo till converged
				{
			cout << k << endl;

			double Pex = data->rho * data->uv[0] * 1 / data->alpha; // ersetze 1 durch dimX
			double Pey = data->rho * data->uv[1] * 1 / data->alpha;
			double ap, ae, aw, an, as;
			double f, g, dx, dy;
			f = data->rho * data->uv[0];
			g = data->rho * data->uv[1];
			dx = data->alpha / 1;
			dy = data->alpha / 1;

			double APEXABS, APEYABS;
			APEXABS = ABS(Pex) / (exp(ABS(Pex) - 1));
			APEYABS = ABS(Pey) / (exp(ABS(Pey) - 1));

			double deltaX = 0.333333;
			double deltaY = 0.333333;
			ae = dx * deltaY * APEXABS + MAX(-f*deltaY,0);
			aw = dx * deltaY * APEXABS + MAX(f*deltaY,0);
			an = dy * deltaX * APEYABS + MAX(-g*deltaX,0);
			as = dy * deltaX * APEYABS + MAX(g*deltaX,0);
			ap = ae + aw + an + as;
			cout << ap << endl;

			// all faces?
			for (int i = 0; i < data->faceNo; i++) {
			//	cout << i << endl;
				curFace = &data->faces[i];
			//	cout << " here" << endl;
				if (curFace->bType == 1 || curFace->bType == 2) {
			//		cout << " here " << endl;
				} else {
					double b = 0;
					double deltap = 0;
					if (curFace->deltaxy[0] == 0) {
						// |
						// |
						// |
						curFace->uv[1] = 0;
						curFace->uv[0] = ae
								* curFace->nCells[0]->cFaces[EAST]->uv[0]
								+ aw * curFace->nCells[1]->cFaces[WEST]->uv[0]
								+ b + deltap * deltaY;
						curFace->uv[0] /= ap;

					} else {
						//
						// ----
						//
						curFace->uv[0] = 0;
						curFace->uv[1] = as
								* curFace->nCells[0]->cFaces[SOUTH]->uv[1]
								+ an * curFace->nCells[1]->cFaces[NORTH]->uv[1]
								+ b + deltap * deltaX;
						curFace->uv[1] /= ap;
					}

				}

			}
			// all cells?
			for (int i = 0; i < data->cellNo; i++) {

				curCell = &data->cells[i];

				if (curCell->bType == 1) {
				} else if (curCell->cFaces[NORTH]->bType == 2
						|| curCell->cFaces[SOUTH]->bType == 2) {
				} else {

					double b = 0;
					curCell->phi[0] = b
							+ (ae * curCell->nCells[EAST]->phi[0]
									+ aw * curCell->nCells[WEST]->phi[0]
									+ an * curCell->nCells[NORTH]->phi[0]
									+ as * curCell->nCells[SOUTH]->phi[0]);
					curCell->phi[0] /= ap;

				}

			}

			// pressure
			for (int i = 0; i < data->cellNo; i++) {
				//      cout << "p i = " << i << endl;

				curCell = &data->cells[i];

				if (curCell->bType == 1) {
				} else if (curCell->cFaces[NORTH]->bType == 2
						|| curCell->cFaces[SOUTH]->bType == 2) {
				} else {
					double aep, awp, anp, asp, atildep;
					double A = curCell->cFaces[EAST]->deltaxy[1]
							* curCell->cFaces[NORTH]->deltaxy[0];
					double atilde = 1.0f;
					aep = (A * data->rho * curCell->cFaces[EAST]->deltaxy[1])
							/ atilde;
					anp = (A * data->rho * curCell->cFaces[NORTH]->deltaxy[0])
							/ atilde;
					awp = (A * data->rho * curCell->cFaces[WEST]->deltaxy[1])
							/ atilde;
					asp = (A * data->rho * curCell->cFaces[SOUTH]->deltaxy[0])
							/ atilde;
					atildep = aep + awp + anp + asp;

					double b = data->rho * A
							* (curCell->cFaces[WEST]->uv[0]
									- curCell->cFaces[EAST]->uv[0]
									+ curCell->cFaces[SOUTH]->uv[1]
									- curCell->cFaces[NORTH]->uv[1]);

					double pbar;

					pbar = (awp * curCell->nCells[WEST]->p
							+ aep * curCell->nCells[EAST]->p
							+ anp * curCell->nCells[NORTH]->p
							+ asp * curCell->nCells[SOUTH]->p + b) / atildep;
					curCell->p = pbar + curCell->p; // new = correction + old; 7-11.4
					curCell->pbar = pbar;

				}

			}

			// all faces
			for (int i = 0; i < data->faceNo; i++) {
				//   cout << "f i = " << i << endl << flush;

				curFace = &data->faces[i];

				double asuv, awuv, aeuv, anuv, aetilde;
				double b;
				double deltaPbar;
				double deltaP;
				double deltaX = data->deltaX;
				double deltaY = data->deltaY;
				double Pex, Pey, f, g, dx, dy;
				double APEXABS, APEYABS;

				if (curFace->bType != 1 && curFace->bType != 2) {
					double uvbar;

					Pex = data->rho * curFace->uv[0] * 1 / data->alpha; // ersetze 1 durch dimX
					Pey = data->rho * curFace->uv[1] * 1 / data->alpha;

					f = data->rho * curFace->uv[0];
					g = data->rho * curFace->uv[1];
					dx = data->alpha / 1;
					dy = data->alpha / 1;

					APEXABS = ABS(Pex) / (exp(ABS(Pex) - 1));
					APEYABS = ABS(Pey) / (exp(ABS(Pey) - 1));

					aeuv = dx * deltaY * APEXABS + MAX(-f*deltaY,0);
					awuv = dx * deltaY * APEXABS + MAX(f*deltaY,0);
					anuv = dy * deltaX * APEYABS + MAX(-g*deltaX,0);
					asuv = dy * deltaX * APEYABS + MAX(g*deltaX,0);
					aetilde = aeuv + awuv + anuv + asuv;

					if (curFace->deltaxy[1] == 0) {

						//
						//  ----- face
						//
						b = aetilde * curFace->uv[1];
						deltaP = curFace->nCells[1]->p - curFace->nCells[0]->p; //7-12 7.1-3 //TODO is this correct ? neighbours left right

						curFace->uv[0] = 0;

						uvbar =
								(asuv * curFace->nCells[0]->cFaces[SOUTH]->uv[1]
										+ anuv
												* curFace->nCells[1]->cFaces[NORTH]->uv[1]
										+ b + deltaP * curFace->deltaxy[1])
										/ aetilde;

						deltaPbar = curFace->nCells[1]->pbar
								- curFace->nCells[0]->pbar; //7-12 7.3-1 //TODO is this correct ? neighbours left right ?

						curFace->uv[1] = curFace->uv[1] + uvbar
								+ (deltaPbar * curFace->deltaxy[1] / aetilde);
					} else {

						//   |
						//   | face
						//   |
						b = aetilde * curFace->uv[0];
						deltaP = curFace->nCells[1]->p - curFace->nCells[0]->p; //7-12 7.1-3 //TODO is this correct ? neighbours left right

						curFace->uv[1] = 0;
						uvbar = (awuv * curFace->nCells[0]->cFaces[WEST]->uv[0]
								+ aeuv * curFace->nCells[1]->cFaces[EAST]->uv[0]
								+ b + deltaP * curFace->deltaxy[1]) / aetilde;
						deltaPbar = curFace->nCells[1]->pbar
								- curFace->nCells[0]->pbar;
						curFace->uv[0] = curFace->uv[0] + uvbar
								+ (deltaPbar * curFace->deltaxy[0] / aetilde);
					}
				}

			}
		}
	}

	std::cout << "\n";
	return true;
}

