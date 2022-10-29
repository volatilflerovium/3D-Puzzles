#include <iostream>
#include <ctime>
#include <SFML/Graphics.hpp>

#include <cassert>

#include "../include/constants.h"
#include "../include/base.h"
#include "../include/framerate.h"
#include "../include/rspace.h"
#include "../include/camera.h"
#include "../include/state_machine.h"
#include "../include/game_state.h"
#include "../include/game_state_machine.h"
#include "../include/reference_frame.h"
#include "../include/utilities.h"

//######################################################################

int main()
{
	#ifndef NDEBUG
		std::cout<<"Set release 1 in cmake\n";
	#endif

	srand(time(0));
	sf::ContextSettings settings;
	settings.antialiasingLevel=8;

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "3D Puzzles", sf::Style::Close, settings);

	window.setFramerateLimit(48);

	Base::setWindow(&window);

	GameStateMachine* stateMachine=GameStateMachine::getStateMachine();

	stateMachine->loadState(States::Initial);

	FrameRate Framerate(148);

	sf::Event event;
	while(window.isOpen()){		
		while(window.pollEvent(event)){
			if(event.type==sf::Event::Closed){
				window.close();
			}
			else{
				stateMachine->eventHandler(event);
			}
		}

		if(Framerate()>=0){
			Framerate.reset();

			window.clear(sf::Color::White);

			//################################################################
			//draw things here ###############################################

			stateMachine->executeState();
			
			stateMachine->render();

			// END of drawing ###############################################
			//###############################################################
			Framerate.reset();
		}

		window.display();
	}

	GameStateMachine::terminate();

	return 0;
}


