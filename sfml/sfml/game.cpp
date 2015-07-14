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

	//create projectiles
	sf::RectangleShape projectileLeft(sf::Vector2f(paddleWidth, paddleHeight / 2));
	projectileLeft.setPosition(-200, -200);
	projectileLeft.setFillColor(sf::Color(250, 0, 0));
	sf::RectangleShape projectileRight(sf::Vector2f(paddleWidth, paddleHeight / 2));
	projectileRight.setPosition(-200, -200);
	projectileRight.setFillColor(sf::Color(250, 0, 0));

	//create ball and set its speed
	sf::RectangleShape ball(sf::Vector2f(ballSize, ballSize));
	ball.setPosition(windowWidth / 2, windowHeight / 2);
	float xSpeed = initialSpeed * 0.5;
	float ySpeed = initialSpeed * 0.5;
	float maxSpeed = initialSpeed;
	float angle;

	//initialize scores and scoreboard
	sf::Font scoreFont;
	scoreFont.loadFromFile("arial.ttf");
	int leftScore = 0;
	int rightScore = 0;
	sf::Text scoreLeft;
	sf::Text scoreRight;
	scoreLeft.setFont(scoreFont);
	scoreLeft.setString("0");
	scoreLeft.setCharacterSize(24);
	scoreLeft.setColor(sf::Color::White);
	scoreLeft.setPosition(windowWidth / 4, 0);
	scoreRight.setFont(scoreFont);
	scoreRight.setString("0");
	scoreRight.setCharacterSize(24);
	scoreRight.setColor(sf::Color::White);
	scoreRight.setPosition(windowWidth * 3 / 4, 0);


	//get bounding boxes of entities
	sf::FloatRect leftBox = paddleLeft.getGlobalBounds();
	sf::FloatRect rightBox = paddleRight.getGlobalBounds();
	sf::FloatRect ballBox = ball.getGlobalBounds();
	sf::FloatRect boxLeft = projectileLeft.getGlobalBounds();
	sf::FloatRect boxRight = projectileRight.getGlobalBounds();

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

		//fire and move projectiles
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
			projectileLeft.setPosition(paddleLeft.getPosition().x, paddleLeft.getPosition().y + (paddleHeight / 4));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)){
			projectileRight.setPosition(paddleRight.getPosition().x, paddleRight.getPosition().y + (paddleHeight / 4));
		}
		projectileLeft.move(0.5, 0);
		projectileRight.move(-0.5, 0);

		//move ball and reverse direction on impacts
		if (ball.getPosition().y <= 0 || ball.getPosition().y >= windowHeight - ballSize){
			ySpeed = -ySpeed;
			xSpeed = xSpeed;
		}

		//collision detection
		leftBox = paddleLeft.getGlobalBounds();
		rightBox = paddleRight.getGlobalBounds();
		ballBox = ball.getGlobalBounds();
		boxLeft = projectileLeft.getGlobalBounds();
		boxRight = projectileRight.getGlobalBounds();
		
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
		if (boxLeft.intersects(ballBox)){
			xSpeed = maxSpeed;
		}
		if (boxRight.intersects(ballBox)){
			xSpeed = -maxSpeed;
		}
		

		//if ball goes off a side, add score and reset ball
		if (ball.getPosition().x <= 0 - ballSize){
			rightScore += 1;
			switch (rightScore)
			{
			case 1: scoreRight.setString("1");
				break;
			case 2: scoreRight.setString("2");
				break;
			case 3: scoreRight.setString("3");
				break;
			case 4: scoreRight.setString("4");
				break;
			case 5:
				rightScore = 0;
				leftScore = 0;
				scoreRight.setString("0");
				scoreLeft.setString("0");
				break;
			default: scoreRight.setString("0");
				break;
			}
			ball.setPosition(windowWidth / 2, windowHeight / 2);
			xSpeed = initialSpeed * -0.5;
			ySpeed = initialSpeed * 0.5;
			maxSpeed = initialSpeed;
		}
		if (ball.getPosition().x >= windowWidth){
			leftScore += 1;
			switch (leftScore)
			{
			case 1: scoreLeft.setString("1");
				break;
			case 2: scoreLeft.setString("2");
				break;
			case 3: scoreLeft.setString("3");
				break;
			case 4: scoreLeft.setString("4");
				break;
			case 5:
				rightScore = 0;
				leftScore = 0;
				scoreRight.setString("0");
				scoreLeft.setString("0");
				break;
			default: scoreLeft.setString("0");
				break;
			}
			ball.setPosition(windowWidth / 2, windowHeight / 2);
			xSpeed = initialSpeed * 0.5;
			ySpeed = initialSpeed * 0.5;
			maxSpeed = initialSpeed;
		}
		ball.move(xSpeed, ySpeed);

		//draw all onscreen objects
		window.clear();
		window.draw(projectileLeft);
		window.draw(projectileRight);
		window.draw(paddleLeft);
		window.draw(paddleRight);
		window.draw(ball);
		window.draw(scoreLeft);
		window.draw(scoreRight);
		window.display();
	}

}
