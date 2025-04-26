/*********************************************************************
* Camera class                               								*
*         	                                                         *
* Version: 1.7                                                       *
* Date:    26-12-2022  (Reviewed 04/2025)                            *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef CAMERA_H
#define CAMERA_H

#include "reference_frame.h"
#include "rspace.h"
#include "utilities.h"

enum class CAM
{
	A,
	B,
	TOTAL,
};

//--------------------------------------------------------------------

class Camera : public ReferenceFrame
{
	public:
		Camera()=delete;
		Camera(const Camera&)=delete;

		virtual ~Camera()=default;

		void setApperture(double aperture);
		void setFocus(double focus);

		sf::Vector2f VectSFVect(const Vect<3>& V) const; //Vect<3> to sf::Vertex

		void lRuD();
		virtual void yawControl(int yawDirection);
		virtual void rollControl(int rollDirection);
		virtual void pitchControl(int pitchDirection);

		bool inRangeView(const Vect<3>& V);
		void updateVertex(sf::Vertex& V, const Vect<3>& W) const;
		void updateVertex(sf::Vector2f& V, const Vect<3>& W) const;
		double distanceToCamera(const Vect<3>& V) const;// V is in world coordinates
		double quasiDistanceToCamera(const Vect<3>& V) const;// V is in world coordinates

		void setWindow(sf::RenderWindow* window);
		void eventHandler(const sf::Event& event);
		void updateView();
	
	private:
		std::shared_ptr<RSpace<3>> m_RS;
		Rotation<3> m_yawRotation;
		Vect<3> m_pitchAxis;//y
		Vect<3> m_rollAxis;//x
		Vect<3> m_yawAxis;//z

		double m_app;
		double m_fD;
		double m_proportion;
		double m_threshold;

		double m_pitchAngle;
		double m_rollAngle;
		double m_yawAngle;

		int m_direction;
		bool m_hasChanged;
		bool m_ready;

		struct DIRECTION
		{
			enum Dir
			{
				LEFT=0,
				RIGHT,
				UP,
				DOWN,
				CLOCKWISE,
				ANTICLOCKWISE
			};
		};

		Camera(std::shared_ptr<RSpace<3>> RS, double fD, double aperture, double topCornerX, double topCornerY, double width, double height, double originX, double originY, double size=0.0);
		Camera(std::shared_ptr<RSpace<3>> RS, double fD, double aperture, double topCornerX, double topCornerY, double width, double height, double size=0.0);
		Camera(std::shared_ptr<RSpace<3>> RS, double fD, double aperture, double originX, double originY, double size=0.0);

		double ppWx(double d) const;
		double ppWy(double d) const;

		double normalizeAngle(double angle);

	friend class CameraManager;
};

//====================================================================
//====================================================================

class CameraManager
{
	public:
		template<CAM C, typename... Args>
		static void init(Args... args)
		{
			static bool initCams=false;
			if(!initCams){
				std::memset(m_CameraPtr, 0, sizeof(Camera*)*static_cast<int>(CAM::TOTAL));
			}

			static ABC<C> cam(args...);
			m_CameraPtr[static_cast<int>(C)]=cam.m_cam;
		}

		static void setCamera(CAM C)
		{
			s_cam=C;
		}

		static bool isCameraAvailable(CAM C)
		{
			return nullptr!=m_CameraPtr[static_cast<int>(C)];
		}

		static void setApperture(double aperture)
		{
			m_CameraPtr[static_cast<int>(s_cam)]->setApperture(aperture);
		}

		static void setFocus(double focus)
		{
			m_CameraPtr[static_cast<int>(s_cam)]->setFocus(focus);
		}

		static sf::Vector2f VectSFVect(const Vect<3>& V)//Vect<3> to sf::Vertex
		{
			return m_CameraPtr[static_cast<int>(s_cam)]->VectSFVect(V);
		}

		static void lRuD()
		{
			m_CameraPtr[static_cast<int>(s_cam)]->lRuD();
		}

		static void yawControl(int yawDirection)
		{
			m_CameraPtr[static_cast<int>(s_cam)]->yawControl(yawDirection);
		}

		static void rollControl(int rollDirection)
		{
			m_CameraPtr[static_cast<int>(s_cam)]->rollControl(rollDirection);
		}

		static void pitchControl(int pitchDirection)
		{
			m_CameraPtr[static_cast<int>(s_cam)]->pitchControl(pitchDirection);
		}

		static bool inRangeView(const Vect<3>& V)
		{
			return m_CameraPtr[static_cast<int>(s_cam)]->inRangeView(V);
		}

		static void updateVertex(sf::Vertex& V, const Vect<3>& W)
		{
			m_CameraPtr[static_cast<int>(s_cam)]->updateVertex(V, W);
		}

		static void updateVertex(sf::Vector2f& V, const Vect<3>& W)
		{
			m_CameraPtr[static_cast<int>(s_cam)]->updateVertex(V, W);
		}

		static double distanceToCamera(const Vect<3>& V)// V is in world coordinates
		{
			return m_CameraPtr[static_cast<int>(s_cam)]->distanceToCamera(V);
		}

		static double quasiDistanceToCamera(const Vect<3>& V)// V is in world coordinates
		{
			return m_CameraPtr[static_cast<int>(s_cam)]->quasiDistanceToCamera(V);
		}

		static void setWindow(sf::RenderWindow* window)
		{
			m_CameraPtr[static_cast<int>(s_cam)]->setWindow(window);
		}

		static void eventHandler(const sf::Event& event)
		{
			m_CameraPtr[static_cast<int>(s_cam)]->eventHandler(event);
		}

		static void updateView()
		{
			m_CameraPtr[static_cast<int>(s_cam)]->updateView();
		}


		/*static const double& rt()
		{
			return m_CameraPtr[static_cast<int>(s_cam)]->rt();
		}

		static double trans_x(double x)
		{
			return m_CameraPtr[static_cast<int>(s_cam)]->trans_x(x);
		}
		
		static double trans_y(double y)
		{
			return m_CameraPtr[static_cast<int>(s_cam)]->trans_y(y);
		}*/

		static void draw(const sf::Drawable& drawable, const sf::RenderStates& states=sf::RenderStates::Default)
		{
			m_CameraPtr[static_cast<int>(s_cam)]->draw(drawable, states);
		}
			
		static void draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states=sf::RenderStates::Default)
		{
			m_CameraPtr[static_cast<int>(s_cam)]->draw(vertices, vertexCount, type, states);
		}


	private:
		static Camera* m_CameraPtr[static_cast<int>(CAM::TOTAL)];

		static CAM s_cam;

		template<CAM C>
		struct ABC
		{
			template<typename... Args>
			ABC(Args... args)
			:m_cam(new Camera(args...))
			{
			}

			~ABC()
			{
				delete m_cam;
			}

			Camera* m_cam;
		};
};

//====================================================================
//====================================================================

//--------------------------------------------------------------------

inline Camera::Camera(std::shared_ptr<RSpace<3>> RS, double fD, double aperture, double topCornerX, double topCornerY, double width, double height, double size)
:
Camera(RS, fD, aperture, topCornerX, topCornerY, width, height, width*0.5, height*0.5, size)
{}

//--------------------------------------------------------------------

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

inline void Camera::setApperture(double aperture)
{
	m_app=(1.0/(2.0*aperture));
	m_threshold=m_fD/sqrt(m_fD*m_fD+aperture*aperture);
	m_proportion=m_app*(0.5*(c_width+c_height));
}

//----------------------------------------------------------------------

inline void Camera::setFocus(double focus)
{
	m_fD=focus;
	double aperture=1.0/(2.0*m_app);	
	m_threshold=m_fD/sqrt(m_fD*m_fD+aperture*aperture);
	m_proportion=m_app*(0.5*(c_width+c_height));
}

//----------------------------------------------------------------------

inline void Camera::setWindow(sf::RenderWindow* window)
{
	if(!m_window){
		m_window=window;
	}
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

inline double Camera::normalizeAngle(double angle)
{
	if(angle>=360.0){
		angle-=360.0;
	}
	else if(angle<=-360.0){
		angle+=360.0;
	}

	return angle;
}

//----------------------------------------------------------------------


#endif
