// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <iostream>
#include "ComplexPlane.h"

// Make code easier to type with "using namespace"
using namespace sf;
using namespace std;


int main()
{
	// Create a video mode object
	int pixelWidth = VideoMode::getDesktopMode().width / 2;
	int pixelHeight = VideoMode::getDesktopMode().height / 2;
	VideoMode vm(pixelWidth, pixelHeight);
	// Create and open a window for the game
	RenderWindow window(vm, "Screen", Style::Default);


	// Draw some text
	Text messageText;
	// We need to choose a font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
	// Set the font to our message
	messageText.setFont(font);
	// Assign the actual message
	//messageText.setString("Press Enter to start!");
	// Make it really big
	messageText.setCharacterSize(15);
	// Choose a color
	messageText.setFillColor(Color::White);
	// position text
	messageText.setPosition(10, 10);

	ComplexPlane plane(pixelWidth, pixelHeight);



	while (window.isOpen())
	{

		/*
		****************************************
		Handle the players input
		****************************************
		*/

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					std::cout << "the left button was pressed" << std::endl;
					std::cout << "mouse x: " << event.mouseButton.x << std::endl;
					std::cout << "mouse y: " << event.mouseButton.y << std::endl;

					// zoom out feature
					plane.zoomIn();
					plane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));
				}
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					std::cout << "the right button was pressed" << std::endl;
					std::cout << "mouse x: " << event.mouseButton.x << std::endl;
					std::cout << "mouse y: " << event.mouseButton.y << std::endl;

					// zoom out feature
					plane.zoomOut();
					plane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));
				}

			}
		//check if mouse moved
		if (event.type == sf::Event::MouseMoved)
		{

			//std::cout << "new mouse x: " << event.mouseMove.x << std::endl;
			//std::cout << "new mouse y: " << event.mouseMove.y << std::endl;

			plane.setMouseLocation(Vector2i(event.mouseMove.x, event.mouseMove.y));
		}
		

		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		/*
		****************************************
		Update the scene
		****************************************
		*/

		plane.updateRender();
		plane.loadText(messageText);


		/*
		****************************************
		Draw the scene
		****************************************
		*/

		window.clear();
		window.draw(plane);
		// window.draw( load the text from plane? );
		window.draw(messageText);
		window.display();
	}
}
