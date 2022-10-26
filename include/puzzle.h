/*********************************************************************
*  Puzzle class                                								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef PUZZLE_H
#define PUZZLE_H

#include <SFML/Graphics.hpp>
#include <functional>

#include "base.h"
#include "modulo.h"
#include "state_machine.h"
#include "utilities.h"
#include "constants.h"
#include "cell.h"
#include "line.h"
#include "puzzle_interface.h"


//######################################################################

struct PuzzleSettings
{
	static const unsigned int TOTAL_CELLS=27;
	static const unsigned int TOTAL_MODULOS=9;
};

//----------------------------------------------------------------------

template<typename Data, bool = std::is_base_of<PuzzleSettings, Data>::value>
class Puzzle 
{};

//----------------------------------------------------------------------

template<typename Data>
class Puzzle<Data, true> : public PuzzleI
{
	public:
		Puzzle(std::shared_ptr<RSpace<3>> RS, double rotationAngle);		
		virtual ~Puzzle();
		
		virtual void setMove(const Instruction& instruction);
		virtual void setFastMove(const Instruction& instruction);
		virtual bool isReady2() const;
		int getTotalModulos() const;
		void draw();
		void execute();
		
		void draw(Camera* cm);

	protected:
		void setMoveState(const Instruction& instruction, State* moveState);
		std::vector<int> m_paintOrder;
		std::vector<double> m_distances;
		std::shared_ptr<RSpace<3>> m_RS;
		ModuloI* m_modules[Data::TOTAL_MODULOS];
		Cell* m_cells[Data::TOTAL_CELLS];
		
		State* m_fastRotationState;
		State* m_rotationState;
		State* m_idelState;
		State* m_currentState;

		Signal m_signal;
		bool m_ready2;
};

//----------------------------------------------------------------------

template<typename Data>
Puzzle<Data, true>::Puzzle(std::shared_ptr<RSpace<3>> RS, double rotationAngle)
:m_RS(RS),
m_paintOrder(Data::TOTAL_CELLS, 0),
m_distances(Data::TOTAL_CELLS, 0.0),
m_fastRotationState{new FastRotating(rotationAngle)},
m_rotationState{new Rotating(rotationAngle)},
m_idelState{new Idel()},
m_currentState{m_idelState},
m_signal(Signal::Ready),
m_ready2{true}
{
	for(unsigned int i=0; i<Data::TOTAL_CELLS; i++){
		m_cells[i]=nullptr;
	}

	for(unsigned int i=0; i<Data::TOTAL_MODULOS; i++){
		m_modules[i]=nullptr;
	}
}

//----------------------------------------------------------------------

template<typename Data>
Puzzle<Data, true>::~Puzzle()
{
	for(unsigned int i=0; i<Data::TOTAL_CELLS; i++){
		delete m_cells[i];
	}
	for(unsigned int i=0; i<Data::TOTAL_MODULOS; i++){
		delete m_modules[i];
	}
	delete m_idelState;
	delete m_rotationState;
}

//----------------------------------------------------------------------

template<typename Data>
inline int Puzzle<Data, true>::getTotalModulos() const
{
	return Data::TOTAL_MODULOS;
}

//----------------------------------------------------------------------

template<typename Data>
inline bool Puzzle<Data, true>::isReady2() const
{
	return m_ready2;
}

//----------------------------------------------------------------------

template<typename Data>
inline void Puzzle<Data, true>::setMoveState(const Instruction& instruction, State* moveState)
{
	if(instruction.m_direction!=0 && instruction.isValid(Data::TOTAL_MODULOS)){
		m_currentState=moveState;
		m_currentState->enter(m_modules[instruction.m_moduloID], instruction);
		m_ready2=false;
	}
}

//----------------------------------------------------------------------

template<typename Data>
inline void Puzzle<Data, true>::setMove(const Instruction& instruction)
{
	setMoveState(instruction, m_rotationState);
}

//----------------------------------------------------------------------

template<typename Data>
inline void Puzzle<Data, true>::setFastMove(const Instruction& instruction)
{
	setMoveState(instruction, m_fastRotationState);
}

//----------------------------------------------------------------------

template<typename Data>
inline void Puzzle<Data, true>::execute()
{
	m_signal=m_currentState->execute();
	if(m_signal==Signal::Ready){
		m_currentState->exit();
		m_currentState=m_idelState;
		m_ready2=true;
	}
}

//----------------------------------------------------------------------

template<typename Data>
inline void Puzzle<Data, true>::draw()
{	
	if(Camera::m_Cameras[0]){
		draw(Camera::m_Cameras[0]);
	}
	if(Camera::m_Cameras[1]){
		draw(Camera::m_Cameras[1]);
	}
}

//----------------------------------------------------------------------

template<typename Data>
void Puzzle<Data, true>::draw(Camera* cm)
{
	Shape::setCamera(cm);

	for(unsigned int i=0; i<Data::TOTAL_CELLS; i++){
		m_distances[i]=m_cells[i]->quasiDistanceToCamera();
		m_paintOrder[i]=i;
	}

	sort(m_paintOrder.begin(), m_paintOrder.end(), [this](int i, int j){
		return m_distances[i]>m_distances[j];
	});

	for(unsigned int i=0; i<Data::TOTAL_CELLS; i++){
		m_cells[m_paintOrder[i]]->draw();
	}
}

//----------------------------------------------------------------------

#endif
