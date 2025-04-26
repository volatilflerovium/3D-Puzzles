/*********************************************************************
* Cell class                                  								*
* LogicCell class                              								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022  (Reviewed 04/2025)                            *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef CELL_H
#define CELL_H
#include "modulo.h"
#include "shape.h"
#include "utilities.h"

//----------------------------------------------------------------------

class Cell : public Shape
{
	public:
		Cell(std::shared_ptr<RSpace<3>> _RS)
		:Shape(_RS)
		{}

		virtual ~Cell()=default;

		virtual void setPin(ModuloI* pin)=0;
		virtual void isolate(const ModuloI* module)=0;	
		virtual void executeAction(Action* action, int direction)=0;		
		virtual void paintFace(int faceID, const sf::Color& colour)=0;
		virtual void paintFace(const Vect<3>& V, const sf::Color& colour){}

		virtual double quasiDistanceToCamera()=0;
		virtual void setMetadata(const ModuloI* module, int val)=0;
		virtual int getMetadata(const ModuloI* module)=0;

		virtual void debug()const=0;
};

//====================================================================
//====================================================================

struct CellSettings
{
	static const int FACE_NUM=0;
	static const int VERTEX_NUM=0;
	static const int VERTEX_IN_FACE=0;
	static const int NUM_EDGES=0;
	static const int NUM_PINS=0;
};

//----------------------------------------------------------------------

template<typename Data, bool=std::is_base_of<CellSettings, Data>::value>
class LogicCell 
{};

//----------------------------------------------------------------------

template<typename Data>
class LogicCell<Data, true> : public Cell
{
	public:
		LogicCell(std::shared_ptr<RSpace<3>> RS, int id=0);
		virtual ~LogicCell();

		LogicCell(const Cell&)=delete;
		LogicCell()=delete;

		virtual void setPin(ModuloI* pin);
		virtual void isolate(const ModuloI* module);	
		virtual void executeAction(Action* action, int direction);
		virtual void paintFace(int faceID, const sf::Color& colour);

		virtual void draw();
		virtual void rotate(const Rotation<3>& Rt);
		virtual void revRotate(const Rotation<3>& Rt);
		virtual void setLocalPosition(const Vect<3>& V);
		virtual void setGlobalPosition(const Vect<3>& V);

		virtual double quasiDistanceToCamera();
		virtual void setMetadata(const ModuloI* module, int val);
		virtual int getMetadata(const ModuloI* module);

		virtual void debug()const override
		{
			std::cout<<m_id<<"\n";
			for(int i=0; i<Data::VERTEX_NUM; i++){
				std::cout<<m_Vertex[i]<<" : ";
			}
			std::cout<<"\n";
		}

	protected:
		void setCentroid();
		Vect<3> getFaceCentroid(int faceID) const;

		/*
		 * A cube is made of 6 sf::ConvexShapeS, each of them is formed by
		 * 4 vertices, these vertices need to be given an orientation
		 * Face struct holds the orientation (order of vertices) in each
		 * face of the cube */ 
		class FaceOrder
		{
			public:
				class DataProxy
				{
					public:
						DataProxy(FaceOrder& order, int idx)
						:m_faceData((order.m_order)[idx])
						{}

						DataProxy& operator=(const DataProxy& rhs)
						{
							m_faceData=rhs;
							return *this;
						}

						DataProxy& operator=(int data)
						{	
							m_faceData=data;
							return *this;
						}
						
						~DataProxy()
						{}

						operator int() const
						{
							return m_faceData;
						}
	
					private:
						int& m_faceData;
				};

				const int operator[](int idx) const
				{
					return m_order[idx];
				}
	
				DataProxy operator[](int i)
				{
					return DataProxy(*this, i);
				}

			friend class DataProxy;
			private:
				int m_order[Data::VERTEX_IN_FACE];
		};

		sf::ConvexShape m_faces[Data::FACE_NUM];		
		sf::Vector2f m_verteces[Data::VERTEX_NUM];
		FaceOrder m_facesOrder[Data::FACE_NUM];
	
		std::vector<int> m_paintOrder;
		std::vector<double> m_distances;

		int m_numEdges;

		ModuloI* m_pins[Data::NUM_PINS];
		int m_metadata[Data::NUM_PINS];
		int m_id;
};

//----------------------------------------------------------------------

template<typename Data>
LogicCell<Data, true>::LogicCell(std::shared_ptr<RSpace<3>> _RS, int id)
:Cell(_RS)
, m_numEdges(Data::NUM_EDGES)
, m_id(id)
{
	m_Vertex=new Vect<3>[Data::VERTEX_NUM];
	m_Edges=new sf::Vertex*[Data::NUM_EDGES];
	for(int i=0; i<Data::NUM_EDGES; i++){
		m_Edges[i]=new sf::Vertex[2];
		m_Edges[i][0].color=sf::Color(0, 0, 0);
		m_Edges[i][1].color=sf::Color(0, 0, 0);
	}

	for(int i=0; i<Data::FACE_NUM; i++){
		m_distances.push_back(0);
		m_paintOrder.push_back(i);
	}	

	for(int i=0; i<Data::NUM_PINS; i++){
		m_pins[i]=nullptr;
		m_metadata[i]=0;
	}
}

//----------------------------------------------------------------------

template<typename Data>
LogicCell<Data, true>::~LogicCell()
{
	delete[] m_Vertex;

	for(int i=0; i<Data::NUM_EDGES; i++){
		delete[] m_Edges[i];
	}
	delete[] m_Edges;
}

//----------------------------------------------------------------------

template<typename Data>
void LogicCell<Data, true>::executeAction(Action* action, int direction)
{
	ModuloI* tmp=nullptr;
	for(int i=0; i<Data::NUM_PINS; i++){
		if(nullptr==m_pins[i]){
			continue;
		}
		tmp=action->move(m_pins[i], direction);
		
		if(m_pins[i]!=tmp){
			m_pins[i]=tmp;
			m_pins[i]->reconnectCell(this, m_metadata[i]);
		}
	}	
}

//----------------------------------------------------------------------

template<typename Data>
void LogicCell<Data, true>::setPin(ModuloI* pin)
{
	for(int i=0; i<Data::NUM_PINS; i++){
		if(m_pins[i]==nullptr){
			m_pins[i]=pin;
			break;
		}
	}
}

//----------------------------------------------------------------------

template<typename Data>
void LogicCell<Data, true>::isolate(const ModuloI* module)
{
	for(int i=0; i<Data::NUM_PINS; i++){
		if(m_pins[i]!=nullptr && module!=m_pins[i]){
			m_pins[i]->disconnect(this);
		}
	}
}

//----------------------------------------------------------------------

template<typename Data>
void LogicCell<Data, true>::rotate(const Rotation<3>& Rt)
{
	Rt.rotate(m_centroid);

	for(int i=0; i<Data::VERTEX_NUM; i++){
		Rt.rotate(m_Vertex[i]);
	}
	m_hasChanged=true;
}

//----------------------------------------------------------------------

template<typename Data>
void LogicCell<Data, true>::revRotate(const Rotation<3>& Rt)
{
	Rt.revRotate(m_centroid);
	for(int i=0; i<Data::VERTEX_NUM; i++){
		Rt.revRotate(m_Vertex[i]);
	}
	m_hasChanged=true;
}

//----------------------------------------------------------------------

template<typename Data>
void LogicCell<Data, true>::setGlobalPosition(const Vect<3>& V)
{
	m_RS->move(V);
	m_hasChanged=true;
}

//----------------------------------------------------------------------

template<typename Data>
void LogicCell<Data, true>::setLocalPosition(const Vect<3>& V)
{
	for(int i=0; i<Data::VERTEX_NUM; i++){
		m_Vertex[i]+=V;
	}

	setCentroid();

	updateDisplay();
	m_hasChanged=true;
}

//----------------------------------------------------------------------

template<typename Data>
void LogicCell<Data, true>::draw()
{
	if(!m_display){
		return;
	}

	updateEdges();

	//if(m_hasChanged || true)
	{
		for(int i=0; i<Data::FACE_NUM; i++){
			m_paintOrder[i]=i;
			m_distances[i]=CameraManager::quasiDistanceToCamera(getFaceCentroid(i));
		}

		sort(m_paintOrder.begin(), m_paintOrder.end(), [this](int i, int j){
			return m_distances[i]>m_distances[j];
		});
	}
	
	for(int i=0; i<m_numEdges; i++){
		CameraManager::draw(m_Edges[i], 2, sf::Lines);
	}
	
	for(int i=0; i<Data::FACE_NUM; i++){
		CameraManager::draw(m_faces[m_paintOrder[i]]);
	}
}

//----------------------------------------------------------------------

template<typename Data>
Vect<3> LogicCell<Data, true>::getFaceCentroid(int faceID) const
{
	Vect<3> centroid{0, 0, 0};
	for(int i=0; i<Data::VERTEX_IN_FACE; i++){
		centroid+=m_Vertex[m_facesOrder[faceID][i]];
	}
	centroid*=1.0/(1.0*Data::VERTEX_IN_FACE);
	
	return centroid;
}

//----------------------------------------------------------------------

template<typename Data>
inline void LogicCell<Data, true>::paintFace(int faceID, const sf::Color& colour)
{
	m_faces[faceID].setFillColor(colour);
}

//----------------------------------------------------------------------

template<typename Data>
inline double LogicCell<Data, true>::quasiDistanceToCamera()
{
	return CameraManager::quasiDistanceToCamera(getCentroidW());
}

//----------------------------------------------------------------------

template<typename Data>
inline void LogicCell<Data, true>::setCentroid()
{
	m_centroid*=0;
	for(int i=0; i<Data::VERTEX_NUM; i++){
		m_centroid+=m_Vertex[i];
	}
	
	m_centroid*=1.0/(1.0*Data::VERTEX_NUM);
}

//----------------------------------------------------------------------

template<typename Data>
inline void LogicCell<Data, true>::setMetadata(const ModuloI* module, int val)
{
	for(int i=0; i<Data::NUM_PINS; i++){
		if(m_pins[i]==module){
			m_metadata[i]=val;
			break;
		}
	}
}

//----------------------------------------------------------------------

template<typename Data>
inline int LogicCell<Data, true>::getMetadata(const ModuloI* module)
{
	for(int i=0; i<Data::NUM_PINS; i++){
		if(m_pins[i]==module){
			return m_metadata[i];
		}
	}
	return 0;
}

//----------------------------------------------------------------------



#endif
