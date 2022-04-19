#pragma once

#include <vector>
#include <cmath>
#include "math.h"

const int NUM_OF_SPLINE_POINTS = 100;




class Spline {

private:

	std::vector<double> splinePointsX;
	std::vector<double> splinePointsY;

    double getBezierPoint(double n1, double n2, double perc)
    {
        double diff = n2 - n1;

        return n1 + (diff * perc);
    }

public:


	Spline(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {

        double xa;
        double ya;
        double xb;
        double yb;
        double xc;
        double yc;
        double xm;
        double ym;
        double xn;
        double yn;
        double t;

        for (int i = 0; i < NUM_OF_SPLINE_POINTS; ++i)
        {
            t = i * 1.0 / NUM_OF_SPLINE_POINTS;

            xa = getBezierPoint(x1, x2, t);
            ya = getBezierPoint(y1, y2, t);
            xb = getBezierPoint(x2, x3, t);
            yb = getBezierPoint(y2, y3, t);
            xc = getBezierPoint(x3, x4, t);
            yc = getBezierPoint(y3, y4, t);

            xm = getBezierPoint(xa, xb, t);
            ym = getBezierPoint(ya, yb, t);
            xn = getBezierPoint(xb, xc, t);
            yn = getBezierPoint(yb, yc, t);

            this->splinePointsX.push_back(getBezierPoint(xm, xn, t));
            this->splinePointsY.push_back(getBezierPoint(ym, yn, t));
        }

	}

    double getX(double t) {
        int index = (int)fmin(t * NUM_OF_SPLINE_POINTS, splinePointsX.size()-1);
        return this->splinePointsX[index];

    }

    double getY(double t) {
        int index = (int)fmin(t * NUM_OF_SPLINE_POINTS, splinePointsY.size() - 1);
        return this->splinePointsY[index];
    }

    double getH(double t) {
        int index = (int)(t * NUM_OF_SPLINE_POINTS);

        if (index >= (NUM_OF_SPLINE_POINTS - 1)) {
            index = NUM_OF_SPLINE_POINTS - 2;
        }

        double theta = atan2(splinePointsY[index + 1] - splinePointsY[index], splinePointsX[index + 1] - splinePointsX[index]);

        return theta;


    }

    double getDistance() {
        double d = 0.0;
        for (int i = 1; i < splinePointsX.size(); ++i) {
            d += sqrt((splinePointsX[i] - splinePointsX[i - 1]) * (splinePointsX[i] - splinePointsX[i - 1]) + (splinePointsY[i] - splinePointsY[i - 1])* (splinePointsY[i] - splinePointsY[i - 1]));
        }
        return d;
    }





};