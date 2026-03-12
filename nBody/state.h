#ifndef NBODY_LURCH
#define NBODY_LURCH

#include "params.h"
#include <Eigen/Dense>

//FAI I BENCHMARK

typedef Eigen::VectorXd VecN;

struct State {

    VecN x, y, vx, vy;
    VecN prevx, prevy;
    VecN ax, ay;
    VecN m;

    State();

    void getAcc();

    void EulerCromer();
    
    void verletBackwardsStep();

    void verlet();
    
};


#endif