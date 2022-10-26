#include "../include/rubik.h"
#include "../include/cube.h"

//======================================================================

Rubik::Rubik(std::shared_ptr<RSpace<3>> RS, Vect<3>* vertices)
:Puzzle(RS, 90.0)
{
	double dk=-1.5;
	Vect<3> position2({dk*SL, dk*SL, dk*SL});
	Vect<3> position(0, 0);
	for(int i=0; i<LocalSettings::SLICES; i++){
		position=Vect<3>({0, 0, i*SL});
		for(int j=0; j<RublikSettings::CELLS_IN_MODULO; j++){		
			m_cells[i*RublikSettings::CELLS_IN_MODULO+j]=new Cube(m_RS, (position2+position), i*RublikSettings::CELLS_IN_MODULO+j);

			if(j==2){
				position=Vect<3>({0, SL, i*SL});
			}
			else if(j==5){
				position=Vect<3>({0, 2*SL, i*SL});
			}
			else{
				position+=Vect<3>({SL, 0, 0});
			}
		}
	}

	for(int i=0; i<RublikSettings::TOTAL_MODULOS; i++){
		if(i<2){
			m_modules[i]=new ModuloR({1, 0, 0}, DELTA_ROTATION);
		}
		else if(i<4){
			m_modules[i]=new ModuloR({0, 1, 0}, DELTA_ROTATION);
		}
		else{
			m_modules[i]=new ModuloR({0, 0, 1}, DELTA_ROTATION);
		}
		m_modules[i]->setID(i);
	}

	//Now all the modules are created so now we can added to the action...Notice the following is not possible since the m_module[3] has NOT been created yet!!!
	//m_modules[i]->setAction(m_modules[5], m_modules[3], m_modules[4], m_modules[2]);
	for(int i=0; i<RublikSettings::TOTAL_MODULOS; i++){
		if(i<2){// X
			m_modules[i]->setAction(m_modules[5], m_modules[3], m_modules[4], m_modules[2]);
		}
		else if(i<4){// Y
			m_modules[i]->setAction(m_modules[0], m_modules[4], m_modules[1], m_modules[5]);
		}
		else{// Z
			m_modules[i]->setAction(m_modules[0], m_modules[3], m_modules[1], m_modules[2]);
		}
	}//*/

	int k=0;
	for(int i=0; i<2; i++){
		for(int j=0; j<RublikSettings::CELLS_IN_MODULO; j++){		
			//for X
			k=3*j+2*i;
			m_modules[i]->registerCell(m_cells[k]);
			m_cells[k]->setPin(m_modules[i]);

			//for Y
			k=9*(j%3)+std::floor(j/3)+6*i;
			m_modules[2+i]->registerCell(m_cells[k]);
			m_cells[k]->setPin(m_modules[2+i]);

			//for Z
			k=j+18*i;
			m_modules[4+i]->registerCell(m_cells[k]);
			m_cells[k]->setPin(m_modules[4+i]);
		}
	}
	
	m_modules[0]->paint(4, sf::Color::Red); 
	m_modules[1]->paint(2, sf::Color(17, 170, 45));//::Green);
	m_modules[2]->paint(1, sf::Color::Yellow);
	m_modules[3]->paint(3, sf::Color::Blue);
	m_modules[4]->paint(0, sf::Color::Magenta);
	m_modules[5]->paint(5, sf::Color::Cyan);
}

//----------------------------------------------------------------------
