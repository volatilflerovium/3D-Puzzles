/*********************************************************************
* ReferenceFrame class                         								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef REF_FRAME_H
#define REF_FRAME_H

#include <SFML/Graphics.hpp>
#include "constants.h"
#include "vector.h"
#include "base.h"

class Camera;

//----------------------------------------------------------------------

class ReferenceFrame : public Base
{
   public:
		ReferenceFrame(double originX, double originY, double size=0.0);// size in cm
		ReferenceFrame(double offsetX, double offsetY, double width, double height, double size=0.0);
		ReferenceFrame(double offsetX, double offsetY, double width, double height, double originX, double originY, double size=0.0);

		const double& rt() const;
		double trans_x(double x) const;
		double trans_y(double y) const;

		//void eventHandler(const sf::Event& event);

		void draw(const sf::Drawable& drawable, const sf::RenderStates& states=sf::RenderStates::Default);
		void draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states=sf::RenderStates::Default);

	protected:
		double m_ration{1.0};
		const double c_px_per_cm{PX_CM};//37.795238;
		const double c_offsetX;
		const double c_offsetY;
		const double c_width;
		const double c_height;
		const double c_originX;
		const double c_originY;

   	ReferenceFrame() = delete;
		ReferenceFrame& operator=(const ReferenceFrame& RF) = delete;
};

//-------------------------------------------------------------------

inline ReferenceFrame::ReferenceFrame(double offsetX, double offsetY, double width, double height, double originX, double originY, double size)
:
c_offsetX(offsetX),
c_offsetY(offsetY),
c_width(width),
c_height(height),
c_originX(originX),
c_originY(originY)
{
	if(size>0){
      m_ration=c_px_per_cm/(1.0*size);
   }
}

//-------------------------------------------------------------------

inline ReferenceFrame::ReferenceFrame(double originX, double originY, double size) // size is metres to scale down to 1 cm
:
ReferenceFrame(0.0, 0.0, D_WINDOW_WIDTH, D_WINDOW_HEIGHT, originX, originY, size)
{}

//-------------------------------------------------------------------

inline ReferenceFrame::ReferenceFrame(double offsetX, double offsetY, double width, double height, double size)
:
ReferenceFrame(offsetX, offsetY, width, height, width*0.5, height*0.5, size)
{}

//-------------------------------------------------------------------

inline const double& ReferenceFrame::rt() const
{
   return m_ration;
}

//-------------------------------------------------------------------

inline double ReferenceFrame::trans_x(double x) const
{
	return c_offsetX+c_originX+x;
}

//-------------------------------------------------------------------

inline double ReferenceFrame::trans_y(double y) const
{
	return c_offsetY+c_originY-y;
}

//-------------------------------------------------------------------

inline void ReferenceFrame::draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
	m_window->draw(drawable, states);
} 

//----------------------------------------------------------------------

inline void ReferenceFrame::draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states)
{
	m_window->draw(vertices, vertexCount, type, states);
}

//-------------------------------------------------------------------

#endif
