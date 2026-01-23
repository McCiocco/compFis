#include "../lib/numericalMethods.h"
#include <fstream>

//PHYSICAL PARAMETERS

constexpr double PI = 3.141592654;

constexpr double g = 9.81;
constexpr double m = 1;
constexpr double M = 8;
constexpr double l1 = 0.7;
constexpr double l2 = 0.705;
constexpr double tau = 0.001;
constexpr unsigned nSteps = 200000;
constexpr double attrT = 0.05;
constexpr double attrS = 0.05;

//INITIAL CONDITIONS

constexpr double x0 = 0;
constexpr double theta1_0 = 5 * PI / 180;
constexpr double theta2_0 = -2 * PI / 180;

struct State {

    double x, v;
    double theta1, theta2;
    double omega1, omega2;

    State(): x(x0), v(0), theta1(theta1_0), theta2(theta2_0),
            omega1(0), omega2(0) {};

    Vector<6> vec() const {

        return Vector<6>({x, theta1, theta2, v, omega1, omega2});

    }

    State& operator=(const Vector<6>& vec) {

        x = vec[0], theta1 = vec[1], theta2 = vec[2];
        v = vec[3], omega1 = vec[4], omega2 = vec[5];

        return *this;

    }

};

Vector<6> f(const Vector<6>& s, double t) {

    Vector<6> res;
    double v = s[3], omega1 = s[4], omega2 = s[5];
    double theta1 = s[1], theta2 = s[2];
    res[0] = v;
    res[1] = omega1;
    res[2] = omega2;

    double cosTheta1 = std::cos(theta1), cosTheta2 = std::cos(theta2);
    double sinTheta1 = std::sin(theta1), sinTheta2 = std::sin(theta2);

    double A = M + 2*m - m * cosTheta1 * cosTheta1 - m * cosTheta2 * cosTheta2;

    // double B = m * g * (cosTheta1 * sinTheta1 + cosTheta2 * sinTheta2) +
    //            attrT * (omega1 * cosTheta1 / l1 + omega2 * cosTheta2 / l2) +
    //            m * (l1 * omega1 * omega1 * sinTheta1 + l2 * omega2 * omega2 * sinTheta2) -
    //            attrS * v;

     double B = m * g * (cosTheta1 * sinTheta1 + cosTheta2 * sinTheta2) +
               attrT * (omega1 * cosTheta1 / l1 + omega2 * cosTheta2 / l2) +
               m * (l1 * omega1 * omega1 * sinTheta1 + l2 * omega2 * omega2 * sinTheta2);

    res[3] = B / A;

    // res[4] = - res[3] * cosTheta1 / l1 - g * sinTheta1 / l1 - attrT * omega1 / (m * l1 * l1);
    // res[5] = - res[3] * cosTheta2 / l2 - g * sinTheta2 / l2 - attrT * omega2 / (m * l2 * l2);
    res[4] = - res[3] * cosTheta1 / l1 - g * sinTheta1 / l1;
    res[5] = - res[3] * cosTheta2 / l2 - g * sinTheta2 / l2;

    return res;

};

int main() {

    double t = 0;
    State state;

    //double thetas1[nSteps], thetas2[nSteps];
    double* thetas1 = new double[nSteps];
    double* thetas2 = new double[nSteps];


    thetas1[0] = theta1_0, thetas2[0] = theta2_0;

    for (int i = 1; i < nSteps; i++) {

        state = rk4(state.vec(), t, tau, f);

        thetas1[i] = state.theta1;
        thetas2[i] = state.theta2;

    }

    std::ofstream outf;
    outf.open("twoPends.txt", std::ios::out | std::ios::trunc);

    outf << std::setprecision(15);

    outf << nSteps << std::endl;

    for (int i = 0; i < nSteps; i++) outf << thetas1[i] << " ";
    outf << std::endl;
    for (int i = 0; i < nSteps; i++) outf << thetas2[i] << " ";
    outf << std::endl;



}
