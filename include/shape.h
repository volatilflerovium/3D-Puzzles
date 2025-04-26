/*********************************************************************
* Shape class                                 								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022  (Reviewed 04/2025)                            *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef SHAPE_H
#define SHAPE_H
#include "camera.h"
#include "vector.h"

//----------------------------------------------------------------------

class Shape
{
	public:
		Shape(std::shared_ptr<RSpace<3>> _RS)
		: m_centroid({0, 0, 0})
		, m_RS(_RS)
		{}

		virtual ~Shape()=default;

		virtual void draw()=0;
		virtual void updateEdges()=0;
		virtual void rotate(const Rotation<3>& Rt)=0;
		virtual void revRotate(const Rotation<3>& Rt)=0;
		virtual void setLocalPosition(const Vect<3>& V)=0;
		virtual void setGlobalPosition(const Vect<3>& V)=0;

		std::shared_ptr<RSpace<3>> getLocalSpace();
		Vect<3> getCentroidW() const; // return the centroid in world coordinates
		bool display() const;
		void setDisplay(bool view);

	protected:
		Vect<3> m_centroid;
		std::shared_ptr<RSpace<3>> m_RS;

		Vect<3>* m_Vertex{nullptr};
		sf::Vertex** m_Edges{nullptr};

		bool m_hasChanged{true};
		bool m_display{true};

		void updateDisplay();
};

//----------------------------------------------------------------------

inline bool Shape::display() const
{
	return m_display;
}

//----------------------------------------------------------------------

inline void Shape::setDisplay(bool display)
{
	m_display=display;
}

//----------------------------------------------------------------------

inline void Shape::updateDisplay()
{
	m_display=CameraManager::inRangeView(m_centroid);
}

//----------------------------------------------------------------------

inline std::shared_ptr<RSpace<3>> Shape::getLocalSpace()
{
	return m_RS;
}

//----------------------------------------------------------------------

inline Vect<3> Shape::getCentroidW() const
{
	return m_RS->local2World(m_centroid);
}

//----------------------------------------------------------------------


#endif
