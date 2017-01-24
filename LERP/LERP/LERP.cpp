// LERP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <SFML\Graphics.hpp>

using namespace std;

sf::Vector2f res(sf::Vertex start, sf::Vertex finish, float  t)
{
	float x = (finish.position.x - start.position.x) * t + start.position.x;
	float y = (finish.position.y - start.position.y) * t + start.position.y;
	return sf::Vector2f(x, y);

}

int main()
{
	int s;
	int n;
	bool mouseClicked = false;
	bool dragging = false;
	int lastPoint = -1;

	cout << "S = ";
	cin >> s;
	cout << "N = ";
	cin >> n;

	//POINTS AND LINES
	sf::VertexArray controlPoints(sf::Points, n);
	sf::VertexArray steps(sf::LinesStrip, s + 1);
	vector<sf::VertexArray> curves(n/2);

	for (int i = 0; i < controlPoints.getVertexCount(); i++)
	{
		cout << "x for P" << i << ": ";
		cin >> controlPoints[i].position.x;
		cout << "y for P" << i << ": ";
		cin >> controlPoints[i].position.y;
	}

	//WINDOW
	sf::RenderWindow window(sf::VideoMode(1500, 1500), "LERP");
	while (window.isOpen())
	{
		//MAKING THE POINTS AND POSITION
		int j = 0;
		for (int c = 0; c < curves.size(); c++)
		{
			for (int i = 0; i < steps.getVertexCount(); i++)
			{
				sf::Vector2f r1 = res(controlPoints[j], controlPoints[j + 1], (float)i / (float)s);
				sf::Vector2f r2 = res(controlPoints[j + 1], controlPoints[j + 2], (float)i / (float)s);
				steps[i].position = res(r1, r2, (float)i / (float)s);
			}
			curves[c] = steps;
			j = j + 2;
		}


		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();


			//When you click on the mouse
			if (event.type == sf::Event::MouseButtonPressed  && event.mouseButton.button == sf::Mouse::Left)
			{
				mouseClicked = true;
			}
			
			//When you release it
			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
			{
				dragging = false;
				mouseClicked = false;
				cout << "done!" << endl;
				
				for (int i = 0; i < controlPoints.getVertexCount(); i++)
					cout << controlPoints[i].position.x << ", " << controlPoints[i].position.y << endl;
			}
		}

		//Check if mouse position is by the control point
		if (mouseClicked)
		{

			float mousex = sf::Mouse::getPosition(window).x;
			float mousey = sf::Mouse::getPosition(window).y;
			for (int i = 0; i < controlPoints.getVertexCount(); i++)
			{
				int clickRadius = 15;
				sf::Vector2f cp = controlPoints[i].position;

				if ((mousex > cp.x - clickRadius) &&
					(mousex < cp.x + clickRadius) &&
					(mousey > cp.y - clickRadius) &&
					(mousey < cp.y + clickRadius))
				{
					lastPoint = i;
					dragging = true;
				}
			}
		}

		//While mouse has not yet been released
		if (dragging)
		{
			cout << lastPoint;
			controlPoints[lastPoint].position.x = sf::Mouse::getPosition(window).x;
			controlPoints[lastPoint].position.y = sf::Mouse::getPosition(window).y;
		}

		//DRAWING THE POINTS
		window.clear();
		for (int c = 0; c < curves.size(); c++)
		{
			window.draw(curves[c]);
		}
		window.draw(controlPoints);
		window.display();
	}

    return 0;
}

