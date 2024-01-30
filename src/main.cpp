#include <SFML/Graphics.hpp>
#include <iostream>

class Paddle {
public:
    sf::RectangleShape shape;
    float speed;

    Paddle(float x, float y) {
        shape.setSize(sf::Vector2f(20, 100));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::White);
        speed = 5.0f;
    }
};

class Ball {
public:
    sf::CircleShape shape;
    sf::Vector2f direction;
    float speed;

    Ball(float x, float y) {
        shape.setRadius(10);
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::White);
        direction = {1.0f, 1.0f};
        speed = 5.0f;
    }

    void reset() {
        shape.setPosition(400, 300);
        direction = {1.0f, 1.0f};
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");
    window.setFramerateLimit(60);

    Paddle paddle1(20, 250);
    Paddle paddle2(760, 250);
    Ball ball(400, 300);

    int score = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        // Paddle movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && paddle1.shape.getPosition().y > 0)
            paddle1.shape.move(0, -paddle1.speed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && paddle1.shape.getPosition().y + paddle1.shape.getSize().y < 600)
            paddle1.shape.move(0, paddle1.speed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && paddle2.shape.getPosition().y > 0)
            paddle2.shape.move(0, -paddle2.speed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && paddle2.shape.getPosition().y + paddle2.shape.getSize().y < 600)
            paddle2.shape.move(0, paddle2.speed);

        // Ball movement
        ball.shape.move(ball.direction * ball.speed);

        // Ball collision with walls
        if (ball.shape.getPosition().y <= 0 || ball.shape.getPosition().y + ball.shape.getRadius() * 2 >= 600)
            ball.direction.y = -ball.direction.y;

        // Ball collision with paddles
        if (ball.shape.getGlobalBounds().intersects(paddle1.shape.getGlobalBounds()) ||
            ball.shape.getGlobalBounds().intersects(paddle2.shape.getGlobalBounds()))
            ball.direction.x = -ball.direction.x;

        // Ball out of bounds
        if (ball.shape.getPosition().x <= 0) {
            // Ball out of bounds on the left
            // Handle left side logic here
            score++;
            ball.reset();
        } else if (ball.shape.getPosition().x + ball.shape.getRadius() * 2 >= 800) {
            // Ball out of bounds on the right
            // Handle right side logic here
            ball.reset();
        }

        sf::Font font;
        if (!font.loadFromFile("OpenSans.ttf"))
        {
            // error...
        }

        sf::Text text;
        text.setFont(font);

        text.setString("Score: " + std::to_string(score));
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);


        window.clear();

        // Draw paddles and ball
        window.draw(paddle1.shape);
        window.draw(paddle2.shape);
        window.draw(ball.shape);
        window.draw(text);

        window.display();
    }

    return 0;

}
