/*********************************************************************
* Cube class                                 								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef CUBE_H
#define CUBE_H

#include "cell.h"

struct CubeSettings : public CellSettings
{
	static const int FACE_NUM=6;
	static const int VERTEX_NUM=8;
	static const int VERTEX_IN_FACE=4;
	static const int NUM_EDGES=12;
	static const int NUM_PINS=3;
};

//----------------------------------------------------------------------

class Cube : public LogicCell<CubeSettings>
{
	public:
		Cube(std::shared_ptr<RSpace<3>> _RS, const Vect<3>& position, int id);
		virtual ~Cube()
		{}

		void updateEdges();
		void setFace(int idx, int f0, int f1, int f2, int f3);
};

//----------------------------------------------------------------------

inline void Cube::setFace(int idx, int f0, int f1, int f2, int f3)
{
	m_facesOrder[idx][0]=f0;
	m_facesOrder[idx][1]=f1;
	m_facesOrder[idx][2]=f2;
	m_facesOrder[idx][3]=f3;
}

//----------------------------------------------------------------------

#endif
