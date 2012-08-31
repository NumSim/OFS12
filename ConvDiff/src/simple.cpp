#include <iostream>

#include <stdio.h>
#include <math.h>

#include "simple.h"
#include "data.h"
using namespace std;

bool simple(sData* data) {

	cout << "\nCalculation:\n------------\n";
	initPressureField(data);

	for (int i = 0; i < 800; i++) {
		cout << "i= "<<i << endl;
		initCoefficient(data);
		//cout << " 1 "<<flush;
		solveImpulseForVelocity(data);
		//cout << " here "<<flush;
		solvePressureCorrection(data);
		//cout << " here "<<flush;
		updatePressure(data);
		//cout << " here "<<flush;
		updateVelocity(data);
		//cout << " asdasdasd "<<flush;
	}
	collectResults(data);

	return true;
}

void collectResults(sData* data) {
	sFace * f;
	for (int i = 0; i < data->faceNo; i++) {
		f = &data->faces[i];
		f->uv[0] = f->uvStar[0];
		f->uv[1] = f->uvStar[1];
	}
	sCell * c;
	for (int i = 0; i < data->cellNo; i++) {
		c = &data->cells[i];
		c->p = c->pStar;
	}

}
void initCoefficient(sData* data) {

	double aSouth, aNorth, aEast, aWest, aTildeE, aTildeN;
	double b;
	double deltaX = 0.1;//data->deltaX;
	double deltaY = 0.1;//data->deltaY;
	double Pex, Pey, f, g, dx, dy;
	double APEXABS, APEYABS;
	double Ae, An;
	Ae = deltaY;
	An = deltaX;

	sFace* curFace;
	for (int i = 0; i < data->faceNo; i++) {
		curFace = &data->faces[i];

		Pex = data->rho * curFace->uv[0] * 1 / data->alpha; // ersetze 1 durch dimX
		Pey = data->rho * curFace->uv[1] * 1 / data->alpha;

		f = data->rho * curFace->uv[0];
		g = data->rho * curFace->uv[1];
		dx = data->alpha / deltaX;
		dy = data->alpha / deltaY;

		APEXABS = ABS(Pex) / (exp(ABS(Pex) - 1));
		APEYABS = ABS(Pey) / (exp(ABS(Pey) - 1));

		aEast = dx * deltaY * APEXABS + MAX(-f*deltaY,0);
		aWest = dx * deltaY * APEXABS + MAX(f*deltaY,0);
		aNorth = dy * deltaX * APEYABS + MAX(-g*deltaX,0);
		aSouth = dy * deltaX * APEYABS + MAX(g*deltaX,0);
		curFace->aEast = aEast;
		curFace->aWest = aWest;
		curFace->aNorth = aNorth;
		curFace->aSouth = aSouth;
	}
}

void initPressureField(sData* data) {
	sCell* curCell;
	for (int i = 0; i < data->cellNo; i++) {
		curCell = &data->cells[i];
		if (curCell->bType != 1) {
			curCell->pStar = 0;
			curCell->pPrime = 0;
		} else {
			curCell->pStar = curCell->p;
			curCell->pPrime = 0;
		}
	}
}

void solveImpulseForVelocity(sData* data) {
// Solve equation 7.1-1, 7.1-2, 7.1-3

	double aSouth, aNorth, aEast, aWest, aTildeE, aTildeN;
	double b;
	double deltaX = data->deltaX;
	double deltaY = data->deltaY;
	double Ae, An;
	Ae = deltaY;
	An = deltaX;
	double amue = data->rho * data->deltaX * data->deltaY / data->deltaT;
	sFace* curFace;
	for (int i = 0; i < data->faceNo; i++) {
		curFace = &data->faces[i];
		aEast = curFace->aEast;
		aWest = curFace->aWest;
		aSouth = curFace->aSouth;
		aNorth = curFace->aNorth;

		if (curFace->bType != 1 && curFace->bType != 2) {

			if (curFace->deltaxy[1] == 0) {
				//
				//  ----- face
				//
				curFace->uvStar[0] = 0; // no u velocity;

				aTildeN = curFace->nCells[1]->cFaces[NORTH]->aEast
						+ curFace->nCells[1]->cFaces[NORTH]->aWest
						+ curFace->nCells[1]->cFaces[NORTH]->aSouth
						+ curFace->nCells[1]->cFaces[NORTH]->aNorth + amue;
				b = amue * curFace->uvStar[1];


				cout << "---faceNo=" << i <<endl;
				cout << "b=" << b << endl;

				curFace->uvStar[1] =
						(aSouth * curFace->nCells[0]->cFaces[SOUTH]->uvStar[1]
								- aNorth
										* curFace->nCells[1]->cFaces[NORTH]->uvStar[1]
								+ b
								-(curFace->nCells[1]->pStar
										- curFace->nCells[0]->pStar) * Ae)
								/ aTildeN;
				cout << "uv =" <<curFace->uvStar[0] << " " << curFace->uvStar[1]<<endl;

			} else {
				//   |
				//   | face
				//   |
				curFace->uvStar[1] = 0; // no u velocity;

				aTildeE = curFace->nCells[1]->cFaces[EAST]->aEast
						+ curFace->nCells[1]->cFaces[EAST]->aWest
						+ curFace->nCells[1]->cFaces[EAST]->aSouth
						+ curFace->nCells[1]->cFaces[EAST]->aNorth + amue;
				b = amue * curFace->uvStar[0];

				curFace->uvStar[0] =
						(-aEast * curFace->nCells[0]->cFaces[EAST]->uvStar[1]
								+ aWest
										* curFace->nCells[1]->cFaces[WEST]->uvStar[1]
								+ b
								- (curFace->nCells[1]->pStar
										- curFace->nCells[0]->pStar) * An)
								/ aTildeE;
				cout << "uv =" <<curFace->uvStar[0] << " " << curFace->uvStar[1]<<endl;


			}
		}
	}
}

void solvePressureCorrection(sData* data) {
	double aEast, aWest, aNorth, aSouth, aTildeP;
	//double A = 1;
	sCell* curCell;
	double b;
	sFace* curFace;
	double amue = data->rho * data->deltaX * data->deltaY / data->deltaT;
	for (int i = 0; i < data->cellNo; i++) {

		curCell = &data->cells[i];

		if (curCell->bType == 1) {

		} else if (curCell->cFaces[NORTH]->bType == 2
				|| curCell->cFaces[SOUTH]->bType == 2) {

		} else {

			double A =data->deltaX*data->deltaY;
			/*
			cout << "###################"<<endl;
			cout << "WESTuv= " <<curCell->cFaces[WEST]->uvStar[0]<<endl;
			cout << "EASTuv= " <<curCell->cFaces[EAST]->uvStar[0]<<endl;
			cout << "NORTHuv= " <<curCell->cFaces[NORTH]->uvStar[1]<<endl;
			cout << "SOUTHuv= " <<curCell->cFaces[SOUTH]->uvStar[1]<<endl;
*/
			b = data->rho * A
					* (curCell->cFaces[WEST]->uvStar[0]
							- curCell->cFaces[EAST]->uvStar[0]
							+ curCell->cFaces[SOUTH]->uvStar[1]
							- curCell->cFaces[NORTH]->uvStar[1]);

			cout << "press b " <<b << endl;
			// is atilde = ap tilde ? asssuming this in the following

			double aTildeN, aTildeE, aTildeS, aTildeW;

			curFace = curCell->cFaces[NORTH];

			aTildeN = curFace->nCells[1]->cFaces[NORTH]->aEast
					+ curFace->nCells[1]->cFaces[NORTH]->aWest
					+ curFace->nCells[1]->cFaces[NORTH]->aSouth
					+ curFace->nCells[1]->cFaces[NORTH]->aNorth + amue;
			curFace = curCell->cFaces[EAST];

			aTildeE = curFace->nCells[1]->cFaces[EAST]->aEast
					+ curFace->nCells[1]->cFaces[EAST]->aWest
					+ curFace->nCells[1]->cFaces[EAST]->aSouth
					+ curFace->nCells[1]->cFaces[EAST]->aNorth + amue;

			curFace = curCell->cFaces[WEST];
			aTildeW = curFace->nCells[0]->cFaces[WEST]->aEast
					+ curFace->nCells[0]->cFaces[WEST]->aWest
					+ curFace->nCells[0]->cFaces[WEST]->aSouth
					+ curFace->nCells[0]->cFaces[WEST]->aNorth + amue;
			curFace = curCell->cFaces[EAST];

			aTildeS = curFace->nCells[0]->cFaces[SOUTH]->aEast
					+ curFace->nCells[0]->cFaces[SOUTH]->aWest
					+ curFace->nCells[0]->cFaces[SOUTH]->aSouth
					+ curFace->nCells[0]->cFaces[SOUTH]->aNorth + amue;

			aEast = (A * data->rho * curCell->cFaces[EAST]->deltaxy[1])
					/ aTildeE;
			aNorth = (A * data->rho * curCell->cFaces[NORTH]->deltaxy[0])
					/ aTildeN;
			aWest = (A * data->rho * curCell->cFaces[WEST]->deltaxy[1])
					/ aTildeW;
			aSouth = (A * data->rho * curCell->cFaces[SOUTH]->deltaxy[0])
					/ aTildeS;
			aTildeP = aEast + aWest + aNorth + aSouth;

			curCell->pPrime = (aEast * curCell->nCells[EAST]->pPrime
					+ aWest * curCell->nCells[WEST]->pPrime
					+ aSouth * curCell->nCells[SOUTH]->pPrime
					+ aNorth * curCell->nCells[NORTH]->pPrime +b) / aTildeP;

		}
	}

}

void updatePressure(sData* data) {
	sCell* curCell;

	for (int i = 0; i < data->cellNo; i++) {
		//      cout << "p i = " << i << endl;

		curCell = &data->cells[i];

		if (curCell->bType == 1) {

		} else if (curCell->cFaces[NORTH]->bType == 2
				|| curCell->cFaces[SOUTH]->bType == 2) {

		} else {
			cout << "pStar= " << curCell->pStar << "\tpPrime= "<<curCell->pPrime<<endl;
			curCell->pStar = curCell->pStar + curCell->pPrime;
		}
	}
}

void updateVelocity(sData* data) {
	double deltaX = data->deltaX;
	double deltaY = data->deltaY;
	double Ae, An;
	Ae = deltaX * deltaY;
	An = deltaX * deltaY;

	double aTildeN, aTildeE;

	sFace* curFace;
	for (int i = 0; i < data->faceNo; i++) {
		double amue = data->rho * data->deltaX * data->deltaY / data->deltaT;

		curFace = &data->faces[i];
		if (curFace->bType != 1 && curFace->bType != 2) {

			aTildeN = curFace->nCells[1]->cFaces[NORTH]->aEast
					+ curFace->nCells[1]->cFaces[NORTH]->aWest
					+ curFace->nCells[1]->cFaces[NORTH]->aSouth
					+ curFace->nCells[1]->cFaces[NORTH]->aNorth + amue;
			aTildeE = curFace->nCells[1]->cFaces[EAST]->aEast
					+ curFace->nCells[1]->cFaces[EAST]->aWest
					+ curFace->nCells[1]->cFaces[EAST]->aSouth
					+ curFace->nCells[1]->cFaces[EAST]->aNorth + amue;
			if (curFace->deltaxy[1] == 0) {
				curFace->uvStar[0] = curFace->uvStar[0]
						+ (curFace->nCells[1]->pPrime
								- curFace->nCells[0]->pPrime) * An / aTildeN;

			} else {

				curFace->uvStar[0] = curFace->uvStar[0]
						+ (curFace->nCells[1]->pPrime
								- curFace->nCells[0]->pPrime) * Ae / aTildeE;
			}
		}
	}
}

