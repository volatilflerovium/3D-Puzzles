/*********************************************************************
* Rubik class                                 								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022  (Reviewed 04/2025)                            *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef RUBIK_H
#define RUBIK_H

#include "puzzle.h"

//======================================================================

struct RublikSettings : public PuzzleSettings
{
	static const unsigned int TOTAL_CELLS=27;
	static const unsigned int TOTAL_MODULOS=9;
	static const unsigned int CELLS_IN_MODULO=9;
};

//----------------------------------------------------------------------

class Rubik : public Puzzle<RublikSettings>
{
	enum LocalSettings{
		SLICES=3
	};

	public:
		Rubik(std::shared_ptr<RSpace<3>> RS, const Vect<3>* vertices);
		~Rubik()=default;
};

//----------------------------------------------------------------------

#endif
