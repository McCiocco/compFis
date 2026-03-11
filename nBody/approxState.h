#ifndef NBODY_APPROX_LURCH
#define NBODY_APPROX_LURCH

#include <SFML/Graphics.hpp>
#include <Eigen/Dense>
#include <vector>
#include <iostream>

//FAI I BENCHMARK

constexpr double PI = 3.14159265358979323846;
constexpr int W = 1800, H = 1000;

typedef Eigen::VectorXd VecN;

template<unsigned N, double G, double tau, double soft>
struct State {

    VecN x, y, vx, vy;
    VecN prevx, prevy;
    VecN ax, ay;
    VecN m;

    State(): x(N), y(N), vx(N), vy(N), prevx(N), prevy(N), ax(N), ay(N), m(N) {}

    void getAcc() {

        #pragma omp parallel for
        for (int i = 1; i < N; i++) {

            //usa .array() per abilitare il broadcasting
            double dx = x[0] - x[i];
            double dy = y[0] - y[i];

            //usa il softening per evitare che due corpi sovrapposti facciano esplodere la simulazione
            double dsSq = dx*dx + dy*dy;

            double dsInvCube = std::pow(dsSq, -1.5);

            double fx = m[0] * dx * dsInvCube;
            double fy = m[0] * dy * dsInvCube;

            ax[i] = G * fx; 
            ay[i] = G * fy; 

        }

    }

    void EulerCromer() {

        getAcc();

        vx += ax  * tau;
        vy += ay * tau;

        x += vx * tau;
        y += vy * tau;

    }
    
    void verletBackwardsStep() {

        getAcc();
        prevx = x - vx * tau + .5 * ax * tau * tau;
        prevy = y - vy * tau + .5 * ay * tau * tau;

    }

    void verlet() {

        getAcc();

        prevx.swap(x);
        prevy.swap(y);

        x = 2*prevx - x + ax * tau * tau;
        y = 2*prevy - y + ay * tau * tau;

    }

};


#endif