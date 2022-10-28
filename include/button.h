/*********************************************************************
* Clickable class                              								*
* Button class                               								*
* MouseEvent class                             								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef BUTTON_H
#define BUTTON_H

#include <functional>
#include <SFML/Graphics.hpp>

#include "base.h"
#include "reference_frame.h"
#include "line.h"

typedef	std::function<void()> Command;

template<int T>
struct MouseBotton
{
	enum data{val=T};
};

//======================================================================
//######################################################################
//======================================================================

template<int N>
class Clickable
{
	public:
		Clickable();
		Clickable(Vect<2> (&corners)[N]);
		virtual ~Clickable();

		virtual void update(Vect<2> (&corners)[N]);
		virtual bool isInnerPoint(float x, float y) const;
		virtual void validate(int x, int y, MouseBotton<sf::Event::MouseButtonPressed>);		
		virtual void validate(int x, int y, MouseBotton<sf::Event::MouseButtonReleased>);
		virtual bool isValid() const;
		virtual void isValid(bool valid);
		const Vect<2>& getCentroid() const;

	protected:
		Line m_edges[N];
		Vect<2> m_centroid;
		int m_criteria;
		bool m_valid;
		bool m_active; //the button might be not active, for example when it is not visible
};

//----------------------------------------------------------------------

template<int N>
Clickable<N>::Clickable()
:m_valid(false),
m_active(true)
{}

//----------------------------------------------------------------------

template<int N>
Clickable<N>::Clickable(Vect<2> (&corners)[N])
:Clickable()
{
	update(corners);
}

//----------------------------------------------------------------------

template<int N>
Clickable<N>::~Clickable()
{}

//----------------------------------------------------------------------

template<int N>
void Clickable<N>::update(Vect<2> (&corners)[N])
{
	m_centroid*=0.0;

	for(int i=0; i<N; i++){
		m_edges[i].updateLine(corners[i], corners[(i+1)%N]);
		m_centroid+=corners[i];
	}

	m_centroid*=1.0/(N*1.0);

	m_criteria=0;
	for(int i=0; i<N; i++){
		m_criteria=m_criteria | (m_edges[i].sider(m_centroid[0], m_centroid[1])<<i);
	}
}

//----------------------------------------------------------------------

template<int N>
bool Clickable<N>::isInnerPoint(float x, float y) const
{
	int criteria=0;
	for(int i=0; i<N; i++){
		criteria=criteria | (m_edges[i].sider(x, y)<<i);
	}

	return (criteria==m_criteria);
}

//----------------------------------------------------------------------

template<int N>
void Clickable<N>::validate(int x, int y, MouseBotton<sf::Event::MouseButtonPressed>)
{
	if(m_active){
		m_valid=isInnerPoint(x, y);
	}
}

//----------------------------------------------------------------------

template<int N>
void Clickable<N>::validate(int x, int y, MouseBotton<sf::Event::MouseButtonReleased>)
{
	m_valid=m_active && m_valid;
	if(m_valid){
		m_valid=isInnerPoint(x, y);
	}
}

//----------------------------------------------------------------------

template<int N>
bool Clickable<N>::isValid() const
{
	return m_active && m_valid;
}

//----------------------------------------------------------------------

template<int N>
void Clickable<N>::isValid(bool valid)
{
	m_valid=valid;
}

//----------------------------------------------------------------------

template<int N>
const Vect<2>& Clickable<N>::getCentroid() const
{
	return m_centroid;
}

//======================================================================
//######################################################################
//======================================================================

template<char const* fontFile>
sf::Text LabelMaker(const char* text, unsigned int characterSize=30)
{
	static bool isSet=false;
	static sf::Font font;
	if(!isSet){
		std::string fontPath;
		fontPath.reserve(100);
		if(!isSet){
			fontPath.append("/usr/share/fonts/truetype/liberation/");
			fontPath.append(fontFile);
			isSet=font.loadFromFile(fontPath);
		}		

		if(!isSet){	
			char* envVar=getenv("APPDIR");
			if(envVar){
				fontPath=std::string(envVar);
				fontPath.append("/usr/share/fonts/truetype/liberation/");
				fontPath.append(fontFile);
				isSet=font.loadFromFile(fontPath);
				std::cout<<"here!\n";
			}		
		}
	}

	return sf::Text(text, font, characterSize);
}

//======================================================================
//######################################################################
//======================================================================

class Button : public Clickable<4>
{
	public:
		Button(const char* label, Command cmd);
		virtual ~Button();

		void setPosition(float x, float y);
		void draw();
		void eventHandler(const sf::Event& event);
		sf::Vector2f getSize() const;

	private:
		sf::RectangleShape m_button;
		sf::Text m_label;
		Command m_cmd;

		//top-left corner in normalized coordinates
		float m_posX;
		float m_posY;

		static const float c_padding;
		static float s_paddingY;
		static bool m_fontLoaded;
};

//----------------------------------------------------------------------

inline Button::~Button()
{}

//----------------------------------------------------------------------

inline void Button::draw()
{
	Normalization::m_window->draw(m_button);
	Normalization::m_window->draw(m_label);	
}

//----------------------------------------------------------------------

inline sf::Vector2f Button::getSize() const
{
	return m_button.getSize();
}

//----------------------------------------------------------------------

#endif
