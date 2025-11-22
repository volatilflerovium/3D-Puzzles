#include "base.h"
#include "camera.h"
#include "framerate.h"
#include "constants.h"
#include "game_state.h"
#include "game_state_machine.h"
#include "rspace.h"
#include "state_machine.h"
#include "reference_frame.h"

#include <ctime>

//######################################################################

int main()
{
	srand(time(0));
	sf::ContextSettings settings;
	settings.antialiasingLevel=8;

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "3D Puzzles", sf::Style::Close, settings);

	window.setFramerateLimit(48);

	Base::setWindow(&window);

	auto image = sf::Image{};
	if(image.loadFromFile("../resources/puzzle_icon.png"))
	{
		window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
	}

	GameStateMachine::loadState(States::Initial);

	FrameRate Framerate(148);

	sf::Event event;
	while(window.isOpen()){		
		while(window.pollEvent(event)){
			if(event.type==sf::Event::Closed){
				window.close();
			}
			else{
				CameraManager::eventHandler(event);
				GameStateMachine::eventHandler(event);
			}
		}

		if(Framerate()>=0){
			Framerate.reset();

			window.clear(sf::Color::White);

			//################################################################
			//draw things here ###############################################

			CameraManager::updateView();

			GameStateMachine::executeState();
			
			GameStateMachine::render();

			// END of drawing ###############################################
			//###############################################################
			Framerate.reset();
		}

		window.display();
	}

	return 0;
}


