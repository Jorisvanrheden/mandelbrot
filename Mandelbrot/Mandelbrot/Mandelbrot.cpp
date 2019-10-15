#include <SFML/Graphics.hpp>

#include <iostream>

double Map(double realValue, double realMin, double realMax, double mappedMin, double mappedMax)
{
	double realPercentage = (realValue - realMin) / (realMax - realMin);

	double additionToMapped = realPercentage * (mappedMax - mappedMin);
	return mappedMin + additionToMapped;
}

sf::Image DrawImage(sf::Image image, int width, int height, double relMinX, double relMaxX, double relMinY, double relMaxY)
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			const int maxIterations = 100;

			//Map pixel to relative range
			double a = Map(i, 0, width, relMinX, relMaxX);
			double b = Map(j, 0, height, relMinY, relMaxY);

			double originalA = a;
			double originalB = b;

			int n = 0;
			while (n < maxIterations)
			{
				double aa = a*a - b*b;
				double bb = 2 * a*b;

				a = aa + originalA;
				b = bb + originalB;

				if (a + b>16)
				{
					break;
				}

				n++;
			}

			int bright = Map(n, 0, maxIterations, 255, 0);

			sf::Color color = sf::Color(bright, bright, bright, 255);
			image.setPixel(i, j, color);
		}
	}
	return image;
}

int main()
{
	int screenWidth = 400;
	int screenHeight = 400;

	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Mandelbrot");

	sf::Image image;
	image.create(screenWidth, screenHeight);

	sf::Texture texture;
	texture.create(screenWidth, screenHeight);
	texture.update(image);

	sf::Sprite sprite = sf::Sprite(texture);
	sprite.setPosition(0, 0);

	double relativeMinX = -1;
	double relativeMaxX = 1;
	double relativeMinY = -1;
	double relativeMaxY = 1;
	
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

					relativeMinY -= iter;
					relativeMaxY -= iter;
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
				{
					double diff = relativeMaxY - relativeMinY;
					double iter = diff*0.01f;

					relativeMinY += iter;
					relativeMaxY += iter;
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

			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		image = DrawImage(image, screenWidth, screenHeight, relativeMinX, relativeMaxX, relativeMinY, relativeMaxY);
		texture.update(image);


		window.draw(sprite);

		window.display();
	}

	return 0;


}




