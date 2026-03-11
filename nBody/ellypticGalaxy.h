#include "approxState.h"
#include <random>
#include <omp.h>
#include <ctime>

constexpr unsigned N = 1000;
constexpr double soft = 1;
constexpr double G = 1;
constexpr double FPS = 60;
constexpr double updatesPerFrame = 25;
constexpr double tau = 1 / (FPS * updatesPerFrame);
constexpr double mToPx = 1;

constexpr double sunRadius = 10;
constexpr double M = 5000000;
constexpr double minR = 100, maxR = 500;

constexpr unsigned trailLen = 1000;

constexpr double cm = 255 / 150;

const sf::Color stuff = sf::Color::White;

typedef State<N, G, tau, soft> state8;

inline sf::Color getColor(const double v) {

    double r = std::min(std::max(cm * (v - 75), 0.0), 255.0);
    double b = std::min(std::max(-cm * (v - 200), 0.0), 255.0);
    return sf::Color(r, 100, b);

}

inline double getV(const state8& s, const unsigned i) {

    double dx = (s.x[i] - s.prevx[i]);
    double dy = (s.y[i] - s.prevy[i]);
    return std::sqrt(dx*dx + dy*dy) / tau;

}

struct Graphics {

    const state8& s;
    sf::VertexArray bodies;
    sf::CircleShape sun;
    sf::VertexArray trail;
    unsigned counter;

    Graphics(const state8& s_): s(s_), bodies(sf::Points, N-1), trail(sf::LinesStrip, trailLen), counter(0) {

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
            bodies[i].color = getColor(getV(s, i+1));

        } 

        // trail[counter].position = {(float) (W/2 + s.x[1] * mToPx), (float) (H/2 - s.y[1] * mToPx)};
        // trail[counter].color = getColor(getV(s, 1));
        // counter = ++counter % trailLen; 

    }

    void draw(sf::RenderWindow& target) {

        update();
        target.draw(bodies);
        target.draw(sun);
        // target.draw(&trail[0], counter, sf::LineStrip);

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
    std::uniform_real_distribution<double> dmp(0.0, 1.3);


    for (int i = 1; i < N; i++) {

        double r = dist(gen);
        double theta = ang(gen);
        double damp = dmp(gen);

        s.x[i] = r * std::cos(theta);
        s.y[i] = r * std::sin(theta);

        double v = damp * std::sqrt(G * M / r);
        s.vx[i] =  -v * std::sin(theta);
        s.vy[i] = v * std::cos(theta);

    }

    s.verletBackwardsStep();

    return s;

}
