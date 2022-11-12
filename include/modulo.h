/*********************************************************************
* ModuloI class                              								*
* ModuloR, ModuloT, ModuloT12 class            								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef MODULO_H
#define MODULO_H

#include <SFML/Graphics.hpp>
#include "vector.h"
#include "action.h"

//----------------------------------------------------------------------

class Cell;

class ModuloI
{
	public:
		ModuloI(const Vect<3>& axe, double ang, unsigned int cellsInModulo);
		ModuloI(const Vect<3>& axe, double ang, double fullAngle, unsigned int cellsInModulo);
		virtual ~ModuloI();

		virtual void disconnectCells();
		virtual void disconnect(Cell* cell);

		virtual void registerCell(Cell* cell, bool connectPin=false);
		virtual void registerCells(Cell* cell0, Cell* cell1, Cell* cell2, Cell* cell3,Cell* cell4)
		{}

		virtual void reconnectCells(int direction);
		virtual void reconnectCell(Cell* cell, int metaData=0);

		virtual void clockRotation();
		virtual void antiClockRotation();
		
		virtual void fastClockRotation();
		virtual void fastAntiClockRotation();

		virtual void setAction(ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3);
		virtual void setAction(ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3, ModuloI* m4);

		virtual void setFull(bool){}

		virtual void paint(int faceID, const sf::Color& colour);
		virtual Vect<3> getCentroid();

		void print();
		void setID(int id);
		void sayId();
		void setAction(Action* action);

	protected:		
		Rotation<3> m_rotation;
		Rotation<3> m_fastRotation;
		Vect<3> m_corner;	
		Cell** m_cells;
		Action* m_action;		
	
		int m_id;
		const unsigned int m_cellsInModulo;
};

//----------------------------------------------------------------------

inline ModuloI::~ModuloI()
{
	if(m_action){
		delete m_action;
	}

	delete m_cells;
}

//----------------------------------------------------------------------

inline void ModuloI::setAction(Action* action){
	if(!m_action){
		m_action=action;
	}
}

//----------------------------------------------------------------------

inline void ModuloI::setID(int id){
	m_id=id;
}

//----------------------------------------------------------------------

inline void ModuloI::sayId(){
	std::cout<<"modulo: "<<m_id<<"\n";
}

//----------------------------------------------------------------------

inline void ModuloI::setAction(ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3)
{
	std::cout<<"override\n";
}

inline void ModuloI::setAction(ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3, ModuloI* m4)
{
	std::cout<<"override\n";
}

//======================================================================
//######################################################################
//======================================================================

class ModuloR : public ModuloI
{
	public:
		ModuloR(const Vect<3>& axe, double ang, unsigned int cellsInModulo=9)
		:ModuloI(axe, ang, 90.0, cellsInModulo)
		{};
		
		~ModuloR()
		{};

		void setAction(ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3);
};

//----------------------------------------------------------------------

inline void ModuloR::setAction(ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3)
{
	m_action=new Action4(this, m0, m1, m2, m3);
}

//======================================================================
//######################################################################
//======================================================================

class ModuloT : public ModuloI
{
	public:
		ModuloT(const Vect<3>& axe, double ang, unsigned int cellsInModulo=5)
		:ModuloI(axe, ang, 72.0, cellsInModulo)
		{}

		virtual ~ModuloT()
		{}

		void setAction(ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3, ModuloI* m4);

		void registerCells(Cell* cell0, Cell* cell1, Cell* cell2, Cell* cell3,Cell* cell4);

		void paint();

};

//----------------------------------------------------------------------

inline void ModuloT::setAction(ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3, ModuloI* m4)
{
	m_action=new Action5(this, m0, m1, m2, m3, m4);
}

//======================================================================
//######################################################################
//======================================================================

class ModuloT12 : public ModuloI
{
	public:
		ModuloT12(const Vect<3>& axe, double ang);

		virtual ~ModuloT12()
		{}

		void setAction(ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3, ModuloI* m4);

		void reconnectCells(int direction);
		void disconnect(Cell* cell);
		void disconnectCells();

		void clockRotation();
		void antiClockRotation();
		void fastClockRotation();
		void fastAntiClockRotation();
		void reconnectCell(Cell* cell, int metaData);
		void registerUpperCells(Cell* cell);//5
		void registerLowerCells(Cell* cell);//15

		void setFull(bool isFull);

	protected:
		const int m_cellsInModulo1;
		const int m_cellsInModulo2;
		bool m_full;		
};

//----------------------------------------------------------------------

inline void ModuloT12::setAction(ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3, ModuloI* m4)
{
	m_action=new Action5(this, m0, m1, m2, m3, m4);
}

//----------------------------------------------------------------------

inline void ModuloT12::setFull(bool isFull)
{
	m_full=isFull;
}

//----------------------------------------------------------------------

#endif
