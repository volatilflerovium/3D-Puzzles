#include "game_state.h"
#include "utilities.h"
#include "puzzle.h"

//----------------------------------------------------------------------

Initial0::~Initial0()
{
	for(size_t i=0; i<m_buttons.size(); i++){
		if(m_buttons[i]){
			delete m_buttons[i];
		}
	}
}

//----------------------------------------------------------------------

void Initial0::enter()
{
	int height=0;
	for(size_t i=0; i<m_buttons.size(); i++){
		if(m_buttons[i]){
			height+=m_buttons[i]->getSize().y+10;
		}
	}

	for(size_t i=0; i<m_buttons.size(); i++){
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
	for(size_t i=0; i<m_buttons.size(); i++){
		if(m_buttons[i]){
			m_buttons[i]->draw();
		}
	}
}

//----------------------------------------------------------------------

void Initial0::eventHandler(const sf::Event& event)
{
	for(size_t i=0; i<m_buttons.size(); i++){
		if(m_buttons[i]){
			m_buttons[i]->eventHandler(event);
		}
	}
}

//====================================================================
//====================================================================

Initial::Initial()
{
	m_label=LabelMaker<FontLMR>("Press 'Esc' to return to this menu", 18);
	m_label.setFillColor(sf::Color::Black);

	m_label.setPosition(Normalization::absolute(WINDOW_WIDTH-m_label.getGlobalBounds().width-20, 44));	

	std::string coverFile=resourcePath("puzzleCover.png");

	m_texture.loadFromFile(coverFile, sf::IntRect(0, 0, 843, 938));

	m_sprite.setTexture(m_texture);
	m_sprite.setScale(sf::Vector2f(0.75f, 0.75f));
	m_sprite.setPosition(sf::Vector2f(300.f, 50.f));
}

//----------------------------------------------------------------------

void Initial::render()
{
	Initial0::render();
	Base::m_window->draw(m_label);
	Base::m_window->draw(m_sprite);
}

//====================================================================
//====================================================================

void Submenu::enter()
{
	int width=0;
	for(size_t i=0; i<m_buttons.size(); i++){
		if(m_buttons[i]){
			width+=m_buttons[i]->getSize().x;
		}
	}

	m_posX=(WINDOW_WIDTH-(width+20))/2;
	int posY=(WINDOW_HEIGHT-m_buttons[0]->getSize().y)/2;
	for(size_t i=0; i<m_buttons.size(); i++){
		if(m_buttons[i]){
			m_buttons[i]->setPosition(m_posX, posY);
			m_posX+=m_buttons[i]->getSize().x+20;
		}
	}
}

//----------------------------------------------------------------------

void Submenu::exit()
{
	for(size_t i=0; i<m_buttons.size(); i++){
		if(m_buttons[i]){
			delete m_buttons[i];
		}
	}
	m_buttons.clear();
};

//====================================================================
//====================================================================

Play::Play()
:m_puzzle(nullptr)
, m_uiiHandlers(nullptr)
, m_ready(false)
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
		if(strInstruction.length()>0){
			Instruction instruction(strInstruction);
			if(instruction.isValid()){
				Command2 cmd;
				cmd.m_action=derivedAction(PuzzleFastMove(instruction));
				m_uiiHandlers->m_commandQueue.push(cmd);
			}
		}
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
	int mod, dir;
	while((m_uiiHandlers->m_commandQueue).size()<m_iterations){
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
		m_uiiHandlers->m_commandQueue.push(cmd);
	}
};

//----------------------------------------------------------------------

void Play::execute()
{
	m_ready=true;
	if(m_puzzle->isReady2()){
		if(!(m_uiiHandlers->m_commandQueue).isEmpty()){
			Command2 tmp=m_uiiHandlers->m_commandQueue.pop();
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
	m_puzzle->execute();// this execute the current state (idel, rotating...)
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
		m_uiiHandlers->updateView();
	}
	else{
		if(!m_ready){
			return;
		}
		m_uiiHandlers->eventHandler(event);		
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
	if(m_uiiHandlers){
		delete m_uiiHandlers;
		m_uiiHandlers=nullptr;
	}
}

//----------------------------------------------------------------------

void Play::setHanders()
{
	m_uiiHandlers=new GroupReactiveButtons<3, 20>(12);

	m_uiiHandlers->setVertix(0,  1, phi, 0, scale*SL);
	m_uiiHandlers->setVertix(1, -1, phi, 0, scale*SL);
	m_uiiHandlers->setVertix(2, -1, -1.0*phi, 0, scale*SL);
	m_uiiHandlers->setVertix(3,  1, -1.0*phi, 0, scale*SL);
	
	m_uiiHandlers->setVertix(4, phi, 0, 1, scale*SL);
	m_uiiHandlers->setVertix(5, -1.0*phi, 0, 1, scale*SL);
	m_uiiHandlers->setVertix(6, -1.0*phi, 0, -1, scale*SL);
	m_uiiHandlers->setVertix(7, phi, 0, -1, scale*SL);
	
	m_uiiHandlers->setVertix(8, 0, 1, phi, scale*SL);
	m_uiiHandlers->setVertix(9, 0, -1, phi, scale*SL);
	m_uiiHandlers->setVertix(10, 0, -1, -1.0*phi, scale*SL);
	m_uiiHandlers->setVertix(11, 0, 1, -1.0*phi, scale*SL);

	m_uiiHandlers->setCorners(0, {1, 0, 11});
	m_uiiHandlers->setModulos(0, {5,0,1});
	m_uiiHandlers->setCorners(1, {1, 11, 6});
	m_uiiHandlers->setModulos(1, {4,0,5});
	m_uiiHandlers->setCorners(2, {1, 6, 5});
	m_uiiHandlers->setModulos(2, {3,0, 4});
	m_uiiHandlers->setCorners(3, {1, 5, 8});	
	m_uiiHandlers->setModulos(3, {2,0,3});
	m_uiiHandlers->setCorners(4, {1, 8, 0});
	m_uiiHandlers->setModulos(4, {1,0,2});

	m_uiiHandlers->setCorners(5, {7, 4, 3});
	m_uiiHandlers->setModulos(5, {11,6,7});
	m_uiiHandlers->setCorners(6, {7, 3, 10});
	m_uiiHandlers->setModulos(6, {10, 6,11});
	m_uiiHandlers->setCorners(7, {3, 2, 10});
	m_uiiHandlers->setModulos(7, {10,11,9});
	m_uiiHandlers->setCorners(8, {2, 3, 9});
	m_uiiHandlers->setModulos(8, {8, 9,11});
	m_uiiHandlers->setCorners(9, {4, 9, 3});
	m_uiiHandlers->setModulos(9, {11,7,8});

	m_uiiHandlers->setCorners(10, {11, 7, 10});
	m_uiiHandlers->setModulos(10, {10,5,6});
	m_uiiHandlers->setCorners(11, {11, 0, 7});
	m_uiiHandlers->setModulos(11, {6,5,1});
	m_uiiHandlers->setCorners(12, {0, 4, 7});
	m_uiiHandlers->setModulos(12, {6,1,7});
	m_uiiHandlers->setCorners(13, {0, 8, 4});
	m_uiiHandlers->setModulos(13, {7, 1,2});
	m_uiiHandlers->setCorners(14, {8, 9, 4});
	m_uiiHandlers->setModulos(14, {7,2, 8});

	m_uiiHandlers->setCorners(15, {8, 5, 9});
	m_uiiHandlers->setModulos(15, {8,2,3});
	m_uiiHandlers->setCorners(16, {5, 2, 9});
	m_uiiHandlers->setModulos(16, {8,3,9});
	m_uiiHandlers->setCorners(17, {5, 6, 2});
	m_uiiHandlers->setModulos(17, {9,3,4});
	m_uiiHandlers->setCorners(18, {6, 2, 10});
	m_uiiHandlers->setModulos(18, {10,4,9});
	m_uiiHandlers->setCorners(19, {6, 10, 11});
	m_uiiHandlers->setModulos(19, {5, 4,10});

	for(int i=0; i<20; i++){
		if(i==18 || i==19){
			m_uiiHandlers->setOrientation(i, {1, 1, 1});
			continue;
		}
		m_uiiHandlers->setOrientation(i, {-1,-1,-1});
	}
};

//----------------------------------------------------------------------

void Play::dumpData()
{
	std::ofstream gameSession;
	gameSession.open(m_fileName, std::ifstream::app);
	for(size_t i=0; i<m_instructions.size(); i++){
		gameSession<<m_instructions[i]<<"\n";
	}
	m_instructions.clear();
	gameSession.close();
}

//====================================================================
//====================================================================

void PlayRubik::setHanders()
{
	m_uiiHandlers=new GroupReactiveButtons<4, 36>(32);	

	int i=0;
	float m=-1.5;
	for(int j=0; j<4; j++){
		
		m_uiiHandlers->setVertix(i++, ((1.0*j)-1.5)*SL, -1.5*SL, -1.5*SL);

		if(j==0 || j==3){
			m= j==0?-1.5 : 1.5;
			m_uiiHandlers->setVertix(i++, m*SL, -0.5*SL, -1.5*SL);
			m_uiiHandlers->setVertix(i++, m*SL, 0.5*SL, -1.5*SL);
		}

		m_uiiHandlers->setVertix(i++, ((1.0*j)-1.5)*SL, 1.5*SL, -1.5*SL);

		if(j==0 || j==3){
			m= j==0?-1.5 : 1.5;
			m_uiiHandlers->setVertix(i++, m*SL, 1.5*SL, -0.5*SL);
			m_uiiHandlers->setVertix(i++, m*SL, 1.5*SL, 0.5*SL);
		}

		m_uiiHandlers->setVertix(i++, ((1.0*j)-1.5)*SL, 1.5*SL, 1.5*SL);

		if(j==0 || j==3){
			m= j==0?-1.5 : 1.5;
			m_uiiHandlers->setVertix(i++, m*SL, 0.5*SL, 1.5*SL);
			m_uiiHandlers->setVertix(i++, m*SL, -0.5*SL, 1.5*SL);
		}

		m_uiiHandlers->setVertix(i++, ((1.0*j)-1.5)*SL, -1.5*SL, 1.5*SL);

		if(j==0 || j==3){
			m= j==0?-1.5 : 1.5;
			m_uiiHandlers->setVertix(i++, m*SL, -1.5*SL, 0.5*SL);
			m_uiiHandlers->setVertix(i++, m*SL, -1.5*SL, -0.5*SL);
		}
	}

	/*
	 * The boundary of a module basically is the rectangle formed by the corners 
	 * of the module. Each edge of that rectangle is associated with the module
	 * that is adjacent to it.
	 * 
	 * For example:
	 * 
	 * m_boundaries[MODULES::id0].setCorners(0, 1, 2, 3);
	 * m_boundaries[MODULES::id0].setModulos(2, 5, 3, 4);
	 * 
	 * means that the edge from corner 0 to 1 is associated to the module 2
	 * the edge [1, 2] associated to module 5
	 * the edge [2, 3] associated to module 3
	 * the edge [3, 1] associated to module 4
	 * 
	 * */
	
	// X: m_modules[i]->setAction(m_mod5, m_mod3, m_mod4, m_mod2);

	// Face M0
	m_uiiHandlers->setCorners(0, {0, 1, 8, 9});
	m_uiiHandlers->setCorners(1, {1, 2, 7, 8});
	m_uiiHandlers->setCorners(2, {2,3,6,7});
	m_uiiHandlers->setCorners(3, {0,3,4,11});
	m_uiiHandlers->setCorners(4, {11,4,5,10});
	m_uiiHandlers->setCorners(5, {10,5,6,9});

	m_uiiHandlers->setModulos(0,{2,2,2,2});
	m_uiiHandlers->setModulos(1,{7,7,7,7});
	m_uiiHandlers->setModulos(2,{3,3,3,3});
	m_uiiHandlers->setModulos(3,{4,4,4,4});
	m_uiiHandlers->setModulos(4,{8,8,8,8});
	m_uiiHandlers->setModulos(5,{5,5,5,5});

	// Face M1 green
	m_uiiHandlers->setCorners(6, {23,22,27,26});
	m_uiiHandlers->setCorners(7, {22,21,28,27});
	m_uiiHandlers->setCorners(8, {21,20,29,28});
	m_uiiHandlers->setCorners(9, {23,20,31,24});
	m_uiiHandlers->setCorners(10, {24,31,30,25});
	m_uiiHandlers->setCorners(11, {25,30,29,26});

	m_uiiHandlers->setModulos(6,{3,3,3,3});
	m_uiiHandlers->setModulos(7,{7,7,7,7});
	m_uiiHandlers->setModulos(8,{2,2,2,2});
	m_uiiHandlers->setModulos(9, {4,4,4,4});
	m_uiiHandlers->setModulos(10,{8,8,8,8});
	m_uiiHandlers->setModulos(11,{5,5,5,5});

	//Face M4
	m_uiiHandlers->setCorners(12, {23,17,16,20});
	m_uiiHandlers->setCorners(13, {17,13,12,16});
	m_uiiHandlers->setCorners(14, {13,3,0,12});
	m_uiiHandlers->setCorners(15, {23,3,2,22});
	m_uiiHandlers->setCorners(16, {22,2,1,21});
	m_uiiHandlers->setCorners(17, {21,1,0,20});

	m_uiiHandlers->setModulos(12,{1,1,1,1});
	m_uiiHandlers->setModulos(13,{6,6,6,6});
	m_uiiHandlers->setModulos(14,{0,0,0,0});
	m_uiiHandlers->setModulos(15,{3,3,3,3});
	m_uiiHandlers->setModulos(16,{7,7,7,7});
	m_uiiHandlers->setModulos(17,{2,2,2,2});

	//Face M5
	m_uiiHandlers->setCorners(18, {29,19,18,26});
	m_uiiHandlers->setCorners(19, {19,15,14,18});
	m_uiiHandlers->setCorners(20, {15,9,6,14});
	m_uiiHandlers->setCorners(21, {29,9,8,28});
	m_uiiHandlers->setCorners(22, {28,8,7,27});
	m_uiiHandlers->setCorners(23, {27,7,6,26});

	m_uiiHandlers->setModulos(18,{1,1,1,1});
	m_uiiHandlers->setModulos(19,{6,6,6,6});
	m_uiiHandlers->setModulos(20,{0,0,0,0});
	m_uiiHandlers->setModulos(21,{2,2,2,2});
	m_uiiHandlers->setModulos(22,{7,7,7,7});
	m_uiiHandlers->setModulos(23,{3,3,3,3});

	//Face M2 yellow
	m_uiiHandlers->setCorners(24, {20,16,19,29});
	m_uiiHandlers->setCorners(25, {16,12,15,19});
	m_uiiHandlers->setCorners(26, {12,0,9,15});
	m_uiiHandlers->setCorners(27, {20,0,11,31});
	m_uiiHandlers->setCorners(28, {31,11,10,30});
	m_uiiHandlers->setCorners(29, {30,10,9,29});

	m_uiiHandlers->setModulos(24,{1,1,1,1});
	m_uiiHandlers->setModulos(25,{6,6,6,6});
	m_uiiHandlers->setModulos(26,{0,0,0,0});
	m_uiiHandlers->setModulos(27,{4,4,4,4});
	m_uiiHandlers->setModulos(28,{8,8,8,8});
	m_uiiHandlers->setModulos(29,{5,5,5,5});

	//Face M3
	m_uiiHandlers->setCorners(30, {26,18,17,23});
	m_uiiHandlers->setCorners(31, {18,14,13,17});
	m_uiiHandlers->setCorners(32, {14,6,3,13});
	m_uiiHandlers->setCorners(33, {26,6,5,25});
	m_uiiHandlers->setCorners(34, {25,5,4,24});
	m_uiiHandlers->setCorners(35, {24,4,3,23});

	m_uiiHandlers->setModulos(30,{1,1,1,1});
	m_uiiHandlers->setModulos(31,{6,6,6,6});
	m_uiiHandlers->setModulos(32,{0,0,0,0});
	m_uiiHandlers->setModulos(33,{5,5,5,5});
	m_uiiHandlers->setModulos(34,{8,8,8,8});
	m_uiiHandlers->setModulos(35,{4,4,4,4});

	for(int k=0; k<36; k++){
		if(k<6){//blue
			if(k<3){
				m_uiiHandlers->setOrientation(k,{0,-1,0,1});
			}
			else{
				m_uiiHandlers->setOrientation(k,{1,0,-1,0});
			}
			continue;
		}
		if(k<12){//green
			if(k<9){
				m_uiiHandlers->setOrientation(k,{0,1,0,-1});
			}
			else{
				m_uiiHandlers->setOrientation(k,{1,0,-1,0});
			}
			continue;
		}

		if(k<18){//pink
			if(k<15){
				m_uiiHandlers->setOrientation(k,{0,1,0,-1});
			}
			else{
				m_uiiHandlers->setOrientation(k,{-1,0,1,0});
			}
			continue;
		}

		if(k<24){//cyan
			if(k<21){
				m_uiiHandlers->setOrientation(k,{0,1,0,-1});
			}
			else{
				m_uiiHandlers->setOrientation(k,{1,0,-1,0});
			}
			continue;
		}
		if(k<30){//yellow
			if(k<27){
				m_uiiHandlers->setOrientation(k,{0,1,0,-1});
			}
			else{
				m_uiiHandlers->setOrientation(k,{1,0,-1,0});
			}
			continue;
		}
		if(k<33){//blue
			m_uiiHandlers->setOrientation(k,{0,1,0,-1});
		}
		else{
			m_uiiHandlers->setOrientation(k,{-1,0,1,0});
		}
	}
};

//----------------------------------------------------------
