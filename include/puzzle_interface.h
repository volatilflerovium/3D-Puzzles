/*********************************************************************
*  PuzzleI class                               								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef PUZZLE_INTERFACE_H
#define PUZZLE_INTERFACE_H

struct Instruction;

//----------------------------------------------------------------------

class PuzzleI
{
	public:
		virtual ~PuzzleI(){};
		virtual bool isReady2() const=0;
		virtual void draw()=0;
		virtual void execute()=0;
		virtual void setMove(const Instruction& instruction)=0;
		virtual void setFastMove(const Instruction& instruction)=0;
		virtual int getTotalModulos() const=0;
};

#endif
