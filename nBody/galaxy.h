#include "state.h"
#include <omp.h>
#include <random>
#include <ctime>

constexpr unsigned N = 1000;
constexpr double soft = 1;
constexpr double G = 1;
constexpr double FPS = 60;
constexpr double updatesPerFrame = 1;
constexpr double tau = 1 / (FPS * updatesPerFrame);
constexpr double mToPx = 1;

constexpr double sunRadius = 10;
constexpr double M = 5000000;
constexpr double minR = 100, maxR = 500;

const sf::Color stuff = sf::Color::White;

typedef State<N, G, tau, soft> state8;

struct Graphics {

    const state8& s;
    sf::VertexArray bodies;
    sf::CircleShape sun;

    Graphics(const state8& s_): s(s_), bodies(sf::Points, N-1) {

        sun.setRadius(sunRadius);
        sun.setFillColor(stuff);
        sun.setOrigin({sunRadius, sunRadius});
        
        update();

    }

    void update() {

        sun.setPosition({(float) (W/2 + s.x[0] * mToPx), (float) (H/2 - s.y[0] * mToPx)});

        #pragma omp parallel for
        for (int i = 0; i < N-1; i++) {

            bodies[i].position = {(float) (W/2 + s.x[i+1] * mToPx), (float) (H / 2 - s.y[i+1] * mToPx)};

        } 

    }

    void draw(sf::RenderWindow& target) {

        update();
        target.draw(bodies);
        target.draw(sun);

    }

};



state8 stateInit() {

    state8 s;

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
