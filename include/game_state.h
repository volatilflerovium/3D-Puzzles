/*********************************************************************
* GameState class                            								*
* Initial, Submenu, Play, Type2Puzzle, PlayDogic, PlayDogic12 class  *
* PlayRubik class                              								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "constants.h"
#include "button.h"
#include "puzzle_interface.h"
#include "rubik.h"
#include "dogic.h"
#include "dogic12.h"
#include "user_input.h"
#include "assertion.h"
#include "game_state_machine.h"

//----------------------------------------------------------------------

class GameState
{
	public:
		virtual ~GameState()
		{};

		virtual void enter()=0;
		virtual void execute()=0;
		virtual void eventHandler(const sf::Event& event)=0;	
		virtual void exit()=0;
		virtual void processInput(){};
		virtual void render()=0;
};

//======================================================================
//######################################################################
//======================================================================

class Initial0 : public GameState
{
	public:
		Initial0();
		~Initial0();
		virtual void enter();
		virtual void execute(){};
		virtual void eventHandler(const sf::Event& event);
		virtual void exit()
		{}

		virtual void render();

		void addButton(Button* button);

	protected:
		std::vector<Button*> m_buttons;
		int m_posX;
};

//----------------------------------------------------------------------

inline Initial0::Initial0()
:
m_posX(40)
{}

//----------------------------------------------------------------------

inline void Initial0::addButton(Button* button)
{
	m_buttons.push_back(button);	
}

//======================================================================
//######################################################################
//======================================================================

class Initial : public Initial0
{
	public:
		Initial();
		virtual ~Initial()
		{};

		virtual void render();

	protected:
		sf::Text m_label;
		sf::Texture m_texture;
		sf::Sprite m_sprite;
		
		struct Btns
		{
			enum btns
			{
				Play=0,
				Dogic,
				Rubik,
				Load,
				Exit,
				Total,
			};
		};
};

//======================================================================
//######################################################################
//======================================================================

class Submenu : public Initial0
{
	public:
		Submenu()
		{};

		virtual ~Submenu()
		{
			exit();
		};
		
		virtual void enter();
		virtual void exit();

	private:		
		struct Btns
		{
			enum btns
			{
				LoadSession=0,
				NewGame,
			};
		};
};

//======================================================================
//######################################################################
//======================================================================

class Play : public GameState
{
	public:
		Play();
		virtual ~Play();
		virtual void enter()=0;
		virtual void execute();
		virtual void eventHandler(const sf::Event& event);
		virtual void exit();
		virtual void render();

		void loadLastSession();
		void clearFile();

	protected:
		virtual bool loadFile();
		virtual void scramble();
		virtual void setHanders();
		virtual void dumpData();
		virtual void mkFile(const char* fileName);

		std::vector<Instruction> m_instructions;
		PuzzleI* m_puzzle;
		UII* m_handlers;// one for each camera
		int m_iterations;
		bool m_ready;
		bool m_loadSession{false};
		char m_fileName[100];
};

//----------------------------------------------------------------------

inline void Play::loadLastSession()
{
	m_loadSession=true;
}

//----------------------------------------------------------------------

inline void Play::clearFile()
{
	std::ofstream gameSession;
	gameSession.open(m_fileName, std::ifstream::out | std::ifstream::trunc);
	gameSession<<"";
	gameSession.close();	
}

//----------------------------------------------------------------------

inline void Play::mkFile(const char* fileName)
{
	memset(m_fileName, 0, 100*sizeof(char));
	int len=0;
	if(getenv("HOME")){
		char* homeDir=getenv("HOME");
		len=strlen(homeDir);
		if(len<80){
			memcpy(m_fileName, homeDir, len*sizeof(char));
			m_fileName[len]='/';
			len++;
		}
	}
	memcpy(m_fileName+len, fileName, strlen(fileName)*sizeof(char));
	//std::cout<<m_fileName<<"\n";
}
//======================================================================
//######################################################################
//======================================================================

template<typename T, bool = std::is_base_of<PuzzleI, T>::value>
class Type2Puzzle
{};
		
template<typename Type>
class Type2Puzzle<Type, true> : public Play
{
	public:
		Type2Puzzle(){}
		virtual ~Type2Puzzle(){}

		virtual void enter();
	
};

template<typename Type>
void Type2Puzzle<Type, true>::enter()
{
	setHanders();
	m_puzzle=new Type(GameStateMachine::Global, m_handlers->m_corners);
	if(!(m_loadSession && loadFile())){
		clearFile();
		scramble();
	}			
	m_handlers->updateView();
}	

//======================================================================
//######################################################################
//======================================================================

class PlayDogic : public Type2Puzzle<Dogic>
{
	public:
		PlayDogic()
		{
			m_iterations=30+rand()%30;
			mkFile(DOGIC_FILE);
		};

		virtual ~PlayDogic()
		{};
};

//======================================================================
//######################################################################
//======================================================================

class PlayDogic12 : public Type2Puzzle<Dogic12>
{
	public:
		PlayDogic12()
		{
			m_iterations=30+rand()%20;
			mkFile(DOGIC12_FILE);
		};

		virtual ~PlayDogic12()
		{};
};

//======================================================================
//######################################################################
//======================================================================

class PlayRubik : public Type2Puzzle<Rubik>
{
	public:
		PlayRubik()
		{
			m_iterations=21+rand()%15;
			mkFile(RUBIK_FILE);
		};

		virtual ~PlayRubik()
		{};
		
	protected:
		virtual void setHanders();
};

//----------------------------------------------------------------------

#endif
