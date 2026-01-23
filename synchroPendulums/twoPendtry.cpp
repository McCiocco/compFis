// prova con due pendoli
// g++ -O3 -o anim synchroPendulum.cpp ../lib/numericalMethods.cpp ../lib/utils.h -lsfml-graphics -lsfml-window -lsfml-system
// to compile
#include "../lib/numericalMethods.h"
#include "../lib/utils.h"
#include <math.h>
#include <SFML/Graphics.hpp>
#include <random>

constexpr int W = 1200, H = 800;
constexpr int ANTIAL = 8;
constexpr bool FPS_LIMIT = true;
constexpr double FPS = 30;
const sf::Color background = sf::Color::Black;
const sf::Color stuff = sf::Color::Green;

constexpr double PX_TO_MM = 1.0/3;

constexpr double x0 = 0; //initial position of support
constexpr double xDot0 = 0; //initial velocity of support
constexpr double minTh = - PI / 3; // minimun generated angle value
constexpr double maxTh = PI / 3; // maximum generated angle value

constexpr double attrT = 0.05;
constexpr double attrS = 0.05;

constexpr double m = 1; //mass of pendulum bobbob
constexpr double M = 8; //mass of support
constexpr double l = 700; //length of pendulum rope (mm)
constexpr double l1 = 705;
constexpr double r = 60; //radius of the circle (mm)
constexpr double suppWidth = 1600; //width of the support (mm)

constexpr double Gx0 = W / 2 - suppWidth * PX_TO_MM / 2;
constexpr double Gy0 = H / 5;

constexpr double tau = 0.001; //time step

struct Graphics {

    const Vector<6>& s;
    sf::RectangleShape support;
    sf::RectangleShape ropes[2];
    sf::CircleShape bobs[2];

    Graphics(const Vector<6>& s_): 
                s(s_), support({PX_TO_MM * suppWidth, PX_TO_MM * 100}) {

        support.setFillColor(stuff);
        support.setOrigin({0, PX_TO_MM * 100});

        for (int i = 0; i < 2; i++) {

            bobs[i] = sf::CircleShape(r * PX_TO_MM);
            bobs[i].setOrigin(r * PX_TO_MM, r * PX_TO_MM);
            bobs[i].setFillColor(stuff);

            ropes[i].setSize({l * PX_TO_MM, 2});
            ropes[i].setOrigin({0, 1});
            ropes[i].setFillColor(stuff);


        }

    }
    
    void draw(sf::RenderWindow& target) {

        support.setPosition({Gx0 + s[0] * 1000 * PX_TO_MM, Gy0});

        double bobx = suppWidth / 4;
        for (int i = 1; i <= 2; i++) {

            bobs[i-1].setPosition(support.getPosition() + 
                    sf::Vector2f({PX_TO_MM * bobx * (2*i - 1), 0}) +
                    sf::Vector2f({(l + r) * PX_TO_MM * std::sin(s[i]), (l+r) * PX_TO_MM * std::cos(s[i])}));

        }

        for (int i = 1; i <= 2; i++) {

            ropes[i-1].setPosition(support.getPosition() + sf::Vector2f({bobx * PX_TO_MM * (2*i - 1), 0}));
            ropes[i-1].setRotation(90 - s[i] * 180 / PI);

        }

        target.draw(support);
        for (int i = 0; i < 2; i++) target.draw(ropes[i]);
        for (int i = 0; i < 2; i++) target.draw(bobs[i]);

    }


};

Vector<6> fattr(const Vector<6>& s, double t) {

    Vector<6> res;

    for (int i = 0; i < 3; i++) res[i] = s[i + 3];

    double cosT[3];
    for (int i = 1; i <= 2; i++) cosT[i] = std::cos(s[i]);
    double sinT[3];
    for (int i = 1; i <= 2; i++) sinT[i] = std::sin(s[i]);

    double A = cosT[1] * cosT[1] + cosT[2] + cosT[2];
    A *= -m;
    A += M + 2*m;

    double B = sinT[1] * (g * cosT[1] + s[4] * s[4] * l / 1000) +
               sinT[2] * (g * cosT[2] + s[5] * s[5] * l1 / 1000);
    B *= -m;
    B -= attrS * s[3];

    res[3] = B / A;

    res[4] = (res[3] * cosT[1] - g * sinT[1] - attrT * 1000 / m / l * s[4]) * 1000 / l;
    res[5] = (res[3] * cosT[2] - g * sinT[2] - attrT * 1000 / m / l1 * s[5]) * 1000 / l1;
    

    return res;

}

Vector<6> f(const Vector<6>& s, double t) {

    Vector<6> res;

    for (int i = 0; i < 3; i++) res[i] = s[i + 3];

    double cosT[3];
    for (int i = 1; i <= 2; i++) cosT[i] = std::cos(s[i]);
    double sinT[3];
    for (int i = 1; i <= 2; i++) sinT[i] = std::sin(s[i]);

    double A = cosT[1] * cosT[1] + cosT[2] + cosT[2];
    A *= -m;
    A += M + 2*m;

    double B = sinT[1] * (g * cosT[1] + s[4] * s[4]) +
               sinT[2] * (g * cosT[2] + s[5] * s[5]);
    B *= -m;

    res[3] = B / A;

    res[4] = (res[3] * cosT[1] - g * sinT[1]) * 1000 / l;
    res[5] = (res[3] * cosT[2] - g * sinT[2]) * 1000 / l;
    

    return res;

}



int main() {

    // setup window 

    sf::ContextSettings settings;
    settings.antialiasingLevel = ANTIAL;
    sf::RenderWindow window(sf::VideoMode(W, H), "Two Pendulum Syncronization", sf::Style::Default, settings);
    if (FPS_LIMIT) window.setFramerateLimit(FPS);
    window.setKeyRepeatEnabled(false);

    // initialize variables

    Vector<6> s = {x0, -2*PI/3, PI/6, 0, 0, 0};

    Graphics gs(s);

    double t = 0;
    unsigned long long frameCount = 0;

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
            
                for (int i = 0; i < 1.0/tau/FPS; i++) {

                    s = rk4(s, t, tau, fattr);
                    t += tau;

                }

                frameCount += 1.0/tau/FPS;
                
            } else {

                for (int i = 0; i < 10.0/tau/FPS; i++) {

                    s = rk4(s, t, tau, fattr);
                    t += tau;

                }
                    frameCount += 10.0/tau/FPS;


            }


        }

        window.clear(background);

        //std::cout << "y" << std::endl;
        gs.draw(window);
        //if (frameCount > 500) frameCount = 0, std::cout << (s[1] - s[2]) << " " << s[1] << " " << s[2]  <<std::endl; 

        window.display();
    

    }
}