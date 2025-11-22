#include "game_state_machine.h"
#include "game_state.h"
#include "puzzle.h"
#include "camera.h"

//----------------------------------------------------------------------

std::shared_ptr<RSpace<3>> GameStateMachine::Global(RSpace<3>::maker({0, 0, 0}, {0, 0, 0}));

/*
Camera GameStateMachine::m_CameraView(GameStateMachine::Global->spawn({5700, 0, 0}, {0, 0, 0.0}), 1.0, 0.3, 0, 0, D_WINDOW_WIDTH/2.0, D_WINDOW_HEIGHT/2.0);
Camera GameStateMachine::m_CameraView2(GameStateMachine::Global->spawn({-5700, 0, 0}, {0, 0, 0.0}), 1.0, 0.3, D_WINDOW_WIDTH/2.0, D_WINDOW_HEIGHT/2.0, D_WINDOW_WIDTH/2.0, D_WINDOW_HEIGHT/2.0);
//*/

//----------------------------------------------------------------------

GameStateMachine::GameStateMachine()
:current_state(nullptr),
previous_state(nullptr)
{
	CameraManager::init<CAM::A>(GameStateMachine::Global->spawn({5700, 0, 0}, {0, 0, 0.0}), 1.0, 0.3, D_WINDOW_WIDTH/2.0, D_WINDOW_HEIGHT/2.0);
	//CameraManager::init<CAM::A>(GameStateMachine::Global->spawn({4700, 730, 0}, {0, 0, 0.0}), 1.0, 0.3, D_WINDOW_WIDTH/2.0, D_WINDOW_HEIGHT/2.0);

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
	static InstanceManager instance;
	return instance.m_stateMachinePtr;
}

//----------------------------------------------------------------------

GameStateMachine::~GameStateMachine()
{
	for(std::pair<States, GameState*> data : s_states){
		delete data.second;
	}
}

//----------------------------------------------------------------------

void GameStateMachine::loadState(States stateID)
{
	getStateMachine()->_loadState(stateID);
}

//----------------------------------------------------------------------

void GameStateMachine::loadSubMenu(States stateID)
{
	auto instancePtr=getStateMachine();
	instancePtr->_loadState(States::Submenu, [instancePtr, stateID](){
		dynamic_cast<Submenu*>(instancePtr->current_state)->addButton(new Button("Load Last Session", [=](){
			dynamic_cast<Play*>((instancePtr->s_states)[stateID])->loadLastSession(true);
			GameStateMachine::loadState(stateID);
		}));
	
		dynamic_cast<Submenu*>(instancePtr->current_state)->addButton(new Button("New Game", [=](){
			dynamic_cast<Play*>((instancePtr->s_states)[stateID])->loadLastSession(false);
			GameStateMachine::loadState(stateID);
		}));
	});
}

//----------------------------------------------------------------------

void GameStateMachine::executeState()
{
	getStateMachine()->current_state->execute();
}
//----------------------------------------------------------------------

void GameStateMachine::render()
{
	getStateMachine()->current_state->render();
}

//----------------------------------------------------------------------

void GameStateMachine::_loadState(States stateID, std::function<void(void)> cbk)
{
	if(s_states.find(stateID)!=s_states.end()){
		if(current_state){
			current_state->exit();
		}
		current_state=s_states[stateID];

		if(cbk){
			cbk();
		}

		current_state->enter();
	}
	else{
		throw "State not found.";
	}
}

//----------------------------------------------------------------------

void GameStateMachine::eventHandler(const sf::Event& event)
{
	if(event.key.code==sf::Keyboard::Escape){
		loadState(States::Initial);
	}
	else{
		getStateMachine()->current_state->eventHandler(event);
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
