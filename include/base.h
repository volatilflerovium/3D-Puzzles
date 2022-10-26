/*********************************************************************
* Base class                               								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef BASE_H
#define BASE_H

#include <SFML/Graphics.hpp>
#include "constants.h"
#include "vector.h"

//----------------------------------------------------------------------

class Base
{
	public:
		static sf::RenderWindow* m_window;
		static void setWindow(sf::RenderWindow* window)
		{
			m_window=window;
		}
};

//======================================================================
//######################################################################
//======================================================================

class Normalization : public Base
{
	public:
		static sf::Vector2f absolute(double x, double y);
		static Vect<2> absoluteVect(const sf::Vector2f& v);
		static sf::Vector2i positionOnView();
		static sf::Vector2i getMousePosition();
};

//----------------------------------------------------------------------

inline sf::Vector2f Normalization::absolute(double x, double y)
{
	return sf::Vector2f(x, D_WINDOW_HEIGHT-y);
}

//----------------------------------------------------------------------

inline Vect<2> Normalization::absoluteVect(const sf::Vector2f& v)
{
	return {v.x, D_WINDOW_HEIGHT-v.y};
}

//----------------------------------------------------------------------

inline sf::Vector2i Normalization::positionOnView()
{
	sf::Vector2i tmp=sf::Mouse::getPosition(*m_window);
	tmp.y=D_WINDOW_HEIGHT-tmp.y;
	return tmp;
}

//----------------------------------------------------------------------

inline sf::Vector2i Normalization::getMousePosition()
{
	return positionOnView();
}

//----------------------------------------------------------------------

#endif
