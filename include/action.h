/*********************************************************************
* Action class                               								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022  (Reviewed 04/2025)                            *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef ACTION_H
#define ACTION_H

#include <assert.h>

class ModuloI;

class Action
{
	public:
		virtual ~Action()=default;
		virtual ModuloI* move(ModuloI* modulo, int direction)=0;
};

//======================================================================

template<int N>
class ActionT : public Action
{
	public:
		virtual ~ActionT()=default;
		virtual ModuloI* move(ModuloI* modulo, int direction);

	protected:
		static int m_degree;
		ModuloI* m_base;
		ModuloI* m_modules[N];	
};

//----------------------------------------------------------------------

template<int N>
int ActionT<N>::m_degree=N;

template<int N>
ModuloI* ActionT<N>::move(ModuloI* modulo, int direction)
{
	if(modulo==m_base){
		return m_base;
	}

	int j=direction;	
	for(int i=0; i<m_degree; i++){
		if(modulo==m_modules[i]){
			j=i+direction;
			if(j==m_degree){
				j=0;
			}
			else if(j<0){
				j=m_degree-1;
			}
			return m_modules[j];					
		}
	}

	return modulo;
}

//======================================================================
//######################################################################
//======================================================================
/*
 * The order in which modulos m0,..., m3 are input reflect 
 * sequecial order for the action
*/

class Action4 : public ActionT<4>
{
	public:
		Action4(ModuloI* base, ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3);
		virtual ~Action4()=default;
};

//----------------------------------------------------------------------

inline Action4::Action4(ModuloI* base, ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3)
{
	assert(base && m0 && m1 && m2 && m3);
	m_base=base;
	m_modules[0]=m0;
	m_modules[1]=m1;
	m_modules[2]=m2;
	m_modules[3]=m3;
}

class Action2 : public ActionT<2>
{
	public:
		Action2(ModuloI* base, ModuloI* m0, ModuloI* m1);
		virtual ~Action2()=default;
};

//----------------------------------------------------------------------

inline Action2::Action2(ModuloI* base, ModuloI* m0, ModuloI* m1)
{
	assert(base && m0 && m1);
	m_base=base;
	m_modules[0]=m0;
	m_modules[1]=m1;
}

//====================================================================
//====================================================================

class Action5 : public ActionT<5>
{
	public:
		Action5(ModuloI* base, ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3, ModuloI* m4);
		virtual ~Action5()=default;
};

//----------------------------------------------------------------------

inline Action5::Action5(ModuloI* base, ModuloI* m0, ModuloI* m1, ModuloI* m2, ModuloI* m3, ModuloI* m4)
{
	assert(base && m0 && m1 && m2 && m3 && m4);
	m_base=base;
	m_modules[0]=m0;
	m_modules[1]=m1;
	m_modules[2]=m2;
	m_modules[3]=m3;
	m_modules[4]=m4;
}

//----------------------------------------------------------------------

#endif
