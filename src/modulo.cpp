#include <iostream>

#include "modulo.h"
#include "cell.h"
#include "utilities.h"

//======================================================================

ModuloI::ModuloI(const Vect<3>& axe, double ang, unsigned int cellsInModulo)
: ModuloI(axe, ang, ang, cellsInModulo)
{}

//======================================================================

ModuloI::ModuloI(const Vect<3>& axe, double ang, double fullAngle, unsigned int cellsInModulo)
: m_rotation(axe, ang)
, m_fastRotation(axe, fullAngle)
, m_action(nullptr)
, m_cellsInModulo(cellsInModulo)
{
	m_cells=new Cell*[m_cellsInModulo];
	for(uint i=0; i<m_cellsInModulo; i++){
		m_cells[i]=nullptr;
	}
}

//----------------------------------------------------------------------

ModuloI::~ModuloI()
{
	if(m_action){
		delete m_action;
	}

	if(m_cells){
		delete[] m_cells;
		m_cells=nullptr;
	}
}

//----------------------------------------------------------------------

void ModuloI::disconnectCells()
{
	for(uint i=0; i<m_cellsInModulo; i++){
		if(m_cells[i]){
			m_cells[i]->isolate(this);
		}
	}
}

//----------------------------------------------------------------------

void ModuloI::disconnect(Cell* cell)
{
	for(uint i=0; i<m_cellsInModulo; i++){
		if(m_cells[i]==cell){
			m_cells[i]=nullptr;					
			break;
		}
	}
}

//----------------------------------------------------------------------

void ModuloI::reconnectCells(int direction)
{
	for(uint i=0; i<m_cellsInModulo; i++){
		if(m_cells[i]){
			m_cells[i]->executeAction(m_action, direction);
		}
	}
}

//----------------------------------------------------------------------

void ModuloI::registerCell(Cell* cell, bool connectPin)
{
	for(uint i=0; i<m_cellsInModulo; i++){
		if(!m_cells[i]){
			m_cells[i]=cell;
			if(connectPin){
				m_cells[i]->setPin(this);
			}
			return;
		}
	}
}

//----------------------------------------------------------------------

void ModuloI::reconnectCell(Cell* cell, int metaData)
{
	for(uint i=0; i<m_cellsInModulo; i++){
		if(!m_cells[i]){
			m_cells[i]=cell;
			return;
		}
	}
}

//----------------------------------------------------------------------

void ModuloI::antiClockRotation()
{
	for(uint i=0; i<m_cellsInModulo; i++){
		m_cells[i]->revRotate(m_rotation);
	}
}

//----------------------------------------------------------------------

void ModuloI::clockRotation()
{
	for(uint i=0; i<m_cellsInModulo; i++){
		m_cells[i]->rotate(m_rotation);
	}
}

//----------------------------------------------------------------------

void ModuloI::fastAntiClockRotation()
{
	for(uint i=0; i<m_cellsInModulo; i++){
		m_cells[i]->revRotate(m_fastRotation);
	}
}

//----------------------------------------------------------------------

void ModuloI::fastClockRotation()
{
	for(uint i=0; i<m_cellsInModulo; i++){
		m_cells[i]->rotate(m_fastRotation);
	}
}
//----------------------------------------------------------------------

void ModuloI::print()
{
	for(uint i=0; i<m_cellsInModulo; i++){
		if(m_cells[i]){
			m_cells[i]->debug();
		}
	}
}

//----------------------------------------------------------------------

void ModuloI::paint(int faceID, const sf::Color& colour)
{
	for(uint i=0; i<m_cellsInModulo; i++){
		m_cells[i]->paintFace(faceID, colour);
	}
}

//----------------------------------------------------------------------

Vect<3> ModuloI::getCentroid()
{
	Vect<3> v(0,0);
	for(uint i=0; i<m_cellsInModulo; i++){
		if(m_cells[i]){
			v+=m_cells[i]->getCentroidW();
		}
	}
	return v;
}

//======================================================================

void ModuloR::reconnectCells(int direction)
{
	for(uint i=0; i<m_cellsInModulo; i++){
		if(m_cells[i]){
			m_cells[i]->executeAction(m_action, direction);
			m_cells[i]->executeAction(m_action2, direction);
		}
	}
}

//----------------------------------------------------------------------

void ModuloR::setAction(ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3, ModuloI* m4)
{
	m_action=new Action4(this, m0, m1, m2, m3);
}

//======================================================================
//######################################################################
//======================================================================

void ModuloT::registerCells(Cell* cell0, Cell* cell1, Cell* cell2, Cell* cell3, Cell* cell4)
{
	m_cells[0]=cell0;
	m_cells[1]=cell1;
	m_cells[2]=cell2;
	m_cells[3]=cell3;
	m_cells[4]=cell4;

	m_cells[0]->setPin(this);
	m_cells[1]->setPin(this);
	m_cells[2]->setPin(this);
	m_cells[3]->setPin(this);
	m_cells[4]->setPin(this);
}

//----------------------------------------------------------------------

void ModuloT::paint()
{
	m_cells[0]->paintFace(2, sf::Color::Red); 
	m_cells[1]->paintFace(2, sf::Color::Green);
	m_cells[2]->paintFace(2, sf::Color::Yellow);
	m_cells[3]->paintFace(2, sf::Color::Blue);
	m_cells[4]->paintFace(2, sf::Color::Magenta);
}


//----------------------------------------------------------------------

void ModuloT::setAction(ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3, ModuloI* m4)
{
	m_action=new Action5(this, m0, m1, m2, m3, m4);
}

//----------------------------------------------------------------------
//######################################################################
//----------------------------------------------------------------------

ModuloT12::ModuloT12(const Vect<3>& axe, double ang)//, unsigned int cells1, unsigned int cells2)
:ModuloI(axe, ang, 72.0, 20),
m_cellsInModulo1(5),
m_cellsInModulo2(15),
m_full(false)
{}

//----------------------------------------------------------------------

void ModuloT12::disconnectCells()
{
	for(uint i=0; i<m_cellsInModulo1; i++){
		m_cells[i]->isolate(this);
	}

	if(m_full){		
		for(uint i=m_cellsInModulo1; i<m_cellsInModulo1+m_cellsInModulo2; i++){
			m_cells[i]->isolate(this);
		}
	}
}

//----------------------------------------------------------------------

void ModuloT12::disconnect(Cell* cell)
{
	for(uint i=0; i<m_cellsInModulo1; i++){
		if(m_cells[i]==cell){
			m_cells[i]=nullptr;
			break;
		}
	}

	for(uint i=m_cellsInModulo1; i<m_cellsInModulo1+m_cellsInModulo2; i++){
		if(m_cells[i]==cell){
			m_cells[i]=nullptr;
			break;
		}
	}
}

//----------------------------------------------------------------------

void ModuloT12::reconnectCells(int direction)
{
	for(uint i=0; i<m_cellsInModulo1; i++){
		m_cells[i]->executeAction(m_action, direction);
	}

	if(m_full){
		for(uint i=m_cellsInModulo1; i<m_cellsInModulo1+m_cellsInModulo2; i++){
			m_cells[i]->executeAction(m_action, direction);
		}
	}
}

//----------------------------------------------------------------------

void ModuloT12::clockRotation()
{
	for(uint i=0; i<m_cellsInModulo1; i++){
		m_cells[i]->rotate(m_rotation);
	}

	if(m_full){
		for(uint i=m_cellsInModulo1; i<m_cellsInModulo1+m_cellsInModulo2; i++){
			m_cells[i]->rotate(m_rotation);
		}
	}
}

//----------------------------------------------------------------------

void ModuloT12::antiClockRotation()
{
	for(uint i=0; i<m_cellsInModulo1; i++){
		m_cells[i]->revRotate(m_rotation);
	}

	if(m_full){
		for(uint i=m_cellsInModulo1; i<m_cellsInModulo1+m_cellsInModulo2; i++){
			m_cells[i]->revRotate(m_rotation);
		}
	}
}

//----------------------------------------------------------------------

void ModuloT12::fastClockRotation()
{
	for(uint i=0; i<m_cellsInModulo1; i++){
		m_cells[i]->rotate(m_fastRotation);
	}

	if(m_full){
		for(uint i=m_cellsInModulo1; i<m_cellsInModulo1+m_cellsInModulo2; i++){
			m_cells[i]->rotate(m_fastRotation);
		}
	}
}

//----------------------------------------------------------------------

void ModuloT12::fastAntiClockRotation()
{
	for(uint i=0; i<m_cellsInModulo1; i++){
		m_cells[i]->revRotate(m_fastRotation);
	}

	if(m_full){
		for(uint i=m_cellsInModulo1; i<m_cellsInModulo1+m_cellsInModulo2; i++){
			m_cells[i]->revRotate(m_fastRotation);
		}
	}
}

//----------------------------------------------------------------------

void ModuloT12::reconnectCell(Cell* cell, int metaData)
{
	if(metaData==1){
		for(uint i=0; i<m_cellsInModulo1; i++){
			if(m_cells[i]==nullptr){
				m_cells[i]=cell;
				m_cells[i]->setPin(this);
				return;
			}
		}
		return;
	}

	for(uint i=m_cellsInModulo1; i<m_cellsInModulo1+m_cellsInModulo2; i++){
		if(m_cells[i]==nullptr){
			m_cells[i]=cell;
			m_cells[i]->setPin(this);
			return;
		}
	}
}

//----------------------------------------------------------------------

void ModuloT12::registerUpperCells(Cell* cell)//5
{
	for(uint i=0; i<m_cellsInModulo1; i++){
		if(m_cells[i]==nullptr){
			m_cells[i]=cell;
			m_cells[i]->setPin(this);
			m_cells[i]->setMetadata(this, 1);
			break;
		}
	}
}

//----------------------------------------------------------------------

void ModuloT12::registerLowerCells(Cell* cell)//15
{
	for(uint i=m_cellsInModulo1; i<m_cellsInModulo1+m_cellsInModulo2; i++){
		if(m_cells[i]==nullptr){
			m_cells[i]=cell;
			m_cells[i]->setPin(this);
			break;
		}
	}
}

//----------------------------------------------------------------------

void ModuloT12::setAction(ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3, ModuloI* m4)
{
	m_action=new Action5(this, m0, m1, m2, m3, m4);
}

//----------------------------------------------------------------------

void ModuloT12::setFull(bool isFull)
{
	m_full=isFull;
}

//----------------------------------------------------------------------
