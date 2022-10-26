/*********************************************************************
* State, Rotating, FastRotating, Idel class    								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <iostream>

#include "utilities.h"

//======================================================================
//######################################################################
//======================================================================

class ModuloI;

typedef void(ModuloI::* ModRotation)();

class State
{
	public:
		State()
		:m_currentModulo(nullptr)
		{};

		virtual ~State(){};
		virtual void enter(ModuloI* module, const Instruction& instruction)=0;
		virtual Signal execute()=0;
		virtual void exit()=0;
	
	protected:
		ModuloI* m_currentModulo;
};

//======================================================================
//######################################################################
//======================================================================

class Rotating : public State
{
	public:
		Rotating(double angle=90.0);
		void enter(ModuloI* modulo, const Instruction& instruction);
		Signal execute();
		void exit();

	protected:
		const double c_angle;
		double m_currentAngle;
		int m_direction;
		ModRotation m_moduloRotation;
		Signal m_signal;
};

//======================================================================
//######################################################################
//======================================================================

class FastRotating : public Rotating
{
	public:
		FastRotating(double angle=90.0)
		:Rotating(angle)
		{}

		virtual void enter(ModuloI* modulo, const Instruction& instruction);
		Signal execute();
};

//======================================================================
//######################################################################
//======================================================================

class Idel : public State
{
	public:
		Idel(){};
		void enter(ModuloI*, const Instruction& instruction){};
		Signal execute(){
			return Signal::Valid;
		};

		void exit(){};
};

//----------------------------------------------------------------------

#endif
