/*********************************************************************
* GameStateMachine class                       								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022  (Reviewed 04/2025)                            *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef GAME_STATE_MACHINE_H
#define GAME_STATE_MACHINE_H
#include "reference_frame.h"
#include "rspace.h"
#include "utilities.h"

#include <memory>

//----------------------------------------------------------------------

class GameState;

class GameStateMachine
{
	public:
		GameStateMachine(const GameStateMachine&)=delete;
		~GameStateMachine();
		
		static void executeState();
		static void eventHandler(const sf::Event& event);
		static void render();		
		
		static void loadState(States state);
		static void loadSubMenu(States state);

		static std::shared_ptr<RSpace<3>> Global;

	private:
		std::map<States, GameState*> s_states;
		GameState* current_state;
		GameState* previous_state;

		struct InstanceManager
		{
			InstanceManager()
			{
				m_stateMachinePtr=new GameStateMachine;
			}

			~InstanceManager()
			{
				if(m_stateMachinePtr){
					delete m_stateMachinePtr;
				}
			}

			GameStateMachine* m_stateMachinePtr;
		};

		GameStateMachine();
		
		void registerState(States stateID, GameState* state);
		void _loadState(States state, std::function<void(void)> cbk=nullptr);

		static GameStateMachine* getStateMachine();
};

//====================================================================

#endif
