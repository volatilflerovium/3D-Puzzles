/*********************************************************************
* Base class                               								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022  (Reviewed 04/2025)                            *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef BASE_H
#define BASE_H

#include "constants.h"
#include "vector.h"

#include <SFML/Graphics.hpp>

//--------------------------------------------------------------------

class Base
{
	public:
		static void setWindow(sf::RenderWindow* window)
		{
			m_window=window;
		}

		static sf::RenderWindow* m_window;
};

//====================================================================

class Normalization
{
	public:
		static sf::Vector2f absolute(double x, double y);
		static Vect<2> absoluteVect(const sf::Vector2f& v);
		static sf::Vector2i positionOnView();
		static sf::Vector2i getMousePosition();
};

//--------------------------------------------------------------------

inline sf::Vector2f Normalization::absolute(double x, double y)
{
	return sf::Vector2f(x, D_WINDOW_HEIGHT-y);
}

//--------------------------------------------------------------------

inline Vect<2> Normalization::absoluteVect(const sf::Vector2f& v)
{
	return {v.x, D_WINDOW_HEIGHT-v.y};
}

//--------------------------------------------------------------------

inline sf::Vector2i Normalization::positionOnView()
{
	sf::Vector2i tmp=sf::Mouse::getPosition(*(Base::m_window));
	tmp.y=D_WINDOW_HEIGHT-tmp.y;
	return tmp;
}

//--------------------------------------------------------------------

inline sf::Vector2i Normalization::getMousePosition()
{
	return positionOnView();
}

//====================================================================

#endif
