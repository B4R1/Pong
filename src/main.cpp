#include <SFML/Graphics.hpp>
#include <iostream>

enum class GameState {
    Playing,
    GameOver
};

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

    void resetRight() {
        shape.setPosition(400, 300);
        direction = {1.0f, 1.0};
    }

    void resetLeft() {
        shape.setPosition(400, 300);
        direction = {-1.0f, -1.0};
    }
};

class Game {
public:
    GameState state;
    Paddle paddle1, paddle2;
    Ball ball;
    int scoreLeft, scoreRight;

    Game() : state(GameState::Playing), paddle1(20, 250), paddle2(760, 250), ball(400, 300) {}

    void handleInput(sf::RenderWindow& window) {
        // Paddle movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && paddle1.shape.getPosition().y > 0)
            paddle1.shape.move(0, -paddle1.speed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && paddle1.shape.getPosition().y + paddle1.shape.getSize().y < 600)
            paddle1.shape.move(0, paddle1.speed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && paddle2.shape.getPosition().y > 0)
            paddle2.shape.move(0, -paddle2.speed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && paddle2.shape.getPosition().y + paddle2.shape.getSize().y < 600)
            paddle2.shape.move(0, paddle2.speed);
    }

    void restart() {
        state = GameState::Playing;
        scoreLeft = 0;
        scoreRight = 0;
        ball.resetLeft();
    }

    void update() {
        if (state == GameState::Playing) {

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
                scoreRight++;
                ball.resetRight()   ;
            } else if (ball.shape.getPosition().x + ball.shape.getRadius() * 2 >= 800) {
                // Ball out of bounds on the right
                // Handle right side logic here
                scoreLeft++;
                ball.resetLeft();
            }

            if(scoreRight == 10 || scoreLeft == 10) {
                state = GameState::GameOver;
            }

        } else if (state == GameState::GameOver) {
            // Check for restart input (e.g., space key)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                restart();
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        window.clear();

        // Draw paddles and ball
        window.draw(paddle1.shape);
        window.draw(paddle2.shape);
        window.draw(ball.shape);

        // Draw score board
        sf::Font font;
        if (font.loadFromFile("OpenSans.ttf")) { // Change the font file path as needed
            sf::Text scoreText("Score: " + std::to_string(scoreLeft) + " - " + std::to_string(scoreRight), font, 24);
            scoreText.setPosition(320, 10);
            window.draw(scoreText);
        }

        // Draw game over screen
        if (state == GameState::GameOver) {
            window.clear();
            sf::Font font;
            if (font.loadFromFile("OpenSans.ttf")) { // Change the font file path as needed
                sf::Text gameOverText("Game Over!", font, 50);
                gameOverText.setPosition(250, 250);
                window.draw(gameOverText);
            }
            if (font.loadFromFile("OpenSans.ttf")) { // Change the font file path as needed
                if(scoreLeft == 10) {
                    sf::Text winnerText("Left Win!", font, 50);
                    winnerText.setPosition(300, 300);
                    window.draw(winnerText);
                }
                if (scoreRight == 10) {
                    sf::Text winnerText2("Right Win!", font, 50);
                    winnerText2.setPosition(300, 300);
                    window.draw(winnerText2);
                }

            }

        }

        window.display();
    }

};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");
    window.setFramerateLimit(60);

    Game game;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        game.handleInput(window);
        game.update();
        game.draw(window);
    }

    return 0;

}
