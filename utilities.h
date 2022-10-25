/*********************************************************************
* Instructions struct                              						*
* PuzzleMove, PuzzleFastMove struct   						               *
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef UTILITIES_H
#define UTILITIES_H

#include <functional>
#include <vector>
#include <string>

#include "constants.h"
#include "vector.h"

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

inline std::ostream& operator<<(std::ostream& out, const Instruction inst)
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

//======================================================================
//######################################################################
//======================================================================

std::string trim(const std::string& str);

//======================================================================

void mkfile(const char* filePath);

//----------------------------------------------------------------------

#endif
