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
/*
 * Given two points P and Q, the line L(P, Q) can be described as following
 * 
 * Let V be a vector such that there is a t such that tV=Q-P 
 * (i.e V is parallel to the line passing through P-Q).
 * 
 * Therefore, tV+P describes the line L(P, Q). 
 * 
 * Now, let W be in L(P, Q) and perpendicular to V and d=P dotProct W. 
 * Then any point O in the line L(P, Q) satisfies:
 * 
 * O dotProct W = (tV+P) dotProct W (since  O=tV+P for some t) 
 * 					=t(V dotProct W) + P dotProct W
 * 					=t*0+ P dotProct W
 * 					=d
 * 
 * Notice that for any point A in the plane there is a scalar r  and 
 * a point P1 in L(P, Q) such that
 * 
 * A=rP1=r(tV+P) for some scalar t.Thus
 * 
 * A dotPro W = (rtV+rP) dotProd W = r(P dotProd W) = rd.
 * 
 * Asumming that L(P, Q) does not pass through the origen, 
 * 
 * So if r=1, the A is in L(P, Q), 
 * 
 * if r<1; A is "below" the line L(P, Q), rd<d and 
 * 
 * if r>1; A is "above" the line L(P, Q), rd>d
 * 
 * This divide the plane in two (three) regions.
 *
 * This criteria can be used to determine if a point is inside
 * of a poligonal convex region. A point P is inside the region
 * if and only if for every side S of the region, P and the centroid 
 * of the region are on the same side of the semi-plane desbribed by S.
 */
class Line
{
	public:
		Line(const Vect<2>& A, const Vect<2>& B)
		{
			updateLine(A, B);
		}

		Line(){};

		virtual ~Line()=default;

		void updateLine(const Vect<2>& A, const Vect<2>& B)
		{
			Vect<2> V=B-A;
			Vect<2> W=V.normalize()*(B.norm()*V.angCos(B));
			m_dir=B-W; // m_dir is actually perpendicular to the line L(B-A, Origin) and in L(A, B)
			m_d=Alg<2>::innerProduct(m_dir, B);
		}

		// README: since the return statement is a bool,
		// it seems the method return type should be bool too,
		// however, the way we are using this method (in Clickable<N>::update),
		// it is more convenient to cast bool into a int. 
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

//####################################################################

#endif
