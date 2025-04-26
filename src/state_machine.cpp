#include "state_machine.h"
#include "modulo.h"

//====================================================================

Rotating::Rotating(double angle)
:c_angle(angle)
, m_currentAngle(0)
, m_direction(-1)
, m_signal(Signal::Ready)
{}

//--------------------------------------------------------------------

void Rotating::enter(ModuloI* modulo, const Instruction& instruction)
{
	if(m_signal==Signal::Busy){
		return;
	}
	m_currentModulo=modulo;
	m_direction=instruction.m_direction;
	if(m_direction==1){
		m_moduloRotation=&ModuloI::clockRotation;
	}
	else{
		m_moduloRotation=&ModuloI::antiClockRotation;
	}
	m_currentModulo->setFull(!instruction.m_short);
	m_currentModulo->disconnectCells();
}

//--------------------------------------------------------------------

Signal Rotating::execute()
{
	if(m_currentAngle<c_angle){
		m_currentAngle+=DELTA_ROTATION;
		(m_currentModulo->*m_moduloRotation)();
		m_signal=Signal::Busy;
	}
	else{
		m_currentAngle=0;
		m_signal=Signal::Ready;
	}
	
	return m_signal;
}

//--------------------------------------------------------------------

void Rotating::exit()
{
	assertm(m_currentModulo, "Modulo is null\n");
	m_currentModulo->reconnectCells(m_direction);
}

//====================================================================
//====================================================================

void FastRotating::enter(ModuloI* modulo, const Instruction& instruction)
{
	if(m_signal==Signal::Busy){
		return;
	}
	m_currentModulo=modulo;
	m_direction=instruction.m_direction;
	if(m_direction==1){
		m_moduloRotation=&ModuloI::fastClockRotation;
	}
	else{
		m_moduloRotation=&ModuloI::fastAntiClockRotation;
	}
	m_currentModulo->setFull(!instruction.m_short);
	m_currentModulo->disconnectCells();
}

//--------------------------------------------------------------------

Signal FastRotating::execute()
{
	(m_currentModulo->*m_moduloRotation)();
	m_currentAngle=0;
	m_signal=Signal::Ready;

	return m_signal;
}

//--------------------------------------------------------------------
