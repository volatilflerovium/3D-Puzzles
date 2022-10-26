#include "../include/tetra.h"
#include "../include/cell.h"
#include "../include/action.h"

//======================================================================

Tetra::Tetra(std::shared_ptr<RSpace<3>> _RS, const Vect<3>& fv0, const Vect<3>& fv1, const Vect<3>& fv2, double vl, int id)
:LogicCell(_RS, id)
{
	m_Vertex[0]=(fv0+fv1+fv2)/3.0;
	double d=m_Vertex[0].norm()-vl;

	m_Vertex[0].normalize();
	m_Vertex[0]*=d;
	m_Vertex[1]=fv0;
	m_Vertex[2]=fv1;
	m_Vertex[3]=fv2;
	setFace(0, 1, 2, 3);
	setFace(1, 1, 0, 2);
	setFace(2, 0, 2, 3);
	setFace(3, 1, 0, 3);

	for(int i=0; i<TetraSettings::FACE_NUM; i++){
		m_faces[i].setPointCount(3);
	}

	setCentroid();
	updateEdges();
};	

//----------------------------------------------------------------------

void Tetra::updateEdges()
{
	if(m_hasChanged || true){
		int j, k;
		for(int i=0; i<3; i++){
			j=(i+1)%3;
			k=2*i;
			m_Cm->updateVertex(m_Edges[k][0], m_RS->local2World(m_Vertex[3]));
			m_Cm->updateVertex(m_Edges[k][1], m_RS->local2World(m_Vertex[i]));

			m_Cm->updateVertex(m_Edges[k+1][0], m_RS->local2World(m_Vertex[i]));
			m_Cm->updateVertex(m_Edges[k+1][1], m_RS->local2World(m_Vertex[j]));
		}

		for(int i=0; i<TetraSettings::VERTEX_NUM; i++){
			m_Cm->updateVertex(m_verteces[i], m_RS->local2World(m_Vertex[i]));
		}

		for(int i=0; i<TetraSettings::FACE_NUM; i++){		
			for(int j=0; j<TetraSettings::VERTEX_IN_FACE; j++){		
				m_faces[i].setPoint(j, m_verteces[m_facesOrder[i][j]]);
			}
		}

		m_hasChanged=false;
	}
}

//======================================================================
//######################################################################
//======================================================================

Tetra12::Tetra12(std::shared_ptr<RSpace<3>> _RS, const Vect<3>& fv0, const Vect<3>& fv1, const Vect<3>& fv2, double vl, int id)
:LogicCell(_RS, id)
{
	m_Vertex[0]=(fv0+fv1+fv2)/3.0;
	double d=m_Vertex[0].norm()-vl;

	m_Vertex[0].normalize();
	m_Vertex[0]*=d;
	m_Vertex[1]=fv0;
	m_Vertex[2]=fv1;
	m_Vertex[3]=fv2;
	setFace(0, 1, 2, 3);
	setFace(1, 1, 0, 2);
	setFace(2, 0, 2, 3);
	setFace(3, 1, 0, 3);

	for(int i=0; i<Tetra12Settings::FACE_NUM; i++){
		m_faces[i].setPointCount(3);
	}

	setCentroid();
	updateEdges();
};	

//----------------------------------------------------------------------

void Tetra12::updateEdges()
{
	if(m_hasChanged || true){
		int j, k;
		for(int i=0; i<3; i++){
			j=(i+1)%3;
			k=2*i;
			m_Cm->updateVertex(m_Edges[k][0], m_RS->local2World(m_Vertex[3]));
			m_Cm->updateVertex(m_Edges[k][1], m_RS->local2World(m_Vertex[i]));

			m_Cm->updateVertex(m_Edges[k+1][0], m_RS->local2World(m_Vertex[i]));
			m_Cm->updateVertex(m_Edges[k+1][1], m_RS->local2World(m_Vertex[j]));
		}

		for(int i=0; i<Tetra12Settings::VERTEX_NUM; i++){
			m_Cm->updateVertex(m_verteces[i], m_RS->local2World(m_Vertex[i]));
		}

		for(int i=0; i<Tetra12Settings::FACE_NUM; i++){		
			for(int j=0; j<Tetra12Settings::VERTEX_IN_FACE; j++){		
				m_faces[i].setPoint(j, m_verteces[m_facesOrder[i][j]]);
			}
		}

		m_hasChanged=false;
	}
}

//----------------------------------------------------------------------

void Tetra12::paintFace(const Vect<3>& V, const sf::Color& colour)
{
	int closeFace=1;
	double min=1.0*1e6;
	double d=0;
	for(int i=1; i<4; i++){
		d=Alg<3>::distance(V, getFaceCentroid(i));
		if(d<min){
			min=d;
			closeFace=i;
		}
	}
	LogicCell<Tetra12Settings>::paintFace(closeFace, colour);
}

//----------------------------------------------------------------------
