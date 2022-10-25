#include "game_state_machine.h"
#include "game_state.h"
#include "puzzle.h"

//----------------------------------------------------------------------

GameStateMachine* GameStateMachine::m_instance(nullptr);

std::shared_ptr<RSpace<3>> GameStateMachine::Global(RSpace<3>::maker({0, 0, 0}, {0, 0, 0}));

Camera GameStateMachine::m_CameraView(GameStateMachine::Global->spawn({5700, 0, 0}, {0, 0, 0.0}), 1.0, 0.3, D_WINDOW_WIDTH/2.0, D_WINDOW_HEIGHT/2.0);

//----------------------------------------------------------------------

GameStateMachine::GameStateMachine()
:current_state(nullptr),
previous_state(nullptr)
{
	Initial* initial=new Initial;
	registerState(States::Initial, initial);

	initial->addButton(new Button("Dogic", [](){
		GameStateMachine::loadSubMenu(States::Dogic);
	}));
	registerState(States::Dogic, new PlayDogic);

	initial->addButton(new Button("Dogic12", [](){
		GameStateMachine::loadSubMenu(States::Dogic12);
	}));
	registerState(States::Dogic12, new PlayDogic12);

	initial->addButton(new Button("Rubik", [](){
		GameStateMachine::loadSubMenu(States::Rubik);
	}));
	registerState(States::Rubik, new PlayRubik);//*/

	initial->addButton(new Button("Exit", [](){
		Base::m_window->close();
	}));

	Submenu* submenu=new Submenu;
	registerState(States::Submenu, submenu);
}

//----------------------------------------------------------------------

GameStateMachine* GameStateMachine::getStateMachine()
{
	if(!m_instance){
		m_instance=new GameStateMachine;
	}
	return m_instance;
}

//----------------------------------------------------------------------

GameStateMachine::~GameStateMachine()
{
	for(std::pair<States, GameState*> data : s_states){
		delete data.second;
	}
}

//----------------------------------------------------------------------

void GameStateMachine::terminate()
{
	delete m_instance;
}

//----------------------------------------------------------------------

void GameStateMachine::loadState(States stateID)
{
	if(m_instance->s_states.find(stateID)!=m_instance->s_states.end()){
		if(m_instance->current_state){
			m_instance->current_state->exit();
		}
		m_instance->current_state=(m_instance->s_states)[stateID];
		m_instance->current_state->enter();
	}
	else{
		throw "State not found.";
	}
}

//----------------------------------------------------------------------

void GameStateMachine::loadSubMenu(States stateID)
{
	if(m_instance->s_states.find(States::Submenu)!=m_instance->s_states.end()){
		if(m_instance->current_state){
			m_instance->current_state->exit();
		}
		m_instance->current_state=(m_instance->s_states)[States::Submenu];
		m_instance->current_state->exit();
		
		dynamic_cast<Submenu*>(m_instance->current_state)->addButton(new Button("Load Last Session", [=](){
			dynamic_cast<Play*>((m_instance->s_states)[stateID])->loadLastSession();
			GameStateMachine::loadState(stateID);
		}));
	
		dynamic_cast<Submenu*>(m_instance->current_state)->addButton(new Button("New Game", [=](){
			GameStateMachine::loadState(stateID);
		}));
		m_instance->current_state->enter();
	}
	else{
		throw "State not found.";
	}
}
//----------------------------------------------------------------------

void GameStateMachine::executeState()
{
	current_state->execute();
}
//----------------------------------------------------------------------

void GameStateMachine::render()
{
	current_state->render();
}

//----------------------------------------------------------------------

void GameStateMachine::eventHandler(const sf::Event& event)
{
	if(event.key.code==sf::Keyboard::Escape){
		loadState(States::Initial);
	}
	else{
		current_state->eventHandler(event);
	}
}	

//----------------------------------------------------------------------

void GameStateMachine::registerState(States stateID, GameState* state)
{
	if(s_states.find(stateID)==s_states.end()){
		s_states[stateID]=state;
	}
}

//----------------------------------------------------------------------
