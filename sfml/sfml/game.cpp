#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

int main(){

	//define constants
	const int paddleHeight = 100;
	const int paddleWidth = 20;
	const int windowHeight = 768;
	const int windowWidth = 1024;
	const int ballSize = 10;
	const float initialSpeed = 0.2;
	const float acceleration = 1.1;

	//load sound
	//sf::SoundBuffer buffer;
	//buffer.loadFromFile("beep");

	//initialize scoreboard
	sf::Font font;

	//create paddles
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "PONG");
	sf::RectangleShape paddleLeft(sf::Vector2f(paddleWidth, paddleHeight));
	paddleLeft.setPosition(paddleWidth + 5, windowHeight / 2);
	sf::RectangleShape paddleRight(sf::Vector2f(paddleWidth, paddleHeight));
	paddleRight.setPosition(windowWidth - (paddleWidth*2) - 5, windowHeight / 2);

	//create ball and set its speed
	sf::RectangleShape ball(sf::Vector2f(ballSize, ballSize));
	ball.setPosition(windowWidth / 2, windowHeight / 2);
	float xSpeed = initialSpeed * 0.5;
	float ySpeed = initialSpeed * 0.5;
	float maxSpeed = initialSpeed;
	float angle;

	//initialize scores and scoreboard
	int leftScore = 0;
	int rightScore = 0;

	//get bounding boxes of entities
	sf::FloatRect leftBox = paddleLeft.getGlobalBounds();
	sf::FloatRect rightBox = paddleRight.getGlobalBounds();
	sf::FloatRect ballBox = ball.getGlobalBounds();

	while (window.isOpen()){

		sf::Event event;
		while (window.pollEvent(event)){
			switch (event.type)
			{
				// window closed
			case sf::Event::Closed:
				window.close();
				break;

			default:
				break;
			}
		}

		//move paddles in response to keypresses
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && paddleRight.getPosition().y <= windowHeight - paddleHeight){
			paddleRight.move(0, 1);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && paddleRight.getPosition().y >= 0){
			paddleRight.move(0, -1);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && paddleLeft.getPosition().y <= windowHeight - paddleHeight){
			paddleLeft.move(0, 1);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && paddleLeft.getPosition().y >= 0){
			paddleLeft.move(0, -1);
		}

		//move ball and reverse direction on impacts
		if (ball.getPosition().y <= 0 || ball.getPosition().y >= windowHeight - ballSize){
			ySpeed = -ySpeed;
			xSpeed = xSpeed;
		}

		leftBox = paddleLeft.getGlobalBounds();
		rightBox = paddleRight.getGlobalBounds();
		ballBox = ball.getGlobalBounds();
		if (leftBox.intersects(ballBox)){
			ball.setPosition(paddleWidth * 2 + 5, ball.getPosition().y);
			maxSpeed = maxSpeed * acceleration;
			angle = (ball.getPosition().y + ballSize - paddleLeft.getPosition().y) / (paddleHeight + ballSize);
			ySpeed = maxSpeed * (angle - 0.5);
			xSpeed = maxSpeed - abs(ySpeed);
		}
		if (rightBox.intersects(ballBox)){
			ball.setPosition(windowWidth - (paddleWidth * 3), ball.getPosition().y);
			maxSpeed = maxSpeed * acceleration;
			angle = (ball.getPosition().y + ballSize - paddleRight.getPosition().y) / (paddleHeight + ballSize);
			ySpeed = maxSpeed * (angle - 0.5);
			xSpeed = (maxSpeed - abs(ySpeed)) * -1;
		}
		

		if (ball.getPosition().x <= 0 - ballSize){
			rightScore += 1;
			ball.setPosition(windowWidth / 2, windowHeight / 2);
			xSpeed = initialSpeed * -0.5;
			ySpeed = initialSpeed * 0.5;
			maxSpeed = initialSpeed;
		}
		if (ball.getPosition().x >= windowWidth){
			leftScore += 1;
			ball.setPosition(windowWidth / 2, windowHeight / 2);
			xSpeed = initialSpeed * 0.5;
			ySpeed = initialSpeed * 0.5;
			maxSpeed = initialSpeed;
		}
		ball.move(xSpeed, ySpeed);

		window.clear();
		window.draw(paddleLeft);
		window.draw(paddleRight);
		window.draw(ball);
		window.display();
	}

}