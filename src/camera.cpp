#include "camera.h"

#include <cmath>

//----------------------------------------------------------------------

Camera* CameraManager::m_CameraPtr[static_cast<int>(CAM::TOTAL)];

CAM CameraManager::s_cam=CAM::A;

//----------------------------------------------------------------------

Camera::Camera(std::shared_ptr<RSpace<3>> RS, double fD, double aperture, double topCornerX, double topCornerY, double width, double height, double originX, double originY, double size)
: ReferenceFrame(topCornerX, topCornerY, width, height, originX, originY, size)
, m_RS(RS)
, m_yawRotation(Vect<3>(2), DELTA_ANGLE)
, m_pitchAxis(1) //y
, m_rollAxis(0)  //inital
, m_yawAxis(2)   //z
, m_app(1.0/(2.0*aperture))
, m_fD(fD)
, m_proportion(m_app*(0.5*(width+height)))
, m_threshold(m_fD/sqrt(m_fD*m_fD+aperture*aperture))
, m_pitchAngle(90.0)
, m_rollAngle(-90.0)
, m_yawAngle(0)
, m_hasChanged(false)
, m_ready(false)
{
	m_RS->updateTransformation(0, m_yawAxis, m_yawAngle);
	m_RS->updateTransformation(1, m_pitchAxis, m_pitchAngle);		
	m_RS->updateTransformation(2, m_rollAxis, m_rollAngle);
	
	for(int i=0; i<10; i++){
		m_direction=DIRECTION::LEFT;
		lRuD();
	}

	for(int i=0; i<10; i++){
		m_direction=DIRECTION::UP;
		lRuD();
	}
}

//----------------------------------------------------------------------

void Camera::yawControl(int yawDirection)
{
	double a=1.0;
	if(m_direction<2){// left-right
		if(m_direction==DIRECTION::RIGHT){
			a=-1.0;
		}

		if(a>0.0){
			m_yawRotation.rotate(m_pitchAxis);
			m_yawRotation.rotate(m_rollAxis);
		}
		else{
			m_yawRotation.revRotate(m_pitchAxis);
			m_yawRotation.revRotate(m_rollAxis);
		}

		m_yawAngle+=a*DELTA_ANGLE;
		m_yawAngle=normalizeAngle(m_yawAngle);

		m_RS->updateTransformation(0, m_yawAxis, m_yawAngle);
		m_RS->updateTransformation(1, m_pitchAxis, m_pitchAngle);		
		m_RS->updateTransformation(2, m_rollAxis, m_rollAngle);
	}
}

//----------------------------------------------------------------------

void Camera::pitchControl(int pitchDirection)
{
	double a=1.0;
	if(m_direction==DIRECTION::DOWN){
		a=-1.0;
	}

	Rotation<3>(m_pitchAxis, a*DELTA_ANGLE).rotate(m_rollAxis);

	m_pitchAngle+=a*DELTA_ANGLE;
	m_pitchAngle=normalizeAngle(m_pitchAngle);

	m_RS->updateTransformation(1, m_pitchAxis, m_pitchAngle);
	m_RS->updateTransformation(2, m_rollAxis, m_rollAngle);
}

//----------------------------------------------------------------------

void Camera::rollControl(int rollDirection)
{
	double a=1.0;
	if(m_direction<2){// left-right
		if(m_direction==DIRECTION::CLOCKWISE){
			a=-1.0;
		}

		m_rollAngle+=a*DELTA_ANGLE;
		m_rollAngle=normalizeAngle(m_rollAngle);
		
		m_RS->updateTransformation(2, m_rollAxis, m_rollAngle);
	}
}

//----------------------------------------------------------------------

void Camera::lRuD()
{
	// i=0 arrow Left, i=1 arrow Right 
	// i=2 arrow Up, i=3 arrow Down 

	double a=1.0;
	Vect<3> V=m_RS->getRelPosition();
	if(m_direction<2){// left-right
		
		yawControl(m_direction);
		
		if(m_direction==DIRECTION::RIGHT){
			a=-1.0;
		}
		Rotation<3> Rt(m_yawAxis, a*DELTA_ANGLE);
		Rt.rotate(V);
		m_RS->setRelPosition(V);

	}
	else{
		if(m_direction==DIRECTION::DOWN){
			a=-1.0;
		}
		pitchControl(m_direction);

		Rotation<3> Rtz(m_pitchAxis, a*DELTA_ANGLE);
		Rtz.rotate(V);
		m_RS->setRelPosition(V);
	}
}

//----------------------------------------------------------------------

void Camera::eventHandler(const sf::Event& event)
{
	if(event.key.code>70 && event.key.code<75){
		if(event.type == sf::Event::KeyPressed){
			m_direction=event.key.code-71;
			m_ready=true;
		}
		else if(event.type == sf::Event::KeyReleased){
			m_ready=false;
		}
	}
}

//----------------------------------------------------------------------

void Camera::updateView()
{
	if(m_ready){
		lRuD();
	}
}

//----------------------------------------------------------------------
