// compute data for a simple Kepler problem, comet orbiting a massive object

#include "../lib/numericalMethods.h"
#include <fstream>

// PHYSICAL PARAMETERS

constexpr double PI = 3.141592654;  
constexpr double GM = 4 * PI * PI;
constexpr double m = 1; // mass of comet, unit

double tau = 0.1; // in years
constexpr int nSteps = 200;


// INITIAL CONDITIONS

constexpr double r0 = 1; // AU as unit
constexpr double omega0 = PI / 2; // initial tangential velocity


struct State {

    double r;
    double theta;
    double v;
    double omega;

    State(double r_, double theta_, double v_, double omega_):
        r(r_), theta(theta_), v(v_), omega(omega_) {}

    
    Vector<4> vec() const {

        return Vector<4>({r, theta, v, omega});

    }

    State& operator=(const Vector<4>& vec) {

        r = vec[0], theta = vec[1], v = vec[2], omega = vec[3];

        return *this;

    }

};

Vector<4> f(const Vector<4>& x, const double t) {

    const double r = x[0], theta = x[1], v = x[2], omega = x[3];

    return Vector<4>({v,
                      omega,
                      r * omega * omega - GM / (r * r), 
                      -2 * v * omega / r});

} 

double K(const double r, const double v, const double omega) {

    return .5 * m * (v * v + r * r * omega * omega);

}

double U(const double r) {

    return - GM * m / r;

}

int main() {


    // setup variables

    double t = 0;
    State state({r0, 0, 0, omega0});

    double rs[nSteps], thetas[nSteps];
    double kinetics[nSteps], potentials[nSteps];

    rs[0] = r0, thetas[0] = 0, 
    kinetics[0] = K(r0, 0, omega0), potentials[0] = U(r0);

    // calculating

    for (int i = 1; i < nSteps; i++) {
    
        state = rk4a(state.vec(), t, tau, 0.0001D, f);

        rs[i] = state.r;
        thetas[i] = state.theta;
        kinetics[i] = K(state.r, state.v, state.omega);
        potentials[i] = U(state.r);

    }

    std::ofstream outf;
    outf.open("kepler.txt", std::ios::out | std::ios::trunc);

    outf << std::setprecision(15);

    double totalEn0 = kinetics[0] + potentials[0];
    double totalEnN = kinetics[nSteps - 1] + potentials[nSteps - 1];
    double err = std::abs(totalEnN - totalEn0) / totalEn0;

    std::cout << "total energy error: " << err << std::endl;

    outf << "0 0 0 0 " << nSteps << std::endl;

    for (auto x : rs) outf << x << " ";
    outf << std::endl;
    for (auto x : thetas) outf << x << " ";
    outf << std::endl;
    for (auto x : kinetics) outf << x << " ";
    outf << std::endl;
    for (auto x : potentials) outf << x << " ";
    outf << std::endl;



}