#include "numericalMethods.h"

std::array<double, 2> euler(const double r, 
                       const double v, 
                       double (*a)(const double, const double), 
                       const double tau
){

    double newV = v + tau * a(r, v);
    double newR = r + tau * v;

    return {newV, newR};

}

std::array<double, 2> eulerCromer(const double r, 
                       const double v, 
                       double (*a)(const double, const double), 
                       const double tau
){

    double newV = v + tau * a(r, v);
    double newR = r + tau * newV;

    return {newV, newR};

}

std::array<double, 2> midpoint(const double r, 
                       const double v, 
                       double (*a)(const double, const double), 
                       const double tau
){

    double newV = v + tau * a(r, v);
    double newR = r + tau * .5 * (v + newV);

    return {newR, newV};

}

std::array<double, 2> leapFrog(const double r, 
                       const double v, 
                       double (*a)(const double), 
                       const double tau
){

    double newV = v + 2 * tau * a(r);
    double newR = r + 2 * tau * newV;

    return {newR, newV};

}

double verletTraj(const double r, 
                  const double prevR, 
                  double (*a)(const double), 
                  const double tau
){

    return 2 * r - prevR + a(r) * tau * tau;

}




