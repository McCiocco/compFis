// dimonstrative project to show the syncronization of pendulums on a shared moving support
// g++ -O3 -o anim synchroPendulum.cpp ../lib/numericalMethods.cpp ../lib/utils.h -lsfml-graphics -lsfml-window -lsfml-system
// to compile
#include "../lib/numericalMethods.h"
#include "../lib/utils.h"
#include <math.h>
#include <SFML/Graphics.hpp>
#include <random>

// set graphics parameters

constexpr int W = 1200, H = 800;
constexpr int ANTIAL = 8;
constexpr bool FPS_LIMIT = true;
constexpr double FPS = 30;
const sf::Color background = sf::Color::Black;
const sf::Color stuff = sf::Color::White;

// set Simulation Parameters 

// INITIAL CONDITIONS

constexpr double x0 = 0; //initial position of support
constexpr double xDot0 = 0; //initial velocity of support
constexpr double minTh = - PI / 3; // minimun generated angle value
constexpr double maxTh = PI / 3; // maximum generated angle value

constexpr unsigned int N = 6; //number of pendulums
constexpr double m = 1; //mass of pendulum bobbob
constexpr double M = 10; //mass of support
constexpr double l = 3; //length of pendulum rope

constexpr double tau = 1 / FPS; //time step


struct System {

    Vector<2 * N + 2> state;

    System(double x, double xDot, double thetas[N], double thetasDot[N]) {

        state[0] = x;
        for (int i = 1; i <= N; i++) state[i] = thetas[i];
        state[N+1] = xDot;
        for (int i = N+2; i < 2*N + 2; i++) state[i] = thetasDot[i];    

    }

    System(double x0, double xDot0, double minTh, double maxTh) {

        std::random_device rd;
        std::mt19937 e2(rd());
        std::uniform_real_distribution<> th(minTh, maxTh);

        state[0] = x0;
        state[N + 1] = xDot0;
         for (int i = 1; i <= N; i++) state[i] = th(e2), state[N + 1 + i] = 0;
        //for (int i = 1; i <= N; i++) state[i] = PI / 3, state[N + 1 + i] = 0;

    }

    inline double x() const {return state[0];}

    inline double xDot() const {return state[N+1];}

    inline double theta(const unsigned i) const {return state[i];}

    inline double thetaDot(const unsigned i) const {return state[N + 1 + i];}

    System& operator=(const Vector<2*N + 2>& v) {

        state = v;
        return *this;

    }


};

struct GraphicSys {

    const System& sys;
    sf::RectangleShape support;
    sf::RectangleShape bobSupps[N];
    sf::RectangleShape ropes[N];
    sf::CircleShape bobs[N];

    GraphicSys(const System& s): sys(s), support({W / 2, H / 40}) {

        support.setOrigin({W / 4, H / 80});
        support.setFillColor(stuff);

        for (int i = 0; i < N; i++) {

            bobs[i] = sf::CircleShape(20);
            bobs[i].setOrigin(20, 20);
            bobs[i].setFillColor(stuff);

            bobSupps[i].setSize({2, 250});
            bobSupps[i].setOrigin({1, 0});
            bobSupps[i].setFillColor(stuff);
            
            ropes[i].setSize({200, 2});
            ropes[i].setOrigin({1, 0});
            ropes[i].setFillColor(stuff);
        
        }

    }

    sf::Vector2f bobSupp(const int i) {

        return sf::Vector2f({- W / 4 + (i) * (int) (W / 2 / (N+1)), - 250});

    }

    void draw(sf::RenderWindow& target) {

        support.setPosition(W / 2 + 30 * sys.x(), H / 2);

        for (int i = 1; i <= N; i++) {

            bobs[i - 1].setPosition(support.getPosition() + bobSupp(i) + 
            sf::Vector2f({200 * std::sin(sys.theta(i)), 200 * std::cos(sys.theta(i))}));

        }

        for (int i = 1; i <= N; i++) {

            bobSupps[i-1].setPosition(support.getPosition() + bobSupp(i));
            ropes[i-1].setPosition(support.getPosition() + bobSupp(i));
            ropes[i-1].setRotation(90 - sys.theta(i) * 180 / PI);

        }


        target.draw(support);
        for (int i = 0; i < N; i++) target.draw(ropes[i]);
        for (int i = 0; i < N; i++) target.draw(bobSupps[i]);
        for (int i = 0; i < N; i++) target.draw(bobs[i]);

    }

};

Vector<2 * N + 2> f(const Vector<2 * N + 2>& state, double t) {

    Vector<2*N + 2> ret;
    for (int i = 0; i <= N; i++) ret[i] = state[N + 1 + i];

    double A = 0;
    double cosT[N+1];
    for (int i = 1; i <= N; i++) cosT[i] = std::cos(state[i]);
    double sinT[N+1];
    for (int i = 1; i <= N; i++) sinT[i] = std::sin(state[i]);
    for (int i = 1; i <= N; i++) A -= cosT[i] * cosT[i];
    A *= m;
    A += M + m*N;

    double B = 0;
    // qui e' += o -= ???
    for (int i = 1; i <= N; i++) B -= (g * cosT[i] + l * state[N + 1 + i] * state[N + 1 + i]) * sinT[i];
    B *= m;

    ret[N+1] = B / A;

    for (int i = 1; i <= N; i++) ret[N + 1 + i] = - (ret[N+1] * cosT[i] + g * sinT[i]) / l;

    return ret;

}

int main() {

    // setup window 

    sf::ContextSettings settings;
    settings.antialiasingLevel = ANTIAL;
    sf::RenderWindow window(sf::VideoMode(W, H), "Pendulum Syncronization", sf::Style::Default, settings);
    if (FPS_LIMIT) window.setFramerateLimit(FPS);
    window.setKeyRepeatEnabled(false);

    // initialize variables

    System sys(x0, xDot0, minTh, maxTh);
    GraphicSys gs(sys);

    double t = 0;

    bool go = true;
    bool fast = false;

    // main loop

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                
                window.close();
                break;
            
            case sf::Event::KeyPressed:

                if (event.key.scancode == sf::Keyboard::Scan::Escape) {

                    window.close();

                } else if (event.key.scancode == sf::Keyboard::Scan::P) { 
                    
                    go = !go;
                
                } else if (event.key.scancode == sf::Keyboard::Scan::L) {

                    std::cout << sys.state << std::endl;

                } else if (event.key.scancode == sf::Keyboard::Scan::F) {
                    
                    fast = !fast;

                }
                
                break;

            default:
                break;
            }
            
        }

        // calculations

        if (go) {

            if (!fast) {
            
                sys = rk4(sys.state, t, tau, f); 
                t += tau; //useless
                
            } else {

                for (int i = 0; i < 10; i++) {

                    sys = rk4(sys.state, t, tau, f);
                    t += tau;

                }

            }


        }


        window.clear(background);

        //std::cout << "y" << std::endl;
        gs.draw(window);

        window.display();
    

    }

}