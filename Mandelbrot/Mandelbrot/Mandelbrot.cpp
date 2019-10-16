#include <SFML/Graphics.hpp>

#include <iostream>
#include <stdio.h>

float map(float value, float realMin, float realMax, float mappedMin, float mappedMax)
{
	float realPercentage = (value - realMin) / (realMax - realMin);

	float additionToMapped = realPercentage * (mappedMax - mappedMin);
	return mappedMin + additionToMapped;
}

int main()
{
	int screenWidth = 900;
	int screenHeight = 900;

	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Mandelbrot");

	sf::Shader shader;

	// Load the shader
	if (!shader.loadFromFile("..\\Shaders\\fragment.frag", sf::Shader::Fragment))
	{
		std::cout << "NOT FOUND" << std::endl;
	}

	sf::Image image;
	image.create(screenWidth, screenHeight);

	sf::Texture texture;
	texture.create(screenWidth, screenHeight);
	texture.update(image);

	sf::Sprite sprite = sf::Sprite(texture);
	sprite.setPosition(0, 0);

	double relativeMinX = -2;
	double relativeMaxX = 2;
	double relativeMinY = -2;
	double relativeMaxY = 2;
	sf::Vector2i mousePos(0, 0);

	bool locked = false;
	bool drag = false;
	sf::Vector2i dragOffset(0, 0);
	
	texture.update(image);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
				{
					double diff = relativeMaxX - relativeMinX;
					double iter = diff*0.01f;

					relativeMinX -= iter;
					relativeMaxX -= iter;
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
				{
					double diff = relativeMaxX - relativeMinX;
					double iter = diff*0.01f;

					relativeMinX += iter;
					relativeMaxX += iter;
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
				{
					double diff = relativeMaxY - relativeMinY;
					double iter = diff*0.01f;

					relativeMinY += iter;
					relativeMaxY += iter;
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
				{
					double diff = relativeMaxY - relativeMinY;
					double iter = diff*0.01f;

					relativeMinY -= iter;
					relativeMaxY -= iter;
				}					
			}

			if (event.type == sf::Event::MouseWheelScrolled)
			{			
				double diff = relativeMaxX - relativeMinX;
				double middle = (relativeMaxX + relativeMinX)/2;

				double zoom = 0.95f;
				if (event.mouseWheelScroll.delta < 0) zoom = 1.05f;

				diff *= zoom;

				relativeMaxX = middle + diff / 2;
				relativeMinX = middle - diff / 2;

				double diffY = relativeMaxY - relativeMinY;
				double middleY = (relativeMaxY + relativeMinY) / 2;

				diffY *= zoom;

				relativeMaxY = middleY + diffY / 2;
				relativeMinY = middleY - diffY / 2;
			}

			if (event.type == sf::Event::MouseButtonReleased) 
			{
				drag = false;
			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					drag = true;
				}
			}

			

			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				locked = !locked;
			}

			if (event.type == sf::Event::MouseMoved) 
			{
				sf::Vector2i newMousePos = sf::Mouse::getPosition(window);
				if (!locked)
				{
					dragOffset = mousePos - newMousePos;

					mousePos = newMousePos;
				}	

				if (drag)
				{
					double percentage_x = dragOffset.x / (double)screenWidth;
					double percentage_y = dragOffset.y / (double)screenHeight;
					double diffX = (relativeMaxX - relativeMinX) * percentage_x;
					double diffY = (relativeMinY - relativeMaxY) * percentage_y;

					std::cout << diffX << std::endl;

					relativeMinX += diffX;
					relativeMaxX += diffX;
					
					relativeMinY += diffY;
					relativeMaxY += diffY;

					dragOffset = sf::Vector2i(0, 0);
				}
			}

			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		float cx = map(mousePos.x, 0, screenWidth, -1, 1);
		float cy = map(mousePos.y, 0, screenHeight, -1, 1);

		//Pass the updated texture to the shader
		shader.setUniform("relativeX", sf::Glsl::Vec2(relativeMinX, relativeMaxX));
		shader.setUniform("relativeY", sf::Glsl::Vec2(relativeMinY, relativeMaxY));
		shader.setUniform("relativeC", sf::Glsl::Vec2(cx, cy));
		shader.setUniform("screenSize", sf::Glsl::Vec2(screenWidth, screenHeight));


		sf::RenderStates states;
		states.shader = &shader;
		
		window.draw(sprite, states);

		window.display();
	}

	return 0;


}




