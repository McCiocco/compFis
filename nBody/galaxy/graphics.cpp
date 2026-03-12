#include "graphics.h"
#include <random>
#include <ctime>
#include "omp.h"

Graphics::Graphics(const State& s_): s(s_), bodies(sf::Points, N-1) {

    Eigen::setNbThreads(1);
    omp_set_num_threads(std::max(omp_get_max_threads() - 1, 1));

    sun.setRadius(sunRadius);
    sun.setFillColor(stuff);
    sun.setOrigin({sunRadius, sunRadius});
    
    update();

}

void Graphics::update() {

    sun.setPosition({(float) (W/2 + s.x[0] * mToPx), (float) (H/2 - s.y[0] * mToPx)});

    #pragma omp parallel for if (OMP_OK)
    for (int i = 0; i < N-1; i++) {

        bodies[i].position = {(float) (W/2 + s.x[i+1] * mToPx), (float) (H / 2 - s.y[i+1] * mToPx)};

    } 

}

void Graphics::draw(sf::RenderWindow& target)  {

    update();
    target.draw(bodies);
    target.draw(sun);

}

State stateInit() {

    State s;

    s.m.setConstant(1);
    s.m[0] = M;

    s.x[0] = 0; s.y[0] = 0;
    s.vx[0] = 0; s.vy[0] = 0;

    std::mt19937 gen(static_cast<unsigned int>(std::time(nullptr))); 
    std::uniform_real_distribution<double> dist(minR, maxR);
    std::uniform_real_distribution<double> ang(0.0, 2.0 * PI);

    for (int i = 1; i < N; i++) {

        double r = dist(gen);
        double theta = ang(gen);

        s.x[i] = r * std::cos(theta);
        s.y[i] = r * std::sin(theta);

        double v = std::sqrt(G * M / r);
        s.vx[i] =  -v * std::sin(theta);
        s.vy[i] = v * std::cos(theta);

    }

    s.verletBackwardsStep();

    return s;

}