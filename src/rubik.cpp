#include "rubik.h"
#include "cube.h"

//======================================================================

Rubik::Rubik(std::shared_ptr<RSpace<3>> RS, const Vect<3>* vertices)
:Puzzle(RS, 90.0)
{
	double dk=-1.5;
	Vect<3> position2({dk*SL, dk*SL, dk*SL});
	Vect<3> position(0, 0);
	for(int i=0; i<LocalSettings::SLICES; i++){
		position=Vect<3>({0, 0, i*SL});
		for(uint j=0; j<RublikSettings::CELLS_IN_MODULO; j++){		
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

	for(uint i=0; i<RublikSettings::TOTAL_MODULOS; i++){
		if(i<2 || i==6){
			m_modules[i]=new ModuloR({1, 0, 0}, DELTA_ROTATION);
		}
		else if(i<4 || i==7){
			m_modules[i]=new ModuloR({0, 1, 0}, DELTA_ROTATION);
		}
		else{
			m_modules[i]=new ModuloR({0, 0, 1}, DELTA_ROTATION);
		}
		m_modules[i]->setID(i);
	}

	/*
	 * Now all the modules are created so now we can added to the action...
	 * Notice the following is not possible since the m_module[3] has NOT been
	 * created yet!!!
	 * m_modules[i]->setAction(m_modules[5], m_modules[3], m_modules[4], m_modules[2]);
	*/
	for(uint i=0; i<RublikSettings::TOTAL_MODULOS; i++){
		if(i<2 || i==6){// X
			m_modules[i]->setAction(m_modules[5], m_modules[3], m_modules[4], m_modules[2]);
			dynamic_cast<ModuloR*>(m_modules[i])->setAction2(m_modules[7], m_modules[8]);
		}
		else if(i<4 || i==7){// Y
			m_modules[i]->setAction(m_modules[0], m_modules[4], m_modules[1], m_modules[5]);
			dynamic_cast<ModuloR*>(m_modules[i])->setAction2(m_modules[6], m_modules[8]);
		}
		else {//if(i<6){// Z
			m_modules[i]->setAction(m_modules[0], m_modules[3], m_modules[1], m_modules[2]);
			dynamic_cast<ModuloR*>(m_modules[i])->setAction2(m_modules[6], m_modules[7]);
		}
	}

	int k=0;
	//register cells on slides with X rotation;
	for(int i=0; i<3; i++){
		for(uint j=0; j<RublikSettings::CELLS_IN_MODULO; j++){
			int m=0;
			if(i==1){
				m=6;
			}
			else if(i==2){
				m=1;
			}
			k=3*j+i;
			m_modules[m]->registerCell(m_cells[k]);
			m_cells[k]->setPin(m_modules[m]);
		}
	}
		//register cells on slides with Y rotation;
	k=0;
	for(int i=0; i<3; i++){
		for(uint j=0; j<RublikSettings::CELLS_IN_MODULO; j++){
			int m=2;
			if(i==1){
				m=7;
			}
			else if(i==2){
				m=3;
			}

			m_modules[m]->registerCell(m_cells[k]);
			m_cells[k]->setPin(m_modules[m]);
			k++;
			if(k%3==0){
				k+=6;
			}
		}
		k=3*(i+1);
	}

	//register cells on slides with Z rotation;
	for(uint i=0; i<3; i++){
		for(uint j=0; j<RublikSettings::CELLS_IN_MODULO; j++){
			k=j+RublikSettings::CELLS_IN_MODULO*i;

			int m=4;
			if(i==1){
				m=8;
			}
			else if(i==2){
				m=5;
			}
			
			m_modules[m]->registerCell(m_cells[k]);
			m_cells[k]->setPin(m_modules[m]);
		}
	}
	
	m_modules[0]->paint(4, sf::Color::Red); 
	m_modules[1]->paint(2, sf::Color(17, 170, 45));//::Green);
	m_modules[2]->paint(1, sf::Color::Yellow);
	m_modules[3]->paint(3, sf::Color::Blue);//(145, 9, 231));
	m_modules[4]->paint(0, sf::Color::Magenta); // pink
	m_modules[5]->paint(5, sf::Color::Cyan); 
}


//----------------------------------------------------------------------
