//script to compute the frames for a single pendulum animation

#include <array>
#include "../lib/numericalMethods.h"
#include <math.h>
#include <SFML/Graphics.hpp>
#include <iostream>

constexpr double PI = 3.141592654;

// PARAMETERS

constexpr double g = 9.81;
constexpr double L = 3;
constexpr double initialAngle = 110;
constexpr double initialAngularVelocity = 0;
constexpr double fps = 60;

constexpr double gtoL = g / L;

//acceleration function

double a(const double th) {

    return -gtoL * std::sin(th);

}

int main() {

    // set initial parameters

    constexpr double theta1 = initialAngle * PI / 180, 
    omega1 = initialAngularVelocity;
    constexpr double tau = 1/fps;

    // set sfml window

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    const int W = 1200, H = 800;
    sf::RenderWindow window(sf::VideoMode(W, H), "Simple Pendulum", sf::Style::Default, settings);
    window.setFramerateLimit(fps);
    window.setKeyRepeatEnabled(false);

    //verlet backwards step

    const double theta0 = theta1 - tau * omega1 + .5 * tau * tau * a(theta1);

    // set animation variables
    double theta = theta1;
    double prevTheta = theta0;

    //set sfml shapes

    const float r = 25;
    sf::CircleShape bob(r);
    bob.setOrigin(r, r);
    bob.setFillColor(sf::Color::White);
    bob.setOutlineColor(sf::Color::White);
    bob.setOutlineThickness(3);


    // const float ropeThickness = 1.5;
    // sf::RectangleShape  rope(sf::Vector2f(L * 100, ropeThickness));
    // rope.setFillColor(sf::Color::Black);
    // rope.setOrigin(sf::Vector2f(0, ropeThickness / 2));
    // rope.setPosition(sf::Vector2f(W / 2, H / 2));
    
    sf::Vertex rope[] = {

        sf::Vertex(sf::Vector2f(W/2, H/2)),
        sf::Vertex(sf::Vector2f(0, 0))

    };

    rope[0].color = sf::Color::White;
    rope[1].color = sf::Color::White;

    //main loop

    bool go = true;
    sf::Time start;
    sf::Clock clock;
    double timePassed = 0;

    while (window.isOpen()) {

        start = clock.getElapsedTime();

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
                
                }
                
                break;

            default:
                break;
            }
            
        }

        // logic

        if (go) {

            double temp = theta;
            theta = verletTraj(theta, prevTheta, a, tau);

            //timePassed += tau;
            //if (prevTheta > temp && temp < theta) std::cout << "half a period: " << timePassed << std::endl; 

            prevTheta = temp;
        
        }
        
        // drawing

        double xbob = W / 2 + 300 * std::sin(theta);
        double ybob = H / 2 + 300 * std::cos(theta); //fixed animation lenght


        bob.setPosition(xbob, ybob);
        //rope.setRotation(90 - theta * 180 / PI);
        rope[1].position = sf::Vector2f(xbob, ybob);

        window.clear(sf::Color::Black);
        //window.draw(rope);
        window.draw(rope, 2, sf::Lines);
        window.draw(bob);

        window.display();

        //while (clock.getElapsedTime() - start < sf::seconds(tau)) ;

    }





    

}
