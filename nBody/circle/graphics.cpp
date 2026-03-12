#include "graphics.h"

Graphics::Graphics(const State& s_): 
    s(s_), bodies(N) {

    for (int i = 0; i < N; i++) {

        bodies[i].setRadius(r);
        bodies[i].setFillColor(stuff);
        bodies[i].setOrigin({r, r});

    }

    update();

}

void Graphics::update() {

    for (int i = 0; i < N; i++) {

        bodies[i].setPosition({(float) (W/2 + s.x[i] * mToPx), (float) (H / 2 - s.y[i] * mToPx)});

    }

} 

void Graphics::draw(sf::RenderWindow& target) {

    update();
    for (int i = 0; i < N; i++) {
        
        target.draw(bodies[i]);

    }

}

State stateInit() {

    State s;

    double sum = 0;

    for (int j = 1; j < N; j++) {

        sum += 1 / std::sin(PI * j / N);

    }

    double v = std::sqrt(G * mass * sum / (4 * r));

    for (int i = 0; i < N; i++) {

        double theta = 2*PI / N * i;

        s.x[i] = r * std::cos(theta);
        s.y[i] = r * std::sin(theta);

        s.m[i] = mass;

        s.vx[i] = std::cos(PI / 2 + theta) * v;
        s.vy[i] = std::sin(PI / 2 + theta) * v;

    }
    
    s.verletBackwardsStep();

    return s;

}
