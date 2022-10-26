#include "../include/cube.h"
#include "../include/action.h"

//----------------------------------------------------------------------

Cube::Cube(std::shared_ptr<RSpace<3>> _RS, const Vect<3>& position, int id)
: LogicCell(_RS, id)
{
	m_Vertex[0]=Vect<3>{0,0,0};
	m_Vertex[1]=Vect<3>{SL, 0, 0};
	m_Vertex[2]=Vect<3>{SL, SL, 0};
	m_Vertex[3]=Vect<3>{0, SL, 0};
	m_Vertex[4]=Vect<3>{0, 0, SL};
	m_Vertex[5]=Vect<3>{SL, 0, SL};
	m_Vertex[6]=Vect<3>{SL, SL, SL};
	m_Vertex[7]=Vect<3>{0, SL, SL};

	setCentroid();

	setFace(0, 0, 1, 2, 3);
	setFace(1, 0, 1, 5, 4);
	setFace(2, 1, 2, 6, 5);
	setFace(3, 3, 2, 6, 7);
	setFace(4, 0, 3, 7, 4);
	setFace(5, 4, 5, 6, 7);

	for(int i=0; i<6; i++){
		m_faces[i].setPointCount(4);
		m_faces[i].setFillColor(sf::Color(0, 0, 0));
		m_faces[i].setOutlineColor(sf::Color(0, 0, 0));
	}

	setLocalPosition(position);
	updateEdges();
}

//----------------------------------------------------------------------

void Cube::updateEdges()
{
	if(m_hasChanged || true)
	{
		int j=0, r=0;
		
		for(int i=0; i<8; i++){
			j=4*(i/4)+(i+1)%4;
			m_Cm->updateVertex(m_Edges[i][0], m_RS->local2World(m_Vertex[i]));
			m_Cm->updateVertex(m_Edges[i][1], m_RS->local2World(m_Vertex[j]));
		}

		for(int i=8; i<m_numEdges; i++){
			j=4*(i/4)+(i+1)%4;
			m_Edges[i][0]=m_Edges[i-8][0];
			m_Edges[i][1]=m_Edges[i-4][0];
		}

		for(int i=0; i<CubeSettings::VERTEX_NUM; i++){
			m_Cm->updateVertex(m_verteces[i], m_RS->local2World(m_Vertex[i]));
		}

		for(int i=0; i<CubeSettings::FACE_NUM; i++){		
			for(int j=0; j<CubeSettings::VERTEX_IN_FACE; j++){		
				m_faces[i].setPoint(j, m_verteces[m_facesOrder[i][j]]);
			}
		}
	}
}

//----------------------------------------------------------------------
