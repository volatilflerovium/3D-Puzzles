/*********************************************************************
* Instructions struct                              						*
* PuzzleMove, PuzzleFastMove struct   						               *
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022  (Reviewed 04/2025)                            *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef UTILITIES_H
#define UTILITIES_H
#include "constants.h"
#include "vector.h"

#include <functional>
#include <vector>
#include <string>
#include <bitset>

#include <SFML/Graphics/Text.hpp>

//======================================================================

class PuzzleI;

struct Instruction
{
	Instruction(int m, int d, bool isShort=false);
	Instruction(const std::string& str);

	bool isValid() const;
	bool isValid(int maxID) const;

	int m_moduloID;
	int m_direction;
	bool m_short;
};	

//----------------------------------------------------------------------

inline Instruction::Instruction(int m, int d, bool isShort)
:m_moduloID(m), m_direction(d), m_short(isShort)
{}

//----------------------------------------------------------------------

inline bool Instruction::isValid() const
{
	return !(m_moduloID<0);
}

//----------------------------------------------------------------------

inline bool Instruction::isValid(int maxID) const
{
	return !(m_moduloID<0) && m_moduloID<maxID;
}

//======================================================================

inline std::ostream& operator<<(std::ostream& out, const Instruction& inst)
{
	out<<inst.m_moduloID<<" "<<inst.m_direction<<" "<<static_cast<int>(inst.m_short);
	return out;
}

//======================================================================

struct Command2
{
	Command2()
	{};

	std::function<Instruction(PuzzleI*)> m_action;
};

//======================================================================

// Notice that cbk needs to be captured in the closure of the lambda!
template <typename Function>
std::function<Instruction(PuzzleI*)> derivedAction(Function cbk)
{
	return [=] (PuzzleI* puzzle){
		return cbk(puzzle);
	};
}

//======================================================================
//######################################################################
//======================================================================

enum class Signal
{
	Busy=0,
	Ready,
	Valid,
	Idel
};

//======================================================================
//######################################################################
//======================================================================

enum Direction
{
	AntiClockwise=-1,
	Error,
	Clockwise,
};

//======================================================================
//######################################################################
//======================================================================

enum class States
{
	Initial,
	Load,
	Play,
	Dogic,
	Dogic12,
	Rubik,
	Exit,
	Submenu,
	Last
};

//====================================================================

std::string trim(const std::string& str);

//====================================================================

void mkfile(const char* filePath);

//====================================================================

struct EE
{
	EE()
	{
		std::memset(m_dd, 0, 5*sizeof(int8_t));
	}

	EE(const EE& e)
	{
		std::memcpy(m_dd, e.m_dd, 5*sizeof(uint8_t));
	}

	void operator()(uint i)
	{
		if(i<8){
			m_dd[0]=m_dd[0] | 1<<i;
		}
		else if(i<16){
			m_dd[1]=m_dd[1] | 1<<(i-8);
		}
		else if(i<24){
			m_dd[2]=m_dd[2] | 1<<(i-16);
		}
		else if(i<32){
			m_dd[3]=m_dd[3] | 1<<(i-24);
		}
		else{
			m_dd[4]=m_dd[4] | 1<<(i-32);
		}
	}

	EE operator=(const EE& e)
	{
		std::memcpy(m_dd, e.m_dd, 5*sizeof(uint8_t));
		return *this;
	}
	
	EE operator&(EE e)
	{
		EE f;
		for(int i=0; i<5; i++){
			f.m_dd[i]=m_dd[i]&e.m_dd[i];
		}
		return f;
	}

	EE operator|(EE e)
	{
		EE f;
		for(int i=0; i<5; i++){
			f.m_dd[i]=m_dd[i] | e.m_dd[i];
		}
		return f;
	}

	bool isValid() const
	{
		bool a=false;
		for(int i=0; i<5; i++){
			a=a || m_dd[i]!=0;
		}
		return a;
	}

	int8_t m_dd[5];
};

//====================================================================

inline std::ostream& operator<<(std::ostream& out, const EE& e)
{
	for(int i=0; i<5; i++){
		out<<std::bitset<8>(e.m_dd[4-i]);
	}
	return out;
}

//====================================================================

std::string resourcePath(const char* fileName);

//====================================================================

template<char const* fontFile>
sf::Text LabelMaker(const char* text, unsigned int characterSize=30)
{
	static bool isSet=false;
	static sf::Font font;
	if(!isSet){
		std::string fontPath=resourcePath("fonts/");
		fontPath.append(fontFile);
		isSet=font.loadFromFile(fontPath);
	}		

	return sf::Text(text, font, characterSize);
}

//====================================================================


#endif
