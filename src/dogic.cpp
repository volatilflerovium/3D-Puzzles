#include "../include/dogic.h"
#include "../include/tetra.h"

//----------------------------------------------------------------------

Dogic::Dogic(std::shared_ptr<RSpace<3>> RS, Vect<3>* vertices)
:Puzzle(RS, 72.0)
{
	double t=Alg<3>::distance(vertices[0], vertices[1]);
	double r=t/(2.0*(std::sin(M_PI/5.0)));//=(2*M_PI/5)/2
	double y=std::sqrt(t*t-r*r);

	int c=0;
	m_cells[0]=new Tetra(m_RS, vertices[1], vertices[0], vertices[11], y, c++);
	m_cells[1]=new Tetra(m_RS, vertices[1], vertices[11], vertices[6], y, c++);
	m_cells[2]=new Tetra(m_RS, vertices[1], vertices[6], vertices[5], y, c++);
	m_cells[3]=new Tetra(m_RS, vertices[1], vertices[5], vertices[8], y, c++);	
	m_cells[4]=new Tetra(m_RS, vertices[1], vertices[8], vertices[0], y, c++);	
	m_cells[5]=new Tetra(m_RS, vertices[3], vertices[4], vertices[7], y, c++);
	m_cells[6]=new Tetra(m_RS, vertices[3], vertices[7], vertices[10], y, c++);
	m_cells[7]=new Tetra(m_RS, vertices[3], vertices[10], vertices[2], y, c++);
	m_cells[8]=new Tetra(m_RS, vertices[3], vertices[2], vertices[9], y, c++);
	m_cells[9]=new Tetra(m_RS, vertices[3], vertices[9], vertices[4], y, c++);
	m_cells[10]=new Tetra(m_RS, vertices[11], vertices[7], vertices[10], y, c++);
	m_cells[11]=new Tetra(m_RS, vertices[11], vertices[0], vertices[7], y, c++);
	m_cells[12]=new Tetra(m_RS, vertices[0], vertices[4], vertices[7], y, c++);
	m_cells[13]=new Tetra(m_RS, vertices[0], vertices[8], vertices[4], y, c++);
	m_cells[14]=new Tetra(m_RS, vertices[8], vertices[9], vertices[4], y, c++);
	m_cells[15]=new Tetra(m_RS, vertices[8], vertices[9], vertices[5], y, c++);
	m_cells[16]=new Tetra(m_RS, vertices[5], vertices[2], vertices[9], y, c++);
	m_cells[17]=new Tetra(m_RS, vertices[5], vertices[6], vertices[2], y, c++);
	m_cells[18]=new Tetra(m_RS, vertices[6], vertices[10], vertices[2], y, c++);
	m_cells[19]=new Tetra(m_RS, vertices[11], vertices[10], vertices[6], y, c++);

	// Top 10 pairs
	m_cells[0]->paintFace(0, sf::Color(179, 247, 19)); 
	m_cells[11]->paintFace(0, sf::Color(179, 247, 19));
	//--------
	m_cells[4]->paintFace(0, sf::Color(214, 38, 76)); 
	m_cells[13]->paintFace(0, sf::Color(214, 38, 76));
	//--------
	m_cells[3]->paintFace(0, sf::Color(1, 193, 117)); 
	m_cells[15]->paintFace(0, sf::Color(1, 193, 117));
	//--------
	m_cells[2]->paintFace(0, sf::Color(32, 2, 140)); 
	m_cells[17]->paintFace(0, sf::Color(32, 2, 140));
	//--------
	m_cells[1]->paintFace(0, sf::Color::Blue); 
	m_cells[19]->paintFace(0, sf::Color::Blue);

	//--------
	//Bottom 10 pairs
	m_cells[12]->paintFace(0, sf::Color(17, 170, 45)); 	
	m_cells[5]->paintFace(0, sf::Color(17, 170, 45));
	//--------
	m_cells[14]->paintFace(0, sf::Color::Magenta); 
	m_cells[9]->paintFace(0, sf::Color::Magenta);
	//-------
	m_cells[16]->paintFace(0, sf::Color::Red);
	m_cells[8]->paintFace(0, sf::Color::Red);
	//--------
	m_cells[10]->paintFace(0, sf::Color(39, 65, 142)); 
	m_cells[6]->paintFace(0, sf::Color(39, 65, 142));
	//---------
	m_cells[7]->paintFace(0, sf::Color(132, 17, 76)); 
	m_cells[18]->paintFace(0, sf::Color(132, 17, 76));

	m_modules[0]=new ModuloT(vertices[1], DELTA_ROTATION);
	m_modules[1]=new ModuloT(vertices[0], DELTA_ROTATION);
	m_modules[2]=new ModuloT(vertices[8], DELTA_ROTATION);
	m_modules[3]=new ModuloT(vertices[5], DELTA_ROTATION);
	m_modules[4]=new ModuloT(vertices[6], DELTA_ROTATION);
	m_modules[5]=new ModuloT(vertices[11], DELTA_ROTATION);
	m_modules[6]=new ModuloT(vertices[7], DELTA_ROTATION);
	m_modules[7]=new ModuloT(vertices[4], DELTA_ROTATION);
	m_modules[8]=new ModuloT(vertices[9], DELTA_ROTATION);
	m_modules[9]=new ModuloT(vertices[2], DELTA_ROTATION);
	m_modules[10]=new ModuloT(vertices[10], DELTA_ROTATION);
	m_modules[11]=new ModuloT(vertices[3], DELTA_ROTATION);

	m_modules[0]->registerCells(m_cells[0], m_cells[1], m_cells[2], m_cells[3], m_cells[4]);
	m_modules[1]->registerCells(m_cells[0], m_cells[4], m_cells[13], m_cells[12], m_cells[11]);
	m_modules[2]->registerCells(m_cells[3], m_cells[15], m_cells[14], m_cells[13], m_cells[4]);
	m_modules[3]->registerCells(m_cells[2], m_cells[17], m_cells[16], m_cells[15], m_cells[3]);
	m_modules[4]->registerCells(m_cells[1], m_cells[19], m_cells[18], m_cells[17], m_cells[2]);
	m_modules[5]->registerCells(m_cells[0], m_cells[11], m_cells[10], m_cells[19], m_cells[1]);
	m_modules[6]->registerCells(m_cells[5], m_cells[6], m_cells[10], m_cells[11], m_cells[12]);
	m_modules[7]->registerCells(m_cells[9], m_cells[5], m_cells[12], m_cells[13], m_cells[14]);
	m_modules[8]->registerCells(m_cells[8], m_cells[9], m_cells[14], m_cells[15], m_cells[16]);
	m_modules[9]->registerCells(m_cells[7], m_cells[8], m_cells[16], m_cells[17], m_cells[18]);
	m_modules[10]->registerCells(m_cells[6], m_cells[7], m_cells[18], m_cells[19], m_cells[10]);
	m_modules[11]->registerCells(m_cells[5], m_cells[9], m_cells[8], m_cells[7], m_cells[6]);

	m_modules[0]->setAction(m_modules[2], m_modules[3], m_modules[4], m_modules[5],m_modules[1]);	
	m_modules[1]->setAction(m_modules[0], m_modules[5], m_modules[6], m_modules[7], m_modules[2]);
	m_modules[2]->setAction(m_modules[1], m_modules[7], m_modules[8], m_modules[3], m_modules[0]);
	m_modules[3]->setAction(m_modules[8], m_modules[9], m_modules[4], m_modules[0],m_modules[2]);
	m_modules[4]->setAction(m_modules[9], m_modules[10], m_modules[5], m_modules[0], m_modules[3]);
	m_modules[5]->setAction(m_modules[0], m_modules[4], m_modules[10], m_modules[6], m_modules[1]);
	m_modules[6]->setAction(m_modules[10], m_modules[11], m_modules[7], m_modules[1], m_modules[5]);
	m_modules[7]->setAction(m_modules[11], m_modules[8], m_modules[2], m_modules[1], m_modules[6]);
	m_modules[8]->setAction(m_modules[11],  m_modules[9], m_modules[3], m_modules[2], m_modules[7]);
	m_modules[9]->setAction(m_modules[11], m_modules[10], m_modules[4], m_modules[3], m_modules[8]);
	m_modules[10]->setAction(m_modules[11],  m_modules[6], m_modules[5], m_modules[4], m_modules[9]);
	m_modules[11]->setAction(m_modules[9], m_modules[8], m_modules[7], m_modules[6], m_modules[10]);
	
	for(int i=0; i<DogicSettings::TOTAL_MODULOS; i++){		
		m_modules[i]->setID(i);
	}
}

//----------------------------------------------------------------------
