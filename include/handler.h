/*********************************************************************
* Handler class                                       					*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022  (Reviewed 04/2025)                            *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef HANDLER_H
#define HANDLER_H

#include "button.h"
#include "camera.h"

//----------------------------------------------------------------------

/*
* A Handler is a region on the puzzle where mouse inputs are
* tracked and if they are good, they trigger a respective rotation
* 
* In the case of the Dogic, a boundary will be formed by 3 vertices
* and each boundary will be able to induce rotation on 3 different 
* modules.
* 
* Notice that boundaries are fixed, they do not rotate...
* 
* */

//CM for num of corners for the handler, notice that the num of corner match 
// with the number of modules a hander can control.

template<int ButtonSides, int TotalButtons>
class GroupReactiveButtons;

template<int CM>
class Handler : public Clickable<CM>
{	
	public:
		Instruction getInstruction(const sf::Vector2i& V, const sf::Vector2i& W) const;
	
		double weight(const Vect<3>* puzzleCorners) const;
		void update(const Vect<3>* puzzleCorners);

	protected:
		Vect<2> m_vect2[CM];
		int m_moduleIndexes[CM];// the modules a boundary have control on
		int m_corners[CM];		// the corners of the boundary, in this case they correspond to the vertex of a tetra-cell
		int m_orientation[CM];//={-1,-1,-1};// the sense to

		std::pair<int, double> getClosetModule(const Vect<2>& V, bool orientation) const;
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
		CameraManager::VectSFVect(puzzleCorners[m_corners[i]]);
		m_vect2[i]=Normalization::absoluteVect(sfV);
	}

	Clickable<CM>::update(m_vect2);
}

//----------------------------------------------------------------------

template<int CM>
std::pair<int, double> Handler<CM>::getClosetModule(const Vect<2>& V, bool orientation) const
{
	int idx=-1;
	double cos=0;
	double rCos=0;
	double max=0;
	double d=0;
	for(int i=0; i<CM; i++){
		Vect<2> W=m_vect2[(i+1)%CM]-m_vect2[i];
		cos=angCos(V, W);
		if(orientation){
			d=std::abs(m_orientation[i]*cos);
		}
		else{
			d=std::abs(cos);
		}
		if(d>max){
			max=d;
			idx=i;
			rCos=cos;
		}
	}

	return {idx, rCos};
}

//----------------------------------------------------------------------

template<int CM>
Instruction Handler<CM>::getInstruction(const sf::Vector2i& V, const sf::Vector2i& W) const
{
	if(V==W){
		return {-1, 0, 0};
	}

	Vect<2> U{1.0*(W.x-V.x), 1.0*(W.y-V.y)};

	int idx;
	double cos;
	std::tie(idx, cos)=getClosetModule(U, CM%2==0);

	double x=m_orientation[idx]*cos;
	int orientation=x>0? 1 : -1;

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

	return {m_moduleIndexes[idx], orientation, isShort};
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

	return CameraManager::quasiDistanceToCamera(centroid);
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

//====================================================================

#endif
