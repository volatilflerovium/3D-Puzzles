/*********************************************************************
* Dogic class                                								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022  (Reviewed 04/2025)                            *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef DOGIC_H
#define DOGIC_H

#include "puzzle.h"

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
		Dogic(std::shared_ptr<RSpace<3>> RS, const Vect<3>* vertices);
		virtual ~Dogic()=default;

	private:
		void registerCells(ModuloI* module, Cell* cell0, Cell* cell1, Cell* cell2, Cell* cell3,Cell* cell4);
};

//----------------------------------------------------------------------

#endif
