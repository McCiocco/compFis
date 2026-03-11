#ifndef NBODY_LURCH
#define NBODY_LURCH

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
        for (int i = 0; i < N; i++) {

            //usa .array() per abilitare il broadcasting
            VecN dx = x.array() - x[i];
            VecN dy = y.array() - y[i];

            //usa il softening per evitare che due corpi sovrapposti facciano esplodere la simulazione
            VecN dsSq = dx.array().square() + dy.array().square() + soft;

            VecN dsInvCube = dsSq.array().pow(-1.5);

            VecN fx = m.array() * dx.array() * dsInvCube.array();
            VecN fy = m.array() * dy.array() * dsInvCube.array();

            fx[i] = 0;
            fy[i] = 0;

            ax[i] = G * fx.sum(); 
            ay[i] = G * fy.sum(); 

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