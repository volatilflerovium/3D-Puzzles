/*********************************************************************
* CommandQueue class                               						*
* UII, GroupReactiveButtons class                     					*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022  (Reviewed 04/2025)                            *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef USER_INPUT_H
#define USER_INPUT_H
#include "handler.h"

#include <queue>

//====================================================================

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

//====================================================================

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

//====================================================================

class CommandQueue
{
	public:
		CommandQueue()=default;
		~CommandQueue()=default;

		void push(Command2 command);
		Command2 pop();
		bool isEmpty() const;
		int size() const;

	private:
		std::queue<Command2> m_queue;
		bool m_removeFront{false};
};

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
	return (this->size())<1;
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
		UII(uint totalCorners)
		{
			m_corners=new Vect<3>[totalCorners]; 
		}

		virtual ~UII()
		{
			delete[] m_corners;
		}

		virtual void eventHandler(const sf::Event& event)=0;
		virtual void updateView()=0;
		
		virtual void setCorners(int idx, std::initializer_list<int > dataList)=0;
		virtual void setModulos(int idx, std::initializer_list<int > dataList)=0;
		virtual void setOrientation(int idx, std::initializer_list<int > dataList)=0;		

		void setVertix(uint idx, double x, double y, double z, double factor=0)
		{
			if(factor==0){
				m_corners[idx]=Vect<3>{x, y, z};
			}
			else{
				m_corners[idx]=Vect<3>{factor*x, factor*y, factor*z};
			}
		}

		const Vect<3>* getCorners() const
		{
			return m_corners;
		}

		CommandQueue m_commandQueue;

	protected:
		Vect<3>* m_corners;
};

//----------------------------------------------------------------------

template<int ButtonSides, int TotalButtons>
class GroupReactiveButtons : public UII
{
	public:
		GroupReactiveButtons(int totalCorners)
		:UII(totalCorners)
		{}

		virtual ~GroupReactiveButtons()=default;

		virtual void updateView();
		virtual void eventHandler(const sf::Event& event);
		
		void setCorners(int idx, std::initializer_list<int > dataList);
		void setModulos(int idx, std::initializer_list<int > dataList);
		void setOrientation(int idx, std::initializer_list<int > dataList);

	private:
		Handler<ButtonSides> m_handlers[TotalButtons];
		double m_boundaryDistances[TotalButtons];
		std::vector<int> m_indexes;

		EE clickedButtons(float x, float y) const;
		Instruction resolveInput(int boundaryIdx, const sf::Vector2i& V, const sf::Vector2i& W);
		int getButton(const EE& x);
};

//----------------------------------------------------------------------

template<int ButtonSides, int TotalButtons>
inline void GroupReactiveButtons<ButtonSides, TotalButtons>::setCorners(int idx, std::initializer_list<int > dataList)
{
	m_handlers[idx].loader(m_handlers[idx].m_corners, dataList);
}

//----------------------------------------------------------------------

template<int ButtonSides, int TotalButtons>
inline void GroupReactiveButtons<ButtonSides, TotalButtons>::setModulos(int idx, std::initializer_list<int > dataList)
{
	m_handlers[idx].loader(m_handlers[idx].m_moduleIndexes, dataList);
}

//----------------------------------------------------------------------

template<int ButtonSides, int TotalButtons>
inline void GroupReactiveButtons<ButtonSides, TotalButtons>::setOrientation(int idx, std::initializer_list<int > dataList)
{
	m_handlers[idx].loader(m_handlers[idx].m_orientation, dataList);
}

//----------------------------------------------------------------------

template<int ButtonSides, int TotalButtons>
EE GroupReactiveButtons<ButtonSides, TotalButtons>::clickedButtons(float x, float y) const
{
	EE clicked;
	for(int i=0; i<TotalButtons; i++){				
		if(m_handlers[i].isInnerPoint(x, y)){
			clicked(i);
		}
	}

	return clicked;
}

//----------------------------------------------------------------------

template<int ButtonSides, int TotalButtons>
void GroupReactiveButtons<ButtonSides, TotalButtons>::eventHandler(const sf::Event& event)
{
	if(event.type == sf::Event::MouseMoved){
		return;
	}

	static EE boundaryIdx;
	static sf::Vector2i startPoint;
	if(event.type==sf::Event::MouseButtonPressed &&
		sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		startPoint=Normalization::positionOnView();
		boundaryIdx=clickedButtons(startPoint.x, startPoint.y);
	}
	else if(event.type==sf::Event::MouseButtonReleased){
		sf::Vector2i endPoint=Normalization::positionOnView();

		if(endPoint!=startPoint){
			boundaryIdx=boundaryIdx & clickedButtons(endPoint.x, endPoint.y);

			if(boundaryIdx.isValid()){
				int idx=getButton(boundaryIdx);
				if(idx!=-1){
					Command2 cmd;
					cmd.m_action=derivedAction(PuzzleMove(resolveInput(idx, startPoint, endPoint)));
					m_commandQueue.push(cmd);
				}
			}
		}
	}
}

//----------------------------------------------------------------------

template<int ButtonSides, int TotalButtons>
int GroupReactiveButtons<ButtonSides, TotalButtons>::getButton(const EE& idxs)
{
	int min=-1;
	for(int i=0; i<TotalButtons; i++){
		EE e;
		e(i);
		if((e & idxs).isValid()){
			if(min==-1){
				min=i;
				continue;
			}
			if(m_boundaryDistances[i]<m_boundaryDistances[min]){
				min=i;
			}
		}
	}

	return min;
}

//----------------------------------------------------------------------

template<int ButtonSides, int TotalButtons>
void GroupReactiveButtons<ButtonSides, TotalButtons>::updateView()
{
	for(int i=0; i<TotalButtons; i++){
		m_handlers[i].update(m_corners);
		m_boundaryDistances[i]=m_handlers[i].weight(m_corners);
	}
}


//----------------------------------------------------------------------

template<int ButtonSides, int TotalButtons>
Instruction GroupReactiveButtons<ButtonSides, TotalButtons>::resolveInput(int boundaryIdx, const sf::Vector2i& V, const sf::Vector2i& W)
{
	return m_handlers[boundaryIdx].getInstruction(V, W);
}

//----------------------------------------------------------------------


#endif
