/*********************************************************************
* Dogic class                                								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef DOGIC_H
#define DOGIC_H

#include "puzzle.h"

class Vect<3>;

//----------------------------------------------------------------------

struct DogicSettings : public PuzzleSettings
{
	static const unsigned int TOTAL_CELLS=20;
	static const unsigned int TOTAL_MODULOS=12;
};

//----------------------------------------------------------------------

class Dogic : public Puzzle<DogicSettings>
{
	public:
		Dogic(std::shared_ptr<RSpace<3>> RS, Vect<3>* vertices);
		virtual ~Dogic()
		{};
};

//----------------------------------------------------------------------

#endif
