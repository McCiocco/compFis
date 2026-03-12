#include "../state.h"
#include <SFML/Graphics.hpp>

constexpr double sunRadius = 10;
constexpr double M = 5000000;
constexpr double minR = 100, maxR = 500;

constexpr unsigned trailLen = 1000;

constexpr double cm = 255 / 150;

const sf::Color stuff = sf::Color::White;

inline sf::Color getColor(const double);

inline double getV(const State&, const unsigned);

struct Graphics {

    const State& s;
    sf::VertexArray bodies;
    sf::CircleShape sun;
    sf::VertexArray trail;
    unsigned counter;

    Graphics(const State&);

    void update();

    void draw(sf::RenderWindow&);

};

State stateInit();
