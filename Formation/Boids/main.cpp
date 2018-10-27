#include <iostream>
#include <vector>
#include "Boid.h"
#include "Pvector.h"
#include "Flock.h"
#include "TextureHolder.h"
#include "Player.h"
#include "KeyHandler.h"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include <string>
using namespace std;

/*
Brief description of Boid Class:
// This file acts as the main for our boids project. Here, we utilize the SFML
// library, import boids and flock classes, and run the program.-
*/

int main()
{
	
	KeyHandler m_keyHandler;
	TextureHolder m_textures;
	m_textures.load(Textures::Player, "assets/player.png");
	m_textures.load(Textures::Enemy, "assets/enemy.png");
	Player m_player(m_textures.get(Textures::Player), m_keyHandler);
	float boidsSize = 20;
	string action = "flock";

	//Gets the resolution, size, and bits per pixel for the screen of the PC that is running this program.
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	const int window_height = desktop.height;
	const int window_width = desktop.width;

	//Having the style of "None" gives a false-fullscreen effect for easier closing and access.
	//No FPS limit of V-sync setting needed for it may cause unnecessary slowdown.
	sf::RenderWindow window(sf::VideoMode(desktop.width, desktop.height, desktop.bitsPerPixel), "Boids", sf::Style::None); 
	
	//Create flock, vector of shapes, and initialize boids
	Flock flock;
	vector<sf::CircleShape> shapes;
	
	int fLeader = 0; //NPC in slot 0 is the leader when in formation mode.

	for (int i = 0; i < 10; i++) //Number of boids is hardcoded for testing pusposes.
	{
		//Boid b(rand() % window_width, rand() % window_height); //Starts the boid with a random position in the window.
		Boid b(window_width / 3, window_height / 3); //Starts all boids in the center of the screen
		sf::CircleShape shape(20); //Shape with a radius of 10 and 3 points (Making it a triangle)

		//Changing the Visual Properties of the shape
		//shape.setPosition(b.location.x, b.location.y); //Sets position of shape to random location that boid was set to.
		shape.setPosition(window_width, window_height); //Testing purposes, starts all shapes in the center of screen.
		//shape.setOutlineColor(sf::Color(0, 255, 0));
		//if (i == fLeader) // So we can see our leader when in formation
		//	shape.setFillColor(sf::Color::Blue);
		//else
		//	shape.setFillColor(sf::Color::Green);
		//shape.setOutlineColor(sf::Color::White);
		//shape.setOutlineThickness(1);
		shape.setRadius(boidsSize);
		shape.setTexture(&m_textures.get(Textures::Enemy));
		shape.setOrigin(shape.getGlobalBounds().width / 2, shape.getGlobalBounds().height / 2);

		//Adding the boid to the flock and adding the shapes to the vector<sf::CircleShape>
		flock.addBoid(b);
		shapes.push_back(shape);
	}

	while (window.isOpen())
	{	
		//Event used to close program when window is closed
		sf::Event event;
		while (window.pollEvent(event))
		{
			//"close requested" event: we close the window
			//Implemented alternate ways to close the window. (Pressing the escape, X, and BackSpace key also close the program.)
			if ((event.type == sf::Event::Closed) ||
				(event.type == sf::Event::KeyPressed &&
					event.key.code == sf::Keyboard::Escape) ||
					(event.type == sf::Event::KeyPressed &&
						event.key.code == sf::Keyboard::BackSpace) ||
						(event.type == sf::Event::KeyPressed &&
							event.key.code == sf::Keyboard::X))
			{
				window.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				m_keyHandler.updateKey(event.key.code, true);
			}
			if (event.type == sf::Event::KeyReleased)
			{
				m_keyHandler.updateKey(event.key.code, false);
			}
			if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space))
				if (action == "flock")
					action = "swarm";
				else
					action = "flock";
			if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z))
				action = "cformation";
			//check for mouse click, draws and adds boid to flock if so.
			else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
			{
				//Boid b(rand() % window_width, rand() % window_height); //Starts the boid with a random position in the window.
				Boid b(window_width / 3, window_height / 3); //Starts all boids in the center of the screen
				sf::CircleShape shape(20); //Shape with a radius of 10 and 3 points (Making it a triangle)

				//Changing the Visual Properties of the shape
				//shape.setPosition(b.location.x, b.location.y); //Sets position of shape to random location that boid was set to.
				shape.setPosition(window_width, window_height); //Testing purposes, starts all shapes in the center of screen.
				//shape.setOutlineColor(sf::Color(0, 255, 0));
				//if (i == fLeader) // So we can see our leader when in formation
				//	shape.setFillColor(sf::Color::Blue);
				//else
				//	shape.setFillColor(sf::Color::Green);
				//shape.setOutlineColor(sf::Color::White);
				//shape.setOutlineThickness(1);
				shape.setRadius(boidsSize);
				shape.setTexture(&m_textures.get(Textures::Enemy));
				shape.setOrigin(shape.getGlobalBounds().width / 2, shape.getGlobalBounds().height / 2);

				//Adding the boid to the flock and adding the shapes to the vector<sf::CircleShape>
				flock.addBoid(b);
				shapes.push_back(shape);

				//New Shape is drawn
				window.draw(shapes[shapes.size() - 1]);
			}
		}
		//Clears previous frames of visualization to not have clutter. (And simulate animation)
		window.clear();
		///////////////////////////////////////////////////////////
		//// UPDATE
		///////////////////////////////////////////////////////////
		m_keyHandler.update();
		m_player.update(0.01f);



		///////////////////////////////////////////////////////////
		//// DRAW
		///////////////////////////////////////////////////////////
		m_player.draw(window);
		//Draws all of the Boids out, and applies functions that are needed to update.
		for (int i = 0; i < shapes.size(); i++)
		{
			window.draw(shapes[i]);

			//Cout's removed due to slowdown and only needed for testing purposes
			//cout << "Boid "<< i <<" Coordinates: (" << shapes[i].getPosition().x << ", " << shapes[i].getPosition().y << ")" << endl;
			//cout << "Boid Code " << i << " Location: (" << flock.getBoid(i).location.x << ", " << flock.getBoid(i).location.y << ")" << endl;

			//Matches up the location of the shape to the boid
			shapes[i].setPosition(flock.getBoid(i).location.x, flock.getBoid(i).location.y);

			// Calculates the angle where the velocity is pointing so that the triangle turns towards it.
			float theta;
			theta = flock.getBoid(i).angle(flock.getBoid(i).velocity);
			shapes[i].setRotation(theta);

			// These if statements prevent boids from moving off the screen through warpping
			// If boid exits right boundary
			if (shapes[i].getPosition().x > window_width)
				shapes[i].setPosition(shapes[i].getPosition().x - window_width, shapes[i].getPosition().y);
			// If boid exits bottom boundary
			if (shapes[i].getPosition().y > window_height)
				shapes[i].setPosition(shapes[i].getPosition().x, shapes[i].getPosition().y - window_height);
			// If boid exits left boundary
			if (shapes[i].getPosition().x < 0)
				shapes[i].setPosition(shapes[i].getPosition().x + window_width, shapes[i].getPosition().y);
			// If boid exits top boundary
			if (shapes[i].getPosition().y < 0)
				shapes[i].setPosition(shapes[i].getPosition().x, shapes[i].getPosition().y + window_height);
		}

		//Applies the three rules to each boid in the flock and changes them accordingly.
		if (action == "flock")
			flock.flocking();
		else if (action == "swarm")
			flock.swarming();
		else
			flock.cFormation(m_player); //Pass in the index position of the leader.

		//Updates the window with current values of any data that was modified.
		window.display();

	//This loop continues until window is closed. Continues the process of updating, drawing, rendering the boids, and the window.
	}
	return 0;

}