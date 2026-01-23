#ifndef NUMERICAL_METHODS_H
#define NUMERICAL_METHODS_H

#include <array>
#include "vec.h"
#include <iomanip>

// Euler method
std::array<double, 2> euler(const double r,
                            const double v,
                            double (*a)(const double, const double),
                            const double tau);

// Euler-Cromer method
std::array<double, 2> eulerCromer(const double r,
                                  const double v,
                                  double (*a)(const double, const double),
                                  const double tau);

// Midpoint method
std::array<double, 2> midpoint(const double r,
                               const double v,
                               double (*a)(const double, const double),
                               const double tau);

// Leapfrog method
std::array<double, 2> leapFrog(const double r,
                               const double v,
                               double (*a)(const double),
                               const double tau);

// Verlet method (single coordinate)
double verletTraj(const double r,
                  const double prevR,
                  double (*a)(const double),
                  const double tau);

// Fourth Order Runge Kutta for a n-variables state
template<const unsigned N>
Vector<N> rk4(const Vector<N>& x,
              const double t,
              const double tau,
              Vector<N> (*f)(const Vector<N>&, double)
            );

template<const unsigned N>
Vector<N> rk4(const Vector<N>& x,
              const double t,
              const double tau,
              Vector<N> (*f)(const Vector<N>&, double)
) {

    Vector<N> nextX;
    auto F1 = f(x, t);
    auto F2 = f(x + .5 * tau * F1, t + .5 * tau);
    auto F3 = f(x + .5 * tau * F2, t + .5 * tau);
    auto F4 = f(x + tau * F3, t + tau);

    nextX = x + 1.0/6 * tau * (F1 + 2 * F2 + 2 * F3 + F4); // careful for integer divion ahahahahahahahahhahahahahaahhahaahhaahh

    return nextX; 
                        
}

template<const unsigned N>
Vector<N> rk4a(const Vector<N>& x,
               double& t,
               double& tau,
			   const double di, //precision
               Vector<N> (*f)(const Vector<N>&, double)
) {

	
	int tries = 0;
	Vector<N> xb, xs;
	double dc;
	do {
		
		xb = rk4(x, t, tau, f);
		xs = rk4(rk4(x, t, tau / 2, f), t + tau / 2, tau / 2, f);
		dc = (xb - xs).norm();

		double tauEst = tau * std::pow(di / dc, 1.0/5);
		const double S1 = 0.9;
		const double S2 = 4.;

		double tauNew;
		tauNew = std::min(S2 * tau, S1 * tauEst);
		tauNew = std::max(tauNew, tau / S2);

		tau = tauNew;
		tries++;

	} while (tries < 100 && di > dc);

	t += tau;
	return xs;

}


#endif // NUMERICAL_METHODS_H
