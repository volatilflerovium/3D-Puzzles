/*********************************************************************
* Tetra, Tetra12 class                         								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022  (Reviewed 04/2025)                            *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef TETRA_H
#define TETRA_H

#include "cell.h"

//----------------------------------------------------------------------

struct TetraSettings : public CellSettings
{
	static const int FACE_NUM=4;
	static const int VERTEX_NUM=4;
	static const int VERTEX_IN_FACE=3;
	static const int NUM_EDGES=6;
	static const int NUM_PINS=3;
};

//----------------------------------------------------------------------

class Tetra : public LogicCell<TetraSettings>
{
	public:
		Tetra(std::shared_ptr<RSpace<3>> _RS, const Vect<3>& fv0, const Vect<3>& fv1, const Vect<3>& fv2, double vl, int id=0);
		virtual ~Tetra()=default;

		void updateEdges();
		void setFace(int idx, int f0, int f1, int f2);
};

//----------------------------------------------------------------------

inline void Tetra::setFace(int idx, int f0, int f1, int f2)
{
	m_facesOrder[idx][0]=f0;
	m_facesOrder[idx][1]=f1;
	m_facesOrder[idx][2]=f2;
}


//======================================================================
//######################################################################
//======================================================================

struct Tetra12Settings : public CellSettings
{
	static const int FACE_NUM=4;
	static const int VERTEX_NUM=4;
	static const int VERTEX_IN_FACE=3;
	static const int NUM_EDGES=6;
	static const int NUM_PINS=3;
};

//----------------------------------------------------------------------

class Tetra12 : public LogicCell<Tetra12Settings>
{
	public:
		Tetra12(std::shared_ptr<RSpace<3>> _RS, const Vect<3>& fv0, const Vect<3>& fv1, const Vect<3>& fv2, double vl, int id=0);
		virtual ~Tetra12(){};

		void updateEdges();
		void setFace(int idx, int f0, int f1, int f2);
		void paintFace(const Vect<3>& V, const sf::Color& colour);
};

//----------------------------------------------------------------------

inline void Tetra12::setFace(int idx, int f0, int f1, int f2)
{
	m_facesOrder[idx][0]=f0;
	m_facesOrder[idx][1]=f1;
	m_facesOrder[idx][2]=f2;
}


//----------------------------------------------------------------------

#endif
