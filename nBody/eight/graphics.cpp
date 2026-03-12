#include "graphics.h"

Graphics::Graphics(const State& s_): 
    s(s_), bodies(N), trail(sf::Points, trailLen * N), last_updated(0) {

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
        trail[last_updated].color = stuff;
        trail[last_updated++].position = bodies[i].getPosition();            

        last_updated = last_updated % (trailLen * N);

    }

}

void Graphics::draw(sf::RenderWindow& target) {

    update();
    for (int i = 0; i < N; i++) {
        
        target.draw(bodies[i]);

    }

    target.draw(trail);

}

State stateInit() {

    State s;

    const double x1 = 0.97000436;
    const double y1 = -0.24308753;
    const double vx1 = 0.46620368;
    const double vy1 = 0.43236573;

    s.x[0] = x1; s.y[0] = y1;
    s.vx[0] = vx1; s.vy[0] = vy1;

    s.x[1] = -x1; s.y[1] = -y1;
    s.vx[1] = vx1; s.vy[1] = vy1;

    s.x[2] = 0; s.y[2] = 0;
    s.vx[2] = -2*vx1; s.vy[2] = -2*vy1;

    s.m[0] = 1; s.m[1] = 1; s.m[2] = 1;

    s.verletBackwardsStep();

    return s;

}