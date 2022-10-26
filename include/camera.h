/*********************************************************************
* Camera class                               								*
*         	                                                         *
* Version: 1.5                                                       *
* Date:    16-09-2022                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>

#include "constants.h"
#include "base.h"
#include "reference_frame.h"
#include "vector.h"
#include "rspace.h"

//----------------------------------------------------------------------

class Camera : public ReferenceFrame
{
	public:
		Camera(std::shared_ptr<RSpace<3>> RS, double fD, double aperture, double topCornerX, double topCornerY, double width, double height, double originX, double originY, double size=0.0);
		Camera(std::shared_ptr<RSpace<3>> RS, double fD, double aperture, double topCornerX, double topCornerY, double width, double height, double size=0.0);
		Camera(std::shared_ptr<RSpace<3>> RS, double fD, double aperture, double originX, double originY, double size=0.0);		

		Camera& setApperture(double aperture);
		Camera& setFocus(double focus);

		void updateAngle(int i, double ang);

		sf::Vector2f VectSFVect(const Vect<3>& V) const;
		void lRuD(int k);
		bool inRangeView(const Vect<3>& V);
		void updateVertex(sf::Vertex& V, const Vect<3>& W) const;
		void updateVertex(sf::Vector2f& V, const Vect<3>& W) const;
		double distanceToCamera(const Vect<3>& V) const;// V is in world coordinates
		double quasiDistanceToCamera(const Vect<3>& V) const;// V is in world coordinates

		void setWindow(sf::RenderWindow* window);

		static Camera* m_Cameras[2];

	private:
		std::shared_ptr<RSpace<3>> m_RS;
		Vect<3> m_Axz;
		Vect<3> m_Ax;
		Rotation<3> m_cameraAngles[2];
		double m_app;
		double m_fD;
		double m_proportion;
		double m_threshold;
		double m_yAxeAngle;
		double m_xAxeAngle;
		bool m_hasChanged;

		struct DIRECTION
		{
			enum Dir
			{
				LEFT=0,
				RIGHT,
				UP,
				DOWN
			};
		};

		double ppWx(double d) const;
		double ppWy(double d) const;
};

//----------------------------------------------------------------------

inline Camera::Camera(std::shared_ptr<RSpace<3>> RS, double fD, double aperture, double topCornerX, double topCornerY, double width, double height, double size)
:
Camera(RS, fD, aperture, topCornerX, topCornerY, width, height, width*0.5, height*0.5, size)
{}

//----------------------------------------------------------------------

inline Camera::Camera(std::shared_ptr<RSpace<3>> RS, double fD, double aperture, double originX, double originY, double size)
:
Camera(RS, fD, aperture, 0.0, 0.0, D_WINDOW_WIDTH, D_WINDOW_HEIGHT, originX, originY, size)
{}

//----------------------------------------------------------------------

inline void Camera::updateVertex(sf::Vertex& V, const Vect<3>& W) const
{
	updateVertex(V.position, W);
}

//----------------------------------------------------------------------

inline sf::Vector2f Camera::VectSFVect(const Vect<3>& V) const
{
	Vect<3> Cv=m_RS->world2Local(V);
	double t=m_fD/Cv[2];
	return {static_cast<float>(trans_x(ppWx(Cv[0]*t))), static_cast<float>(trans_y(ppWx(Cv[1]*t)))};
}

//----------------------------------------------------------------------

inline void Camera::updateVertex(sf::Vector2f& V, const Vect<3>& W) const
{	
	Vect<3> Cv=m_RS->world2Local(W);
	double t=m_fD/Cv[2];
	V.x=static_cast<float>(trans_x(ppWx(Cv[0]*t)));
	V.y=static_cast<float>(trans_y(ppWy(Cv[1]*t)));
}

//----------------------------------------------------------------------

inline Camera& Camera::setApperture(double aperture)
{
	m_app=(1.0/(2.0*aperture));
	m_threshold=m_fD/sqrt(m_fD*m_fD+aperture*aperture);
	m_proportion=m_app*(0.5*(c_width+c_height));
	return *this;
}

//----------------------------------------------------------------------

inline Camera& Camera::setFocus(double focus)
{
	m_fD=focus;
	double aperture=1.0/(2.0*m_app);	
	m_threshold=m_fD/sqrt(m_fD*m_fD+aperture*aperture);
	m_proportion=m_app*(0.5*(c_width+c_height));
	return *this;
}

//----------------------------------------------------------------------

inline void Camera::setWindow(sf::RenderWindow* window)
{
	if(!m_window){
		m_window=window;
	}
}

//----------------------------------------------------------------------

inline void Camera::updateAngle(int i, double ang)
{
	m_RS->updateTransformations(i, ang);
}

//----------------------------------------------------------------------

inline double Camera::ppWx(double d) const 
{
	return d*m_proportion;
}

//----------------------------------------------------------------------

inline double Camera::ppWy(double d) const 
{
	return d*m_proportion;
}

//----------------------------------------------------------------------

inline bool Camera::inRangeView(const Vect<3>& V)
{	
	return fast_cos(m_RS->world2Local(V))>m_threshold;
}

//----------------------------------------------------------------------

inline double Camera::distanceToCamera(const Vect<3>& V) const
{
	return (m_RS->world2Local(V)).norm();
}

//----------------------------------------------------------------------

inline double Camera::quasiDistanceToCamera(const Vect<3>& V) const
{
	return (m_RS->world2Local(V)).quasiNorm();
}

//----------------------------------------------------------------------

#endif
