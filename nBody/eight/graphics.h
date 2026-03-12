#include "../state.h"
#include <SFML/Graphics.hpp>

//length of a trail for a single body
constexpr int trailLen = 100;
constexpr double r = 3;

const sf::Color stuff = sf::Color::White;

struct Graphics {

    const State& s;
    std::vector<sf::CircleShape> bodies;
    sf::VertexArray trail;
    unsigned last_updated;

    Graphics(const State&);

    void update();

    void draw(sf::RenderWindow&);

};

State stateInit();
