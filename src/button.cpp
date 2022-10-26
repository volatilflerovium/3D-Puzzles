#include <iostream>
#include "../include/button.h"

//----------------------------------------------------------------------

const float Button::c_padding(18.0);
float Button::s_paddingY(-1.0);
bool Button::m_fontLoaded(false);

//----------------------------------------------------------------------

Button::Button(const char* label, Command cmd)
:
m_cmd(cmd),
m_posX(0),
m_posY(0)
{
	m_button.setFillColor(sf::Color(36, 198, 84));

	m_label=LabelMaker<FontLSR>(label, 24);
	m_label.setFillColor(sf::Color::Black);
	m_label.setStyle(sf::Text::Bold);

	sf::Rect<float> txtSize=m_label.getGlobalBounds();
	
	if(s_paddingY<0){
		s_paddingY=m_label.getGlobalBounds().top-m_label.getPosition().y;
	}

	float w=txtSize.width+2*c_padding;
	float h=txtSize.height+2*c_padding;

	m_button.setSize(sf::Vector2f(w, h));

	setPosition(0, 0);
}

//----------------------------------------------------------------------

void Button::setPosition(float x, float y)
{
	m_posX=x;
	m_posY=y;

	m_button.setPosition(Normalization::absolute(m_posX, m_posY));

	float yy=m_posY-(c_padding-s_paddingY);
	m_label.setPosition(Normalization::absolute(m_posX+c_padding, yy));

	sf::Vector2f a=Normalization::absolute(m_posX, m_posY);

	sf::Vector2f tmp=m_button.getSize(); 
	Vect<2> corners[4];
	corners[0]=Vect<2>{x, y};
	corners[1]=Vect<2>{x+tmp.x, y};
	corners[2]=Vect<2>{x+tmp.x, y-tmp.y};
	corners[3]=Vect<2>{x, y-tmp.y};

	update(corners);
}

//----------------------------------------------------------------------
	
void Button::eventHandler(const sf::Event& event)
{
	if(event.type == sf::Event::MouseMoved){
		return;
	}

	sf::Vector2i mousePosition=Normalization::getMousePosition();

	if(event.type==sf::Event::MouseButtonPressed &&
		sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		validate(mousePosition.x, mousePosition.y, MouseBotton<sf::Event::MouseButtonPressed>());
	}
	else if(event.type==sf::Event::MouseButtonReleased){				
		validate(mousePosition.x, mousePosition.y, MouseBotton<sf::Event::MouseButtonReleased>());
		if(isValid()){
			m_cmd();
		}
	}
}

//----------------------------------------------------------------------

