#include "../state.h"
#include <SFML/Graphics.hpp>

constexpr double sunRadius = 10;
constexpr double M = 5000000;
constexpr double minR = 100, maxR = 500;

const sf::Color stuff = sf::Color::White;

struct Graphics {

    const State& s;
    sf::VertexArray bodies;
    sf::CircleShape sun;

    Graphics(const State&);

    void update();

    void draw(sf::RenderWindow&);

};

State stateInit();
