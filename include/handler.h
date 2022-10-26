/*********************************************************************
* Handler class                                       					*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef HANDLER_H
#define HANDLER_H

#include "button.h"
#include "camera.h"
#include "vector.h"
#include "utilities.h"

//----------------------------------------------------------------------

template<int ButtonSides, int TotalButtons>
class GroupReactiveButtons;

template<int CM>
class Handler : public Clickable<CM>
{	
	public:	
		Instruction getModuleDirection(const sf::Vector2i& V, const sf::Vector2i& W) const;
	
		double weight(const Vect<3>* puzzleCorners) const;
		void update(const Vect<3>* puzzleCorners);

	protected:
		Vect<2> m_vect2[CM];
		int m_moduleIndexes[CM];
		int m_corners[CM];
		int m_orientation[CM];

		int getClosetModule(const sf::Vector2i& V, const sf::Vector2i& W) const;
		int getClosetModule(const Vect<2>& V) const;
		void loader(int* dataHolder, std::initializer_list<int> dataList);
		
	template<int ButtonSides, int TotalButtons>
	friend class GroupReactiveButtons;
};

//----------------------------------------------------------------------

template<int CM>
void Handler<CM>::update(const Vect<3>* puzzleCorners)
{
	for(int i=0; i<CM; i++){
		sf::Vector2f sfV=
		Camera::m_Cameras[0]->VectSFVect(puzzleCorners[m_corners[i]]);
		m_vect2[i]=Normalization::absoluteVect(sfV);
	}

	Clickable<CM>::update(m_vect2);
}

//----------------------------------------------------------------------

template<int CM>
int Handler<CM>::getClosetModule(const sf::Vector2i& V, const sf::Vector2i& W) const
{
	if(CM%2==0){
		return -1;
	}

	int idx=-1;
	double d=0.0;
	double ang=0;
	sf::Vector2i a=W-V;
	Vect<2> B{1.0*a.x, 1.0*a.y};
	for(int i=0; i<CM; i++){
		Vect<2> A=m_vect2[(i+1)%CM]-m_vect2[i];
		d=angCos(A, B);
		if(d<0){
			d=-1.0*d;
		}
		if(d>ang){
			ang=d;
			idx=i;
		}
	}

	return idx;
}

//----------------------------------------------------------------------

template<int CM>
int Handler<CM>::getClosetModule(const Vect<2>& V) const
{
	int idx=-1;
	double minDis=1e6;
	double d=0;
	for(int i=0; i<CM; i++){
		Vect<2> W=0.5*(m_vect2[(i+1)%CM]+m_vect2[i]);
		d=Alg<2>::distance(V, W);
		if(d<minDis){
			minDis=d;
			idx=i;
		}
	}
	return idx;
}

//----------------------------------------------------------------------

template<int CM>
Instruction Handler<CM>::getModuleDirection(const sf::Vector2i& V, const sf::Vector2i& W) const
{
	if(V==W){
		return {-1, 0, 0};
	}

	sf::Vector2i a=W-V;
	Vect<2> U{1.0*a.x, 1.0*a.y};
	
	Vect<2> A{0.5*(V.x+W.x), 0.5*(V.y+W.y)};// (V+W)/2

	int idx=getClosetModule(V, W);
	if(idx<0){
		idx=getClosetModule(A);
	}

	int b=m_orientation[idx];
	
	if(angCos((m_vect2[(idx+1)%CM]-m_vect2[idx]), U)<0){
		b*=-1;
	}

	bool isShort=false;
	if(CM%2!=0){
		int j=(idx+2)%CM;

		Vect<2> a{1.0*(W.x+V.x), 1.0*(W.y+V.y)};
		a*=0.5;

		double d=Alg<2>::distance(m_vect2[j], a);
		Vect<2> tmp=getCentroid(m_vect2[(j+1)%CM], m_vect2[(j+2)%CM]);
		if(d<Alg<2>::distance(tmp, m_vect2[j])*0.5){
			isShort=true;
		}
	}

	return {m_moduleIndexes[idx], b, isShort};
}

//----------------------------------------------------------------------

template<int CM>
double Handler<CM>::weight(const Vect<3>* puzzleCorners) const
{
	Vect<3> centroid{0, 0, 0};
	for(int i=0; i<CM; i++){
		centroid+=puzzleCorners[m_corners[i]];
	}
	centroid*=(1.0/(CM*1.0));

	return Camera::m_Cameras[0]->quasiDistanceToCamera(centroid);
}

//----------------------------------------------------------------------

template<int CM>
void Handler<CM>::loader(int* dataHolder, std::initializer_list<int > dataList)
{
	if(dataList.size()<CM){
		throw "Data list size is too low";
	}
	int i=0;
	for(int idx : dataList){
		dataHolder[i++]=idx;
		if(i==CM){
			break;
		}
	}
}

#endif
