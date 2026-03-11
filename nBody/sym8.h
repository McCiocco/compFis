#include "state.h"

constexpr unsigned N = 3;
constexpr double soft = 0;
constexpr double G = 1;
constexpr double FPS = 60;
constexpr double updatesPerFrame = 100;
constexpr double tau = 1 / (FPS * updatesPerFrame);
constexpr double mToPx = 300;

const sf::Color stuff = sf::Color::White;

typedef State<N, G, tau, soft> state8;

//length of a trail for a single body
constexpr int trailLen = 2000;
constexpr double r = 3;

struct Graphics {

    const state8& s;
    std::vector<sf::CircleShape> bodies;
    sf::VertexArray trail;
    unsigned last_updated;

    Graphics(const state8& s_): 
        s(s_), bodies(N), trail(sf::Points, trailLen * N), last_updated(0) {

        for (int i = 0; i < N; i++) {

            bodies[i].setRadius(r);
            bodies[i].setFillColor(stuff);
            bodies[i].setOrigin({r, r});

        }

        update();

    }

    void update() {

        for (int i = 0; i < N; i++) {

            bodies[i].setPosition({(float) (W/2 + s.x[i] * mToPx), (float) (H / 2 - s.y[i] * mToPx)});
            trail[last_updated].color = stuff;
            trail[last_updated++].position = bodies[i].getPosition();            

            last_updated = last_updated % (trailLen * N);

        }

    } 

    void draw(sf::RenderWindow& target) {

        update();
        for (int i = 0; i < N; i++) {
            
            target.draw(bodies[i]);

        }

        target.draw(trail);

    }

};

state8 stateInit() {

    state8 s;

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