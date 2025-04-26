/*********************************************************************
* ModuloI class                              								*
* ModuloR, ModuloT, ModuloT12 class            								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022  (Reviewed 04/2025)                            *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef MODULO_H
#define MODULO_H
#include "action.h"
#include "vector.h"

#include <SFML/Graphics/Color.hpp>

//----------------------------------------------------------------------

class Cell;

class ModuloI
{
	public:
		ModuloI(const Vect<3>& axe, double ang, unsigned int cellsInModulo);
		ModuloI(const Vect<3>& axe, double ang, double fullAngle, unsigned int cellsInModulo);

		virtual ~ModuloI();

		virtual void setAction(ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3, ModuloI* m4=nullptr)=0;
		virtual void setFull(bool){}

		virtual void setAction(Action* action);

		virtual void disconnectCells();
		virtual void disconnect(Cell* cell);

		virtual void registerCell(Cell* cell, bool connectPin=false);
		
		virtual void reconnectCells(int direction);
		virtual void reconnectCell(Cell* cell, int metaData=0);

		virtual void clockRotation();
		virtual void antiClockRotation();
		
		virtual void fastClockRotation();
		virtual void fastAntiClockRotation();

		void paint(int faceID, const sf::Color& colour);
		Vect<3> getCentroid();

		void setID(int id);
		void print();

	protected:		
		Rotation<3> m_rotation;
		Rotation<3> m_fastRotation;
		Vect<3> m_corner;	
		Cell** m_cells;
		Action* m_action;

		const uint m_cellsInModulo;
		int m_id;		
};

//----------------------------------------------------------------------

inline void ModuloI::setAction(Action* action)
{
	if(!m_action){
		m_action=action;
	}
}

//----------------------------------------------------------------------

inline void ModuloI::setID(int id)
{
	m_id=id;
}

//======================================================================
//######################################################################
//======================================================================

class ModuloR : public ModuloI
{
	public:
		ModuloR(const Vect<3>& axe, double ang, unsigned int cellsInModulo=9)
		:ModuloI(axe, ang, 90.0, cellsInModulo)
		, m_action2(nullptr)
		{};
		
		virtual ~ModuloR()
		{
			if(m_action2){
				delete m_action2;
			}
		}

		virtual void reconnectCells(int direction);
		virtual void setAction(ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3, ModuloI* m4=nullptr) override;
		void setAction2(ModuloI* m0, ModuloI* m1);

	private:
		Action* m_action2;
};

//----------------------------------------------------------------------

inline void ModuloR::setAction2(ModuloI* m0, ModuloI* m1)
{
	m_action2=new Action2(this, m0, m1);
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

		virtual ~ModuloT()=default;

		virtual void setAction(ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3, ModuloI* m4=nullptr) override;

		void registerCells(Cell* cell0, Cell* cell1, Cell* cell2, Cell* cell3,Cell* cell4);

		void paint();
};

//======================================================================
//######################################################################
//======================================================================

class ModuloT12 : public ModuloI
{
	public:
		ModuloT12(const Vect<3>& axe, double ang);

		virtual ~ModuloT12()=default;

		virtual void setAction(ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3, ModuloI* m4=nullptr) override;

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
		const uint m_cellsInModulo1;
		const uint m_cellsInModulo2;
		bool m_full;		
};

//----------------------------------------------------------------------

#endif
