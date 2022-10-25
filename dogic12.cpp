#include "dogic12.h"
#include "tetra.h"

//----------------------------------------------------------------------

Dogic12::Dogic12(std::shared_ptr<RSpace<3>> RS, Vect<3>* vertices)
:Puzzle(RS, 72.0),
m_cellIndex(0),
m_partIndex(0)
{
	m_modules[0]=new ModuloT12(vertices[1], DELTA_ROTATION);
	m_modules[1]=new ModuloT12(vertices[0], DELTA_ROTATION);
	m_modules[2]=new ModuloT12(vertices[8], DELTA_ROTATION);
	m_modules[3]=new ModuloT12(vertices[5], DELTA_ROTATION);
	m_modules[4]=new ModuloT12(vertices[6], DELTA_ROTATION);
	m_modules[5]=new ModuloT12(vertices[11], DELTA_ROTATION);
	m_modules[6]=new ModuloT12(vertices[7], DELTA_ROTATION);
	m_modules[7]=new ModuloT12(vertices[4], DELTA_ROTATION);
	m_modules[8]=new ModuloT12(vertices[9], DELTA_ROTATION);
	m_modules[9]=new ModuloT12(vertices[2], DELTA_ROTATION);
	m_modules[10]=new ModuloT12(vertices[10], DELTA_ROTATION);
	m_modules[11]=new ModuloT12(vertices[3], DELTA_ROTATION);

	mkFace(vertices[1], vertices[0], vertices[11]);//0
	mkFace(vertices[1], vertices[11], vertices[6]);//1
	mkFace(vertices[1], vertices[6], vertices[5]);//2
	mkFace(vertices[1], vertices[5], vertices[8]);//3
	mkFace(vertices[1], vertices[8], vertices[0]);//4
	
	mkFace(vertices[3], vertices[4], vertices[7]);//5
	mkFace(vertices[3], vertices[7], vertices[10]);//6
	mkFace(vertices[3], vertices[10], vertices[2]);//7
	mkFace(vertices[3], vertices[2], vertices[9]);//8
	mkFace(vertices[3], vertices[9], vertices[4]);//9

	mkFace(vertices[11], vertices[7], vertices[10]);//10
	mkFace(vertices[11], vertices[0], vertices[7]);//11
	mkFace(vertices[0], vertices[4], vertices[7]);//12
	mkFace(vertices[0], vertices[8], vertices[4]);//13
	mkFace(vertices[8], vertices[9], vertices[4]);//14
	mkFace(vertices[8], vertices[9], vertices[5]);//15
	
	mkFace(vertices[5], vertices[2], vertices[9]);//16
	mkFace(vertices[5], vertices[6], vertices[2]);//17
	mkFace(vertices[6], vertices[10], vertices[2]);//18
	mkFace(vertices[11], vertices[10], vertices[6]);//19

	sf::Color colours[]={sf::Color(179, 247, 19),
	sf::Color(214, 38, 76),
	sf::Color(1, 193, 117),
	sf::Color(32, 2, 140),
	sf::Color(17, 170, 45),
	sf::Color(39, 65, 142),
	sf::Color(132, 17, 76),
	sf::Color::Blue,
	sf::Color(249, 234, 182),
	sf::Color(215, 104, 232),
	sf::Color::Magenta,
	sf::Color::Red};

	looping(vertices, [&, this](std::vector<int> idxs, int i){
		for(int k=0; k<5; k++){
			this->m_cells[idxs[k]]->paintFace(0, colours[i]);
		}
		for(int k=5; k<10; k++){
			this->m_cells[idxs[k]]->paintFace(vertices[i], colours[i]);
		}
	});

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

	Vect<3> moduleCorners[12]={vertices[1], vertices[0], vertices[8], vertices[5],
	vertices[6], vertices[11], vertices[7], vertices[4],
	vertices[9], vertices[2], vertices[10], vertices[3]};

	looping(moduleCorners, [this](const std::vector<int>& idxs, int i){
		for(int k=0; k<20; k++){
			if(k<5){
				(dynamic_cast<ModuloT12*>(m_modules[i]))->registerUpperCells(m_cells[idxs[k]]);
			}
			else{
				(dynamic_cast<ModuloT12*>(m_modules[i]))->registerLowerCells(m_cells[idxs[k]]);
			}
		}
	});

	for(int i=0; i<Dogic12Settings::TOTAL_MODULOS; i++){
		if(m_modules[i]){
			m_modules[i]->setID(i);
		}
	}
}

//----------------------------------------------------------------------

void Dogic12::mkFace(const Vect<3>& V1, const Vect<3>& V2, const Vect<3>& V3)
{
	Vect<3> W1=mk1(V1, V2);
	Vect<3> W2=mk1(V2, V3);
	Vect<3> W3=mk1(V1, V3);

	double y=350.0;
	
	Parts part;	
	part.m_idx[0]=m_cellIndex;	
	m_cells[m_cellIndex]=new Tetra12(m_RS, V1, W1, W3, y, m_cellIndex);
	m_cellIndex++;

	part.m_idx[1]=m_cellIndex;
	m_cells[m_cellIndex]=new Tetra12(m_RS, W1, W2, W3, 0.0, m_cellIndex);
	m_cells[m_cellIndex]->paintFace(0, sf::Color::Transparent);
	m_cellIndex++;

	part.m_idx[2]=m_cellIndex;
	m_cells[m_cellIndex]=new Tetra12(m_RS, W1, V2, W2, y, m_cellIndex);
	m_cellIndex++;

	part.m_idx[3]=m_cellIndex;
	m_cells[m_cellIndex]=new Tetra12(m_RS, W2, V3, W3, y, m_cellIndex);
	m_cellIndex++;

	m_parts[m_partIndex++]=part;
}

//----------------------------------------------------------------------
