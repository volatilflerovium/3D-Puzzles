/*********************************************************************
*  Puzzle class                                								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022  (Reviewed 04/2025)                            *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef PUZZLE_H
#define PUZZLE_H
#include "base.h"
#include "cell.h"
#include "constants.h"
#include "modulo.h"
#include "puzzle_interface.h"
#include "state_machine.h"

//====================================================================

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

		void draw(CAM c);

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
:m_paintOrder(Data::TOTAL_CELLS, 0)
, m_distances(Data::TOTAL_CELLS, 0.0)
, m_RS(RS)
, m_fastRotationState{new FastRotating(rotationAngle)}
, m_rotationState{new Rotating(rotationAngle)}
, m_idelState{new Idel()}
, m_currentState{m_idelState}
, m_signal(Signal::Ready)
, m_ready2{true}
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

// virtual methods/function should not be inlined!
template<typename Data>
bool Puzzle<Data, true>::isReady2() const
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

// virtual methods/function should not be inlined!
template<typename Data>
void Puzzle<Data, true>::setMove(const Instruction& instruction)
{
	setMoveState(instruction, m_rotationState);
}

//----------------------------------------------------------------------
// virtual methods/function should not be inlined!
template<typename Data>
void Puzzle<Data, true>::setFastMove(const Instruction& instruction)
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
	if(CameraManager::isCameraAvailable(CAM::A)){		
		draw(CAM::A);
	}
	if(CameraManager::isCameraAvailable(CAM::B)){
		draw(CAM::B);
	}
}

//----------------------------------------------------------------------

template<typename Data>
void Puzzle<Data, true>::draw(CAM c)
{
	CameraManager::setCamera(c);
	
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
