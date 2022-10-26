#include <filesystem>
#include "../include/utilities.h"
#include "../include/game_state.h"
#include "../include/puzzle.h"
#include "../include/rubik.h"
#include "../include/dogic.h"
#include "../include/dogic12.h"

//----------------------------------------------------------------------

Initial0::~Initial0()
{
	for(int i=0; i<m_buttons.size(); i++){
		if(m_buttons[i]){
			delete m_buttons[i];
		}
	}
}

//----------------------------------------------------------------------

void Initial0::enter()
{
	int height=0;
	for(int i=0; i<m_buttons.size(); i++){
		if(m_buttons[i]){
			height+=m_buttons[i]->getSize().y+10;
		}
	}

	for(int i=0; i<m_buttons.size(); i++){
		if(m_buttons[i]){			
			m_buttons[i]->setPosition(m_posX, height);
			height-=10;
			height-=m_buttons[i]->getSize().y;
		}
	}
}

//----------------------------------------------------------------------

void Initial0::render()
{
	for(int i=0; i<m_buttons.size(); i++){
		if(m_buttons[i]){
			m_buttons[i]->draw();
		}
	}
}

//----------------------------------------------------------------------

void Initial0::eventHandler(const sf::Event& event)
{
	for(int i=0; i<m_buttons.size(); i++){
		if(m_buttons[i]){
			m_buttons[i]->eventHandler(event);
		}
	}
}

//======================================================================
//######################################################################
//======================================================================

Initial::Initial()
{
	m_label=LabelMaker<FontLMR>("Press 'Esc' to return to this menu", 18);
	m_label.setFillColor(sf::Color::Black);

	sf::Rect<float> txtSize=m_label.getGlobalBounds();
	m_label.setPosition(Normalization::absolute(WINDOW_WIDTH-m_label.getGlobalBounds().width-20, 44));	

	//std::cout<<std::filesystem::current_path()<<"\n";
	m_texture.loadFromFile("resources/puzzleCover.png", sf::IntRect(0, 0, 843, 938));
	m_sprite.setTexture(m_texture);
	m_sprite.setScale(sf::Vector2f(0.75f, 0.75f));
	m_sprite.setPosition(sf::Vector2f(300.f, 50.f)); // absolute position
}

//----------------------------------------------------------------------

void Initial::render()
{
	Initial0::render();
	Normalization::m_window->draw(m_label);
	Normalization::m_window->draw(m_sprite);
}

//======================================================================
//######################################################################
//======================================================================

void Submenu::enter()
{
	int width=0;
	for(int i=0; i<m_buttons.size(); i++){
		if(m_buttons[i]){
			width+=m_buttons[i]->getSize().x;
		}
	}

	m_posX=(WINDOW_WIDTH-(width+20))/2;
	int height=m_buttons[0]->getSize().y+10;
	int posY=(WINDOW_HEIGHT-m_buttons[0]->getSize().y)/2;
	for(int i=0; i<m_buttons.size(); i++){
		if(m_buttons[i]){
			m_buttons[i]->setPosition(m_posX, posY);
			m_posX+=m_buttons[i]->getSize().x+20;
		}
	}
}

//----------------------------------------------------------------------

void Submenu::exit()
{
	for(int i=0; i<m_buttons.size(); i++){
		if(m_buttons[i]){
			delete m_buttons[i];
		}
	}
	m_buttons.clear();
};

//======================================================================
//######################################################################
//======================================================================

Play::Play()
:
m_puzzle(nullptr),
m_ready(false)
{
	m_instructions.reserve(60);	
}

//----------------------------------------------------------------------

Play::~Play()
{
	if(m_puzzle){
		exit();
	}
}

//----------------------------------------------------------------------

bool Play::loadFile()
{
	std::ifstream lastSession;
	lastSession.open(m_fileName, std::ifstream::in);
	if(!lastSession.is_open()){
		return false;
	}

	std::string strInstruction;
	strInstruction.reserve(100);
	while(std::getline(lastSession, strInstruction)){	
		Command2 cmd;
		cmd.m_action=derivedAction(PuzzleFastMove(Instruction(strInstruction)));
		m_handlers->m_commandQueue.push(cmd);

		strInstruction.clear();
	}
	lastSession.close();
	return true;
}

//----------------------------------------------------------------------

void Play::scramble()
{
	static Instruction prevInstruction(-1, 0, false);
	m_ready=false;
	int mod, dir, j;
	while((m_handlers->m_commandQueue).size()<m_iterations){
		mod=rand()%m_puzzle->getTotalModulos();
		dir=(rand()%3)-1;
		if(dir==0){
			continue;
		}

		if(prevInstruction.m_moduloID==mod && prevInstruction.m_direction!=dir){
			continue;
		}
		prevInstruction.m_moduloID=mod;
		prevInstruction.m_direction=dir;
		prevInstruction.m_short=rand()%2;
		
		Command2 cmd;
		cmd.m_action=derivedAction(PuzzleMove(mod, dir, prevInstruction.m_short));//PuzzleSetMove(resolveInput(boundaryIdx, enterPoint, mousePosition)));
		m_handlers->m_commandQueue.push(cmd);
	}
};

//----------------------------------------------------------------------

void Play::execute()
{
	m_ready=true;
	if(m_puzzle->isReady2()){
		if(!(m_handlers->m_commandQueue).isEmpty()){
			Command2 tmp=m_handlers->m_commandQueue.pop();
			Instruction tmpIns=tmp.m_action(m_puzzle);
			if(tmpIns.isValid()){
				m_instructions.push_back(tmpIns);
				if(m_instructions.size()>30){
					dumpData();
				}
			}
			m_ready=false;
		}
	}
	m_puzzle->execute();
}

//----------------------------------------------------------------------

void Play::render()
{
	m_puzzle->draw();
}

//----------------------------------------------------------------------

void Play::eventHandler(const sf::Event& event)
{
	if(event.key.code>70 && event.key.code<75){
		GameStateMachine::m_CameraView.lRuD(event.key.code-71);
		m_handlers->updateView();
	}
	else{
		if(!m_ready){
			return;
		}
		m_handlers->eventHandler(event);		
	}
}

//----------------------------------------------------------------------

void Play::exit()
{
	dumpData();
	if(m_puzzle){
		delete m_puzzle;
		m_puzzle=nullptr;
	}

	if(m_handlers->m_corners){
		delete[] m_handlers->m_corners;
		m_handlers->m_corners=nullptr;
	}
}

//----------------------------------------------------------------------

void Play::setHanders()
{
	m_handlers=new GroupReactiveButtons<3, 20>(&GameStateMachine::m_CameraView);
	m_handlers->m_corners=new Vect<3>[12];

	m_handlers->m_corners[0]=scale*SL*Vect<3>{1, phi, 0};
	m_handlers->m_corners[1]=scale*SL*Vect<3>{-1, phi, 0};
	m_handlers->m_corners[2]=scale*SL*Vect<3>{-1, -1.0*phi, 0};
	m_handlers->m_corners[3]=scale*SL*Vect<3>{1, -1.0*phi, 0};
	
	m_handlers->m_corners[4]=scale*SL*Vect<3>{phi, 0, 1};
	m_handlers->m_corners[5]=scale*SL*Vect<3>{-1.0*phi, 0, 1};
	m_handlers->m_corners[6]=scale*SL*Vect<3>{-1.0*phi, 0, -1};
	m_handlers->m_corners[7]=scale*SL*Vect<3>{phi, 0, -1};
	
	m_handlers->m_corners[8]=scale*SL*Vect<3>{0, 1, phi};
	m_handlers->m_corners[9]=scale*SL*Vect<3>{0, -1, phi};
	m_handlers->m_corners[10]=scale*SL*Vect<3>{0, -1, -1.0*phi};
	m_handlers->m_corners[11]=scale*SL*Vect<3>{0, 1, -1.0*phi};

	m_handlers->setCorners(0, {1, 0, 11});
	m_handlers->setModulos(0, {5,0,1});
	m_handlers->setCorners(1, {1, 11, 6});
	m_handlers->setModulos(1, {4,0,5});
	m_handlers->setCorners(2, {1, 6, 5});
	m_handlers->setModulos(2, {3,0, 4});
	m_handlers->setCorners(3, {1, 5, 8});	
	m_handlers->setModulos(3, {2,0,3});
	m_handlers->setCorners(4, {1, 8, 0});
	m_handlers->setModulos(4, {1,0,2});

	m_handlers->setCorners(5, {7, 4, 3});
	m_handlers->setModulos(5, {11,6,7});
	m_handlers->setCorners(6, {7, 3, 10});
	m_handlers->setModulos(6, {10, 6,11});
	m_handlers->setCorners(7, {3, 2, 10});
	m_handlers->setModulos(7, {10,11,9});
	m_handlers->setCorners(8, {2, 3, 9});
	m_handlers->setModulos(8, {8, 9,11});
	m_handlers->setCorners(9, {4, 9, 3});
	m_handlers->setModulos(9, {11,7,8});

	m_handlers->setCorners(10, {11, 7, 10});
	m_handlers->setModulos(10, {10,5,6});
	m_handlers->setCorners(11, {11, 0, 7});
	m_handlers->setModulos(11, {6,5,1});
	m_handlers->setCorners(12, {0, 4, 7});
	m_handlers->setModulos(12, {6,1,7});
	m_handlers->setCorners(13, {0, 8, 4});
	m_handlers->setModulos(13, {7, 1,2});
	m_handlers->setCorners(14, {8, 9, 4});
	m_handlers->setModulos(14, {7,2, 8});

	m_handlers->setCorners(15, {8, 5, 9});
	m_handlers->setModulos(15, {8,2,3});
	m_handlers->setCorners(16, {5, 2, 9});
	m_handlers->setModulos(16, {8,3,9});
	m_handlers->setCorners(17, {5, 6, 2});
	m_handlers->setModulos(17, {9,3,4});
	m_handlers->setCorners(18, {6, 2, 10});
	m_handlers->setModulos(18, {10,4,9});
	m_handlers->setCorners(19, {6, 10, 11});
	m_handlers->setModulos(19, {5, 4,10});

	for(int i=0; i<20; i++){
		if(i==18 || i==19){
			m_handlers->setOrientation(i, {1, 1, 1});
			continue;
		}
		m_handlers->setOrientation(i, {-1,-1,-1});
	}
};

//----------------------------------------------------------------------

void Play::dumpData()
{
	std::ofstream gameSession;
	gameSession.open(m_fileName, std::ifstream::app);
	for(int i=0; i<m_instructions.size(); i++){
		gameSession<<m_instructions[i]<<"\n";
	}
	m_instructions.clear();
	gameSession.close();
}

//======================================================================
//######################################################################
//======================================================================

void PlayRubik::setHanders()
{
	m_handlers=new GroupReactiveButtons<4, 6>(&GameStateMachine::m_CameraView);	

	m_handlers->m_corners=new Vect<3>[8];
	m_handlers->m_corners[0]=Vect<3>{-1.5*SL, -1.5*SL, -1.5*SL};
	m_handlers->m_corners[1]=Vect<3>{-1.5*SL, -1.5*SL, 1.5*SL};
	m_handlers->m_corners[2]=Vect<3>{-1.5*SL, 1.5*SL, 1.5*SL};
	m_handlers->m_corners[3]=Vect<3>{-1.5*SL, 1.5*SL, -1.5*SL};

	m_handlers->m_corners[4]=Vect<3>{1.5*SL, -1.5*SL, -1.5*SL};
	m_handlers->m_corners[5]=Vect<3>{1.5*SL, -1.5*SL, 1.5*SL};
	m_handlers->m_corners[6]=Vect<3>{1.5*SL, 1.5*SL, 1.5*SL};
	m_handlers->m_corners[7]=Vect<3>{1.5*SL, 1.5*SL, -1.5*SL};	
			
	m_handlers->setCorners(0,{0, 1, 2, 3});
	m_handlers->setCorners(1,{4, 5, 6, 7});
	m_handlers->setCorners(2,{0, 1, 5, 4});
	m_handlers->setCorners(3,{3, 2, 6, 7});
	m_handlers->setCorners(4,{0, 4, 7, 3});
	m_handlers->setCorners(5,{1, 5, 6, 2});

	m_handlers->setModulos(0,{2, 5, 3, 4});
	m_handlers->setModulos(1,{2, 5, 3, 4});
	m_handlers->setModulos(2,{0, 5, 1, 4});
	m_handlers->setModulos(3,{0, 5, 1, 4});
	m_handlers->setModulos(4,{2, 1, 3, 0});
	m_handlers->setModulos(5,{2, 1, 3, 0});	

	m_handlers->setOrientation(0,{-1, 1, 1, -1});
	m_handlers->setOrientation(1,{1, -1, -1, 1});
	m_handlers->setOrientation(2,{1, -1, -1, 1});
	m_handlers->setOrientation(3,{-1, 1, 1, -1});
	m_handlers->setOrientation(4,{1, -1, -1, 1});
	m_handlers->setOrientation(5,{-1, 1, 1, -1});
};

//----------------------------------------------------------------------
