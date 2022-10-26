/*********************************************************************
* Line class                                 								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef LINE_H
#define LINE_H

#include "vector.h"

//----------------------------------------------------------------------

class Line
{
	public:
		Line(const Vect<2>& A, const Vect<2>& B)
		{
			updateLine(A, B);
		}

		Line()
		{};

		virtual ~Line()
		{}

		void updateLine(const Vect<2>& A, const Vect<2>& B)
		{
			Vect<2> V=B-A;
			Vect<2> W=V.normalize()*(B.norm()*V.angCos(B));
			m_dir=B-W; // m_dir is actually perpendicular to the line L(B-A, Origin) and in L(A, B)
			m_d=Alg<2>::innerProduct(m_dir, B);
		}

		//README: since the return statement is a bool, it seems the method return type should be bool too, however, the way we are using this method (in Clickable<N>::update), it is more convenient to cast bool into a int. 
		int sider(double x, double y) const 		{
			return (m_dir[0]*x+m_dir[1]*y)<m_d;
		}
	
		int sider(const Vect<2>& a) const
		{
			return sider(a[0], a[1]);
		}

	private:
		Vect<2> m_dir;
		double m_d;
};

//----------------------------------------------------------------------

#endif
