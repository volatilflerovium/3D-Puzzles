#include <cmath>
#include "../include/camera.h"
#include "../include/shape.h"

//----------------------------------------------------------------------

Camera* Camera::m_Cameras[2]={nullptr, nullptr};

Camera::Camera(std::shared_ptr<RSpace<3>> RS, double fD, double aperture, double topCornerX, double topCornerY, double width, double height, double originX, double originY, double size)
:
ReferenceFrame(topCornerX, topCornerY, width, height, originX, originY, size),
m_RS(RS), 
m_Axz(crossProduct({-1.0*(m_RS->getRelPosition()[0]), 0, -1.0*(m_RS->getRelPosition()[2])}, Vect<3>(1))),	
m_cameraAngles{Rotation<3>(m_Ax, DELTA_ANGLE), Rotation<3>({0,0,1}, DELTA_ANGLE) },
m_app(1.0/(2.0*aperture)),
m_fD(fD),
m_proportion(m_app*(0.5*(width+height))),
m_threshold(m_fD/sqrt(m_fD*m_fD+aperture*aperture)),
m_yAxeAngle(0),
m_xAxeAngle(0),
m_hasChanged(false)
{
	m_RS->updateTransformation(0, {1,0,0}, 0.0);
	m_RS->updateTransformation(1, {1,0,0}, 0.0);
	m_RS->updateTransformation(2, {1,0,0}, 0.0);
	
	for(int i=0; i<9; i++){
		lRuD(DIRECTION::LEFT);	
	}
	for(int i=0; i<7; i++){
		lRuD(DIRECTION::UP);
	}

	if(!m_Cameras[0]){
		Shape::setCamera(this);
	}

	for(int i=0; i<2; i++){
		if(!m_Cameras[i]){
			m_Cameras[i]=this;
			break;
		}
	}
}

//----------------------------------------------------------------------

void Camera::lRuD(int k)
{
	int direction=k;//-71;

	// i=0 arrow Left, i=1 arrow Right 
	// i=2 arrow Up, i=3 arrow Down 

	// * We need to rotate the initial position V through
	// * the geodesic describe by R and V

	double a=1.0;
	Vect<3> V=m_RS->getRelPosition();
	if(direction<2){
		if(direction==DIRECTION::RIGHT){
			a=-1.0;
		}

		m_xAxeAngle+=a*DELTA_ANGLE;
		Rotation<3> Rty(Vect<3>(1), a*DELTA_ANGLE);
		Rty.rotate(V);

		V-=m_RS->getRelPosition();
		m_RS->move(V);
		V=m_RS->getRelPosition();
		
		m_RS->updateTransformation(0, Vect<3>(1), 90+m_xAxeAngle);
		
		if(std::abs(m_yAxeAngle)==90){
			Rty.rotate(m_Axz);
		}
		else{
			if(std::abs(m_yAxeAngle)>90 && std::abs(m_yAxeAngle)<270){
				m_Axz=crossProduct({V[0], 0, V[2]}, Vect<3>(1));
			}
			else{
				m_Axz=crossProduct({-1.0*V[0], 0, -1.0*V[2]}, Vect<3>(1));
			}
		}

		m_RS->updateTransformation(1, m_Axz, m_yAxeAngle);

		if(m_xAxeAngle>=360.0){
			m_xAxeAngle-=360.0;
		}
		else if(m_xAxeAngle<=-360.0){
			m_xAxeAngle+=360.0;
		}
	}
	else{
		if(direction==DIRECTION::DOWN){
			a=-1.0;
		}

		m_yAxeAngle+=a*DELTA_ANGLE;
		Rotation<3> Rtx(m_Axz, a*DELTA_ANGLE);
		Rtx.rotate(V);

		V-=m_RS->getRelPosition();
		m_RS->move(V);

		m_RS->updateTransformation(1, m_Axz, m_yAxeAngle);

		if(m_yAxeAngle>=360.0){
			m_yAxeAngle-=360.0;
		}
		else if(m_yAxeAngle<=-360.0){
			m_yAxeAngle+=360.0;
		}
	}
}

//----------------------------------------------------------------------
