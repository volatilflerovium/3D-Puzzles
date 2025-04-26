/*********************************************************************
* Dogic12 class                               								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022  (Reviewed 04/2025)                            *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef DOGIC12_H
#define DOGIC12_H

#include "puzzle.h"

//----------------------------------------------------------------------

struct Dogic12Settings : public PuzzleSettings
{
	static const unsigned int TOTAL_CELLS=80;
	static const unsigned int TOTAL_MODULOS=12;	
};

//----------------------------------------------------------------------

class Dogic12 : public Puzzle<Dogic12Settings>
{
	public:
		Dogic12(std::shared_ptr<RSpace<3>> RS, const Vect<3>* vertices);
		virtual ~Dogic12()=default;

	protected:
		Vect<3> mk1(const Vect<3>& vertex1, const Vect<3>& vertex2);
		void mkFace(const Vect<3>& vertex1, const Vect<3>& vertex2, const Vect<3>& vertex3);

		template<typename Func>
		void looping(const Vect<3>* vertices, Func cbk);

		struct Parts
		{
			int m_idx[4];
		};

		Parts m_parts[20];		
		unsigned int m_cellIndex;
		unsigned int m_partIndex;
};

//----------------------------------------------------------------------

inline Vect<3> Dogic12::mk1(const Vect<3>& vertex1, const Vect<3>& vertex2)
{
	Vect<3> W1=vertex1;
	W1-=vertex2;
	W1*=0.5;
	W1+=vertex2;
	return W1;
}

//----------------------------------------------------------------------

template<typename Func>
void Dogic12::looping(const Vect<3>* vertices, Func cbk)
{
	std::vector<double> distances(80, 1e6);
	std::vector<int> cellIdxs(80, 0);
	for(int i=0; i<12; i++){
		for(int j=0; j<80; j++){
			cellIdxs[j]=j;
			distances[j]=Alg<3>::distance(vertices[i], m_cells[j]->getCentroidW());
		}
		sort(cellIdxs.begin(), cellIdxs.end(),[&distances](int p, int q){
			return distances[p]<distances[q];
		});
		
		cbk(cellIdxs, i);
	}	
}

//----------------------------------------------------------------------

#endif
