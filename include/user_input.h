/*********************************************************************
* CommandQueue class                               						*
* UII, GroupReactiveButtons class                     					*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef USER_INPUT_H
#define USER_INPUT_H

#include <functional>
#include <queue>
#include <fstream>

#include "handler.h"
#include "camera.h"
#include "button.h"
#include "vector.h"
#include "puzzle_interface.h"
#include "base.h"
#include "utilities.h"

//======================================================================
//######################################################################
//======================================================================

struct PuzzleMove : public Instruction
{
	PuzzleMove(Instruction instruction)
	:Instruction(instruction)
	{}
	
	PuzzleMove(int a, int b, bool c)
	:Instruction(a, b, c)
	{}

	Instruction operator()(PuzzleI* puzzle) const
	{
		puzzle->setMove(*this);
		return *this;
	}
};

struct PuzzleFastMove : public Instruction
{
	PuzzleFastMove(Instruction instruction)
	:Instruction(instruction)
	{}
	
	PuzzleFastMove(int a, int b, bool c)
	:Instruction(a, b, c)
	{}

	Instruction operator()(PuzzleI* puzzle) const
	{
		puzzle->setFastMove(*this);
		return {-1, 1, 0};
	}
};
//======================================================================
//######################################################################
//======================================================================

class CommandQueue
{
	public:
		CommandQueue();
		~CommandQueue();

		void push(Command2 command);
		Command2 pop();
		bool isEmpty() const;
		int size() const;

	private:
		std::queue<Command2> m_queue;
		bool m_removeFront{false};	
};

//----------------------------------------------------------------------

inline CommandQueue::CommandQueue()
{}

//----------------------------------------------------------------------

inline CommandQueue::~CommandQueue()
{}

//----------------------------------------------------------------------

inline void CommandQueue::push(Command2 command)
{
	m_queue.push(command);
}

//----------------------------------------------------------------------

inline Command2 CommandQueue::pop()
{
	if(m_removeFront){
		m_queue.pop();
	}

	m_removeFront=true;

	return m_queue.front();
}

//----------------------------------------------------------------------

inline bool CommandQueue::isEmpty() const
{
	return this->size()<1;
}

//----------------------------------------------------------------------

inline int CommandQueue::size() const
{
	return m_queue.size()-static_cast<int>(m_removeFront);
}

//======================================================================
//######################################################################
//======================================================================

class UII
{
	public:
		virtual void eventHandler(const sf::Event& event)=0;
		virtual void updateView()=0;
		
		virtual void setCorners(int idx, std::initializer_list<int > dataList)=0;
		virtual void setModulos(int idx, std::initializer_list<int > dataList)=0;
		virtual void setOrientation(int idx, std::initializer_list<int > dataList)=0;

		static Vect<3>* m_corners;
		CommandQueue m_commandQueue;
};

//----------------------------------------------------------------------

template<int ButtonSides, int TotalButtons>
class GroupReactiveButtons : public UII
{
	public:
		GroupReactiveButtons(Camera* camera)
		:m_visibleBoundaries(TotalButtons, -1),
		m_camera(camera)
		{};

		virtual void updateView();
		virtual void eventHandler(const sf::Event& event);
		
		void setCorners(int idx, std::initializer_list<int > dataList);
		void setModulos(int idx, std::initializer_list<int > dataList);
		void setOrientation(int idx, std::initializer_list<int > dataList);

	private:
		std::vector<int> m_visibleBoundaries;
		Camera* m_camera;
		Handler<ButtonSides> m_boundaries[TotalButtons];
		double m_boundaryDistances[TotalButtons];
		
		int clickedModule(float x, float y) const;
		Instruction resolveInput(int boundaryIdx, const sf::Vector2i& V, const sf::Vector2i& W);
};

//----------------------------------------------------------------------

template<int ButtonSides, int TotalButtons>
inline void GroupReactiveButtons<ButtonSides, TotalButtons>::setCorners(int idx, std::initializer_list<int > dataList)
{
	m_boundaries[idx].loader(m_boundaries[idx].m_corners, dataList);
}

//----------------------------------------------------------------------

template<int ButtonSides, int TotalButtons>
inline void GroupReactiveButtons<ButtonSides, TotalButtons>::setModulos(int idx, std::initializer_list<int > dataList)
{
	m_boundaries[idx].loader(m_boundaries[idx].m_moduleIndexes, dataList);
}

//----------------------------------------------------------------------

template<int ButtonSides, int TotalButtons>
inline void GroupReactiveButtons<ButtonSides, TotalButtons>::setOrientation(int idx, std::initializer_list<int > dataList)
{
	m_boundaries[idx].loader(m_boundaries[idx].m_orientation, dataList);
}

//----------------------------------------------------------------------

template<int ButtonSides, int TotalButtons>
int GroupReactiveButtons<ButtonSides, TotalButtons>::clickedModule(float x, float y) const
{
	for(int i=0; i<TotalButtons; i++){				
		if(m_boundaries[i].isInnerPoint(x, y) && m_boundaries[i].isValid()){
			return i;
		}
	}

	return -1;
}

//----------------------------------------------------------------------

template<int ButtonSides, int TotalButtons>
void GroupReactiveButtons<ButtonSides, TotalButtons>::eventHandler(const sf::Event& event)
{
	if(event.type == sf::Event::MouseMoved){
		return;
	}

	static int boundaryIdx;
	static sf::Vector2i enterPoint;
	if(event.type==sf::Event::MouseButtonPressed &&
		sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		enterPoint=Normalization::positionOnView();
		boundaryIdx=clickedModule(enterPoint.x, enterPoint.y);
	}
	else if(event.type==sf::Event::MouseButtonReleased){
		sf::Vector2i mousePosition=Normalization::positionOnView();
		int t=clickedModule(mousePosition.x, mousePosition.y);
		if(boundaryIdx==clickedModule(mousePosition.x, mousePosition.y)){
			Command2 cmd;
			cmd.m_action=derivedAction(PuzzleMove(resolveInput(boundaryIdx, enterPoint, mousePosition)));
			m_commandQueue.push(cmd);
		}
	}
}

//----------------------------------------------------------------------

template<int ButtonSides, int TotalButtons>
void GroupReactiveButtons<ButtonSides, TotalButtons>::updateView()
{
	for(int i=0; i<TotalButtons; i++){
		m_boundaries[i].update(m_corners);
		m_boundaryDistances[i]=m_boundaries[i].weight(m_corners);
		m_boundaries[i].isValid(false);
		m_visibleBoundaries[i]=i;
	}

	sort(m_visibleBoundaries.begin(), m_visibleBoundaries.end(), [this](int i, int j){
		return m_boundaryDistances[i]>m_boundaryDistances[j];
	});
	
	bool a=false;
	int j=0;
	int r;
	int q=0;
	for(int i=0; i<TotalButtons; i++){
		a=false;
		j=m_visibleBoundaries[i];
		const Vect<2>& W=m_boundaries[j].getCentroid();
		for(int k=i+1; k<TotalButtons; k++){
			r=m_visibleBoundaries[k];
			if(m_boundaries[r].isInnerPoint(W[0], W[1])){
				a=true;
				break;
			}
		}

		if(a==false){
			m_boundaries[j].isValid(true);
		}
	}
}

//----------------------------------------------------------------------

template<int ButtonSides, int TotalButtons>
Instruction GroupReactiveButtons<ButtonSides, TotalButtons>::resolveInput(int boundaryIdx, const sf::Vector2i& V, const sf::Vector2i& W)
{
	return m_boundaries[boundaryIdx].getModuleDirection(V, W);
}

//----------------------------------------------------------------------

#endif
