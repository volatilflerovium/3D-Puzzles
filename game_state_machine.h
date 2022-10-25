/*********************************************************************
* GameStateMachine class                       								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef GAME_STATE_MACHINE_H
#define GAME_STATE_MACHINE_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "utilities.h"
#include "camera.h"

//----------------------------------------------------------------------

class GameState;

class GameStateMachine
{
	public:
		~GameStateMachine();
		
		void executeState();
		void eventHandler(const sf::Event& event);
		void render();		
		
		static GameStateMachine* getStateMachine();

		static void loadState(States state);
		static void loadSubMenu(States state);
		static void terminate();

		static Camera m_CameraView;
		static Camera m_CameraView2;
		static std::shared_ptr<RSpace<3>> Global;

	private:
		GameStateMachine();
		void registerState(States stateID, GameState* state);
		std::map<States, GameState*> s_states;
		GameState* current_state;
		GameState* previous_state;
		static GameStateMachine* m_instance;
};

#endif
