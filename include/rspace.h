/*********************************************************************
* rspace class                               								*
*                                                                    *
* Version: 1.5                                                       *
* Date:    24-07-2021                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef RSPACE_H
#define RSPACE_H
#include <memory>
#include "vector.h"

//----------------------------------------------------------------------

template<int D>
class RSpace
{
	public:
		~RSpace();
		Vect<D> getRelPosition() const;
		Vect<D> local2World(const Vect<D>& VTX);
		Vect<D> world2Local(const Vect<D>& VTX);

		static std::shared_ptr<RSpace<D>> maker(Vect<D> initialPosition, std::initializer_list<double> angles);
		std::shared_ptr<RSpace<D>> spawn(Vect<D> initialPosition, std::initializer_list<double> angles);

		void updateTransformations(int i, double ang);
		void updateTransformation(int i, const Vect<3>& axe, double angle);
		void move(const Vect<D>& V);// set relPosition+=V
		void setRelPosition(const Vect<D>& V);

	private:
		Vect<D> m_relPosition; // the position of the origin of the space relatively to the reference space (parent)
		Rotation<D>* m_Transf[D]; // rotations always at (0,0,0)
      
      RSpace* m_RF=NULL;
      double m_angles[D];

		void setRef(RSpace<D>* _RF);		
		RSpace() = delete;
		RSpace(Vect<D> initialPosition, std::initializer_list<double> _angles);
};

//-------------------------------------------------------------------

template<int D>
inline RSpace<D>::RSpace(Vect<D> initialRelPosition, std::initializer_list<double> angles)
:m_relPosition(initialRelPosition)
{
	int i=0;
	for(double ang : angles){
		m_angles[i]=ang;
		m_Transf[i]=new Rotation<D>(Vect<D>(i), ang);
		i++;
	}
}

//-------------------------------------------------------------------

template<int D>
RSpace<D>::~RSpace()
{
	for(int i=0; i<D; i++){
		delete m_Transf[i];
	}
}

//-------------------------------------------------------------------

template<int D>
inline Vect<D> RSpace<D>::getRelPosition() const
{
	return m_relPosition;
}

//-------------------------------------------------------------------

template<int D>
inline std::shared_ptr<RSpace<D>> RSpace<D>::maker(Vect<D> initialPosition, std::initializer_list<double> angles)
{
	return std::shared_ptr<RSpace<D>>(new RSpace<D>(initialPosition, angles));
}

//-------------------------------------------------------------------

template<int D>
inline std::shared_ptr<RSpace<D>> RSpace<D>::spawn(Vect<D> initialPosition, std::initializer_list<double> angles)
{
	std::shared_ptr<RSpace<D>> tmp(new RSpace<D>(initialPosition, angles));
	tmp->setRef(this);
	return tmp;
}

//-------------------------------------------------------------------

template<int D>
inline void RSpace<D>::setRef(RSpace<D>* _RF)
{
	if(!m_RF){
		m_RF=_RF;
		return;
	}
	throw "Reference Frame already set";
}

//-------------------------------------------------------------------

template<int D>
inline void RSpace<D>::updateTransformations(int i, double ang)
{
	m_angles[i]+=ang;	
	//~m_Transf[i]();
	new(m_Transf[i]) Rotation<D>(Vect<D>(i), m_angles[i]);
}

//-------------------------------------------------------------------

template<int D>
inline void RSpace<D>::updateTransformation(int i, const Vect<3>& axe, double angle)
{
	new(m_Transf[i]) Rotation<D>(axe, angle);
}

//-------------------------------------------------------------------

template<int D>
Vect<D> RSpace<D>::local2World(const Vect<D>& VRX)
{
	Vect<D> V(VRX);
	for(int i=0; i<D; i++){
		m_Transf[i]->rotate(V);
	}

	V+=m_relPosition;

	if(m_RF){
		return m_RF->local2World(V);
	}

	return V;
}

//-------------------------------------------------------------------

template<int D>
Vect<D> RSpace<D>::world2Local(const Vect<D>& VRX)
{
	Vect<D> V(VRX);
	V-=m_relPosition;
	//transformations in reverse order!
	for(int i=D-1; i>-1; i--){
		m_Transf[i]->revRotate(V);
	}

	if(m_RF){
		return m_RF->world2Local(V);
	}
	return V;
}

//-------------------------------------------------------------------

template<int D>
inline void RSpace<D>::move(const Vect<D>& V)
{
	m_relPosition+=V;
}

//-------------------------------------------------------------------

template<int D>
inline void RSpace<D>::setRelPosition(const Vect<D>& V)
{
	m_relPosition=V;
}

//-------------------------------------------------------------------

#endif
