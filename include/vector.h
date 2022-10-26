/*********************************************************************
* Vect<D>, Vect<2>, Vect<3>, Rotation classes  								*
* Alg<D> struct                                                      *
*                                                                    *
* Version: 2.0                                                       *
* Date:    20-09-2022                                                *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/

#ifndef VECTOR_CLASS_H
#define VECTOR_CLASS_H

#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <algorithm>

#include "assertion.h"

//README: for value of pi use M_PI which is part of cmath
//const double pi=3.1415926535897; 

//===================================================================

template<int D>
class Vect
{
	public:
		Vect();

		Vect(unsigned int i, double p);

		explicit Vect(unsigned int i);

		Vect(double (&arr)[D]);
		Vect(const Vect<D>& V);
		Vect(std::initializer_list<double> l);
		int get_dimension() const;

		Vect<D>& operator=(const Vect<D>& V);		
		bool operator==(const Vect<D>& v);
		bool operator!=(const Vect<D>& v);
		Vect<D>& operator+=(const Vect<D>& w);
		Vect<D>& operator-=(const Vect<D>& w);
		Vect<D> operator+(const Vect<D>& W);
		Vect<D> operator-(const Vect<D>& W);
		Vect<D>& operator*=(double f);

		static Vect canonical(unsigned int i);

		double operator[](unsigned int i)const;

		// utilities
		double norm() const;
		double quasiNorm() const;
		Vect<D>& canonical_reflexion(unsigned int p);
		Vect<D>& normalize();

		double innerProduct(const Vect<D>& W);
		double angCos(const Vect<D>& W);

		//Orthogonal projection on vector W
		Vect<D> projectTo(const Vect<D>& W);

		void fastOp(double f, const Vect<D>& W);

	private:
		double m_V[D];
};

//===================================================================

template<int D>
Vect<D>::Vect()
{
	for(int i=0; i<D; i++){
		m_V[i]=1.0;
	}
}

//===================================================================

template<int D>
Vect<D>::Vect(unsigned int i)
:Vect(i, 1.0)
{}

//===================================================================

template<int D>
Vect<D>::Vect(unsigned int i, double p)
{
	assertm(i<D, "Dimension does not match");

	std::memset(m_V, 0, D*sizeof(double));
	m_V[i]=1.0*p;
}

//===================================================================

template<int D>
Vect<D>::Vect(double (&arr)[D])
{
	std::memcpy(m_V, arr, D*sizeof(double));
}

//===================================================================

template<int D>
Vect<D> Vect<D>::canonical(unsigned int i)
{
	Vect<D> result;
	for(int j=0; j<D; j++){
		if(j!=i){
			result.m_V[i]=0;
		}
	}
	return result;
}

//===================================================================

template<int D>
Vect<D>::Vect(std::initializer_list<double> l)
:Vect()
{
	int i=0;
	for(double x : l){
		m_V[i++]=x;
		if(i==D){
			break;
		}
	}
}

//===================================================================

template<int D>
Vect<D>::Vect(const Vect<D>& W)
{
	std::memcpy(m_V, W.m_V, D*sizeof(double));
}

//===================================================================

template<int D>
int Vect<D>::get_dimension() const
{
	return D;
}

//===================================================================

template<int D>
Vect<D>& Vect<D>::operator=(const Vect<D>& W)
{
	std::memcpy(m_V, W.m_V, D*sizeof(double));
	return *this;
}

//===================================================================

template<int D>
bool Vect<D>::operator==(const Vect<D>& W)
{
	if(this==&W){
		return true;
	}
	
	for(int i=0; i<D; i++){
		if(m_V[i]!=W.m_V[i]){
			return false;
		}
	}
	
	return true;
}
//===================================================================

template<int D>
bool Vect<D>::operator!=(const Vect<D>& W)
{
	return !(*this==W);
}

//===================================================================

template<int D>
Vect<D>& Vect<D>::operator+=(const Vect<D>& W)
{	
	for(int i=0; i<D; i++){	
		m_V[i]=m_V[i]+W.m_V[i];		
	}
	return *this;
}

//===================================================================

template<int D>
Vect<D>& Vect<D>::operator-=(const Vect<D>& W)
{
	for(int i=0; i<D; i++){
		//m_V[i]-=W.m_V[i];		
		m_V[i]=m_V[i]-W.m_V[i];		
	}
	return *this;
}

//===================================================================

template<int D>
Vect<D> Vect<D>::operator+(const Vect<D>& W)
{ 
	Vect<D> V(*this);
	V+=W;
	
	return V;
}

//===================================================================

template<int D>
Vect<D> Vect<D>::operator-(const Vect<D>& W)
{
	Vect<D> V(*this);
	V-=W;
	return V;
}

//===================================================================

template<int D>
Vect<D>& Vect<D>::operator*=(double f)
{
	for(int i=0; i<D; i++){
		//m_V[i]*=f;		
		m_V[i]=m_V[i]*f;		
	}
	return *this;
}

//===================================================================

template<int D>
inline double Vect<D>::operator[](unsigned int i)const 
{
	return m_V[i];
}

//===================================================================

template<int D>
double Vect<D>::quasiNorm() const
{
	double sum=0;
	for(int i=0; i<D; i++){
		sum+=m_V[i]*m_V[i];
	}

	return sum;
}

//===================================================================

template<int D>
double Vect<D>::norm() const
{
	return sqrt(quasiNorm());
}

//===================================================================

template<int D>
inline Vect<D>& Vect<D>::normalize()
{
	(*this)*=1/norm();
	return *this;
}

//===================================================================
/*
This is a reflexion with respect to the hyperplane orthogonal to the p-th
element of the canonical base.
*/

template<int D>
Vect<D>& Vect<D>::canonical_reflexion(unsigned int p)
{
	assertm(p<D, "Dimension does not match in Vect<D>::canonical_reflexion"); // assertion fails
	m_V[p]*=-1;

	return *this;
}

//===================================================================

template<int D>
void Vect<D>::fastOp(double f, const Vect<D>& W)
{
	for(int i=0; i<D; i++){
		m_V[i]=m_V[i]+f*W.m_V[i];
	}	
}

//===================================================================

template<int D>
double Vect<D>::innerProduct(const Vect<D>& W)
{
	double dprod=0;
	for(int i=0; i<D; i++){
		dprod+=m_V[i]*W.m_V[i];
	}
	return dprod;
}

//===================================================================

template<int D>
double Vect<D>::angCos(const Vect<D>& W)
{
	return innerProduct(W)/(norm()*W.norm());
}

//===================================================================

template<int D>
inline Vect<D> Vect<D>::projectTo(const Vect<D>& W)
{
	Vect<D> U=W;
	U.normalize();
	double r=angCos(W)*this->norm();
	U*=r;
	return U;
}

//===================================================================
//###################################################################
//===================================================================

template<>
class Vect<2>
{
	public:
		Vect();
		Vect(unsigned int i, double p);
		explicit Vect(unsigned int i);

		Vect(double (&arr)[2]);
		Vect(const Vect<2>& V);
		Vect(std::initializer_list<double> l);
		int get_dimension() const;

		Vect<2>& operator=(const Vect<2>& V);		
		bool operator==(const Vect<2>& v);
		bool operator!=(const Vect<2>& v);
		Vect<2>& operator+=(const Vect<2>& w);
		Vect<2>& operator-=(const Vect<2>& w);
		Vect<2> operator+(const Vect<2>& W);
		Vect<2> operator-(const Vect<2>& W);
		Vect<2>& operator*=(double f);

		static Vect canonical(unsigned int i);

		double operator[](unsigned int i)const;

		// utilities
		double norm() const;
		double quasiNorm() const;
		Vect<2>& canonical_reflexion(unsigned int p);
		Vect<2>& normalize();

		double innerProduct(const Vect<2>& W);
		double angCos(const Vect<2>& W);

		//Orthogonal projection on vector W
		Vect<2> projectTo(const Vect<2>& W);

		void fastOp(double f, const Vect<2>& W);

	private:
		double m_V[2];
};

//===================================================================

inline Vect<2>::Vect()
{
	m_V[0]=1.0;
	m_V[1]=1.0;
}

//===================================================================

inline Vect<2>::Vect(unsigned int i)
:Vect(i, 1.0)
{}

//===================================================================

inline Vect<2>::Vect(unsigned int i, double p)
{
	assert(i<2);
	
	m_V[0]=0.0;
	m_V[1]=0.0;
	m_V[i]=1.0*p;
}

//===================================================================

inline Vect<2>::Vect(double (&arr)[2])
{
	m_V[0]=arr[0];
	m_V[1]=arr[1];
}

//===================================================================

inline Vect<2> Vect<2>::canonical(unsigned int i)
{
	assert(i<2);
	double result[]={0.0, 0.0};
	result[i]=1.0;
	
	return result;
}

//===================================================================

inline Vect<2>::Vect(std::initializer_list<double> l)
:Vect()
{
	int i=0;
	for(double x : l){
		m_V[i++]=x;
		if(i==2){
			break;
		}
	}
}

//===================================================================

inline Vect<2>::Vect(const Vect<2>& W)
{
	m_V[0]=W.m_V[0];
	m_V[1]=W.m_V[1];
}

//===================================================================

inline int Vect<2>::get_dimension() const
{
	return 2;
}

//===================================================================

inline Vect<2>& Vect<2>::operator=(const Vect<2>& W)
{
	m_V[0]=W.m_V[0];
	m_V[1]=W.m_V[1];
	return *this;
}

//===================================================================

inline bool Vect<2>::operator==(const Vect<2>& W)
{
	if(this==&W){
		return true;
	}
	return m_V[0]==W.m_V[0] && m_V[1]==W.m_V[1];
}
//===================================================================

inline bool Vect<2>::operator!=(const Vect<2>& W)
{
	return !(*this==W);
}

//===================================================================

inline Vect<2>& Vect<2>::operator+=(const Vect<2>& W)
{
	m_V[0]=m_V[0]+W.m_V[0];	
	m_V[1]=m_V[1]+W.m_V[1];	
	
	return *this;
}

//===================================================================

inline Vect<2>& Vect<2>::operator-=(const Vect<2>& W)
{
	m_V[0]=m_V[0]-W.m_V[0];	
	m_V[1]=m_V[1]-W.m_V[1];	
	return *this;
}

//===================================================================

inline Vect<2> Vect<2>::operator+(const Vect<2>& W)
{ 
	Vect<2> V(*this);
	V+=W;
	
	return V;
}

//===================================================================

inline Vect<2> Vect<2>::operator-(const Vect<2>& W)
{	
	Vect<2> V(*this);
	V-=W;
	return V;
}

//===================================================================

inline Vect<2>& Vect<2>::operator*=(double f)
{
	m_V[0]=m_V[0]*f;	
	m_V[1]=m_V[1]*f;	

	return *this;
}

//===================================================================

inline double Vect<2>::operator[](unsigned int i) const 
{
	assert(i<2);
	return m_V[i];
}

//===================================================================

inline double Vect<2>::quasiNorm() const
{
	double sum=m_V[0]*m_V[0];
	sum+=m_V[1]*m_V[1];

	return sum;
}

//===================================================================

inline double Vect<2>::norm() const
{
	return sqrt(quasiNorm());
}

//===================================================================

inline Vect<2>& Vect<2>::normalize()
{
	(*this)*=1/norm();
	return *this;
}

//===================================================================
/*
This is a reflexion with respect to the hyperplane orthogonal to the p-th
element of the canonical base.
*/

inline Vect<2>& Vect<2>::canonical_reflexion(unsigned int p)
{
	assert(p<2);
	m_V[p]*=-1.0;
	return *this;
}


//===================================================================

inline void Vect<2>::fastOp(double f, const Vect<2>& W)
{
	m_V[0]=m_V[0]+f*W.m_V[0];
	m_V[1]=m_V[1]+f*W.m_V[1];
}

//===================================================================

inline double Vect<2>::innerProduct(const Vect<2>& W)
{
	double dprod=m_V[0]*W.m_V[0];
	dprod=dprod+m_V[1]*W[1];

	return dprod;
}

//===================================================================

inline double Vect<2>::angCos(const Vect<2>& W)
{
	return innerProduct(W)/(norm()*W.norm());
}

//===================================================================

inline Vect<2> Vect<2>::projectTo(const Vect<2>& W)
{
	Vect<2> U=W;
	U.normalize();
	double r=angCos(W)*this->norm();
	U*=r;
	return U;
}

//===================================================================

inline std::ostream& operator<<(std::ostream& out, const Vect<2>& v)
{
	out<<'('<<v[0]<<", "<<v[1]<<')';
	return out;
}

//===================================================================
//###################################################################
//===================================================================

template<>
class Vect<3>
{
	public:
		Vect();
		Vect(unsigned int i, double p);
		explicit Vect(unsigned int i);

		Vect(double (&arr)[3]);
		Vect(const Vect<3>& V);
		Vect(std::initializer_list<double> l);
		int get_dimension() const;

		Vect<3>& operator=(const Vect<3>& V);		
		bool operator==(const Vect<3>& v);
		bool operator!=(const Vect<3>& v);
		Vect<3>& operator+=(const Vect<3>& w);
		Vect<3>& operator-=(const Vect<3>& w);
		Vect<3> operator+(const Vect<3>& W);
		Vect<3> operator-(const Vect<3>& W);
		Vect<3>& operator*=(double f);

		static Vect canonical(unsigned int i);

		double operator[](unsigned int i)const;

		// utilities
		double norm() const;
		double quasiNorm() const;
		Vect<3>& canonical_reflexion(unsigned int p);
		Vect<3>& normalize();

		double innerProduct(const Vect<3>& W);
		double angCos(const Vect<3>& W);

		//Orthogonal projection on vector W
		Vect<3> projectTo(const Vect<3>& W);
		void fastOp(double f, const Vect<3>& W);

	private:
		double m_V[3];
};

//===================================================================

inline Vect<3>::Vect()
{
	m_V[0]=1.0;
	m_V[1]=1.0;
	m_V[2]=1.0;
}

//===================================================================

inline Vect<3>::Vect(unsigned int i)
:Vect(i, 1.0)
{}

//===================================================================

inline Vect<3>::Vect(unsigned int i, double p)
{
	assert(i<3);
	
	m_V[0]=0.0;
	m_V[1]=0.0;
	m_V[2]=0.0;
	m_V[i]=1.0*p;

}

//===================================================================

inline Vect<3>::Vect(double (&arr)[3])
{
	m_V[0]=arr[0];
	m_V[1]=arr[1];
	m_V[2]=arr[2];
}

//===================================================================

inline Vect<3> Vect<3>::canonical(unsigned int i)
{
	assert(i<3);
	double result[]={0.0, 0.0, 0.0};
	result[i]=1.0;
	
	return result;
}

//===================================================================

inline Vect<3>::Vect(std::initializer_list<double> l)
:Vect()
{
	int i=0;
	for(double x : l){
		m_V[i++]=x;
		if(i==3){
			break;
		}
	}
}

//===================================================================

inline Vect<3>::Vect(const Vect<3>& W)
{
	m_V[0]=W.m_V[0];
	m_V[1]=W.m_V[1];
	m_V[2]=W.m_V[2];
}

//===================================================================

inline int Vect<3>::get_dimension() const
{
	return 3;
}

//===================================================================

inline Vect<3>& Vect<3>::operator=(const Vect<3>& W)
{
	m_V[0]=W.m_V[0];
	m_V[1]=W.m_V[1];
	m_V[2]=W.m_V[2];
	return *this;
}

//===================================================================

inline bool Vect<3>::operator==(const Vect<3>& W)
{
	if(this==&W){
		return true;
	}
	return m_V[0]==W.m_V[0] && m_V[1]==W.m_V[1] && m_V[2]==W.m_V[2];
}

//===================================================================

inline bool Vect<3>::operator!=(const Vect<3>& W)
{
	return !(*this==W);
}

//===================================================================

inline Vect<3>& Vect<3>::operator+=(const Vect<3>& W)
{
	m_V[0]=m_V[0]+W.m_V[0];	
	m_V[1]=m_V[1]+W.m_V[1];	
	m_V[2]=m_V[2]+W.m_V[2];	
	
	return *this;
}

//===================================================================

inline Vect<3>& Vect<3>::operator-=(const Vect<3>& W)
{
	m_V[0]=m_V[0]-W.m_V[0];	
	m_V[1]=m_V[1]-W.m_V[1];
	m_V[2]=m_V[2]-W.m_V[2];

	return *this;
}

//===================================================================

inline Vect<3> Vect<3>::operator+(const Vect<3>& W)
{ 
	Vect<3> V(*this);
	V+=W;
	
	return V;
}

//===================================================================

inline Vect<3> Vect<3>::operator-(const Vect<3>& W)
{	
	Vect<3> V(*this);
	V-=W;

	return V;
}

//===================================================================

inline Vect<3>& Vect<3>::operator*=(double f)
{
	m_V[0]=m_V[0]*f;	
	m_V[1]=m_V[1]*f;	
	m_V[2]=m_V[2]*f;	

	return *this;
}

//===================================================================

inline double Vect<3>::operator[](unsigned int i) const 
{
	assert(i<3);
	return m_V[i];
}

//===================================================================

inline double Vect<3>::quasiNorm() const
{
	double sum=m_V[0]*m_V[0];
	sum+=m_V[1]*m_V[1];
	sum+=m_V[2]*m_V[2];

	return sum;
}

//===================================================================

inline double Vect<3>::norm() const
{
	return sqrt(quasiNorm());
}

//===================================================================

inline Vect<3>& Vect<3>::normalize()
{
	(*this)*=1/norm();
	return *this;
}

//===================================================================
/*
This is a reflexion with respect to the hyperplane orthogonal to the p-th
element of the canonical base.
*/

inline Vect<3>& Vect<3>::canonical_reflexion(unsigned int p)
{
	assert(p<3);
	m_V[p]*=-1.0;
	return *this;
}

//===================================================================

inline void Vect<3>::fastOp(double f, const Vect<3>& W)
{
	m_V[0]=m_V[0]+f*W.m_V[0];
	m_V[1]=m_V[1]+f*W.m_V[1];
	m_V[2]=m_V[2]+f*W.m_V[2];
}

//===================================================================

inline double Vect<3>::innerProduct(const Vect<3>& W)
{
	double dprod=m_V[0]*W.m_V[0];
	dprod=dprod+m_V[1]*W.m_V[1];
	dprod=dprod+m_V[2]*W.m_V[2];

	return dprod;
}

//===================================================================

inline double Vect<3>::angCos(const Vect<3>& W)
{
	return innerProduct(W)/(norm()*W.norm());
}

//===================================================================

inline Vect<3> Vect<3>::projectTo(const Vect<3>& W)
{
	Vect<3> U(W);
	U.normalize();
	double r=angCos(W)*this->norm();
	U*=r;
	return U;
}

//===================================================================

inline std::ostream& operator<<(std::ostream& out, const Vect<3>& v)
{
	out<<'('<<v[0]<<", "<<v[1]<<", "<<v[2]<<')';
	return out;
}

//===================================================================
//###################################################################
//===================================================================

template<int D>
struct Alg
{
	static double innerProduct(const Vect<D>& V, const Vect<D>& W)
	{
		double dprod=0;
		for(int i=0; i<D; i++){
			dprod+=V[i]*W[i];
		}
		return dprod;
	}

	static double distance(const Vect<D>& V, const Vect<D>& W)
	{
		double sum=0;
		for(int i=0; i<D; i++){
			sum+=pow(V[i]-W[i], 2);
		}
		return sqrt(sum);
	};

	static Vect<D> getCentroid0(const Vect<D>& V, const Vect<D>& W)
	{
		Vect<D> centroid(V);
		centroid+=W;
		return centroid;
	}
};

template<>
struct Alg<2>
{
	static double innerProduct(const Vect<2>& V, const Vect<2>& W)
	{
		double dprod=V[0]*W[0];
		dprod+=V[1]*W[1];
		return dprod;
	}

	static double distance(const Vect<2>& V, const Vect<2>& W)
	{
		double sum=pow(V[0]-W[0], 2);
		sum=sum+pow(V[1]-W[1], 2);
		return sqrt(sum);
	}
};

template<>
struct Alg<3>
{
	static double innerProduct(const Vect<3>& V, const Vect<3>& W)
	{
		double dprod=V[0]*W[0];
		dprod+=V[1]*W[1];
		dprod+=V[2]*W[2];
		return dprod;
	}

	static double distance(const Vect<3>& V, const Vect<3>& W)
	{
		double sum=pow(V[0]-W[0], 2);
		sum=sum+pow(V[1]-W[1], 2);
		sum=sum+pow(V[2]-W[2], 2);
		return sqrt(sum);
	}
};

//===================================================================
//###################################################################
//===================================================================

template<int D>
Vect<D> operator*(double f, const Vect<D>& V)
{
	Vect<D> W(V);
	return W*=f;
}

//===================================================================

template<int D>
Vect<D> operator*(const Vect<D>& V, double f)
{
	return f*V;
} 

//===================================================================

template<int D>
Vect<D> operator/(const Vect<D>& V, double f)
{
	return (1.0/f)*V;
}
 
//===================================================================

template<int D>
Vect<D> operator+(const Vect<D>& U, const Vect<D>& V)
{
	Vect<D> W(U);
	W+=V;
	return W;
}

//===================================================================

template<int D>
Vect<D> operator-(const Vect<D>& U, const Vect<D>& V)
{
	Vect<D> W(U);
	W-=V;
	return W;
}

//===================================================================

template<int D>
std::ostream& operator<<(std::ostream& out, const Vect<D>& v)
{
	out<<'(';
	for(int i=0; i<D; i++){
		out<<v[i];
		if(i<D-1){
			out<<", ";
		}
	}
	out<<')';
	return out;
}

//===================================================================

/*
* Cosine of between V and the (D-1)-canonical vector
* 
*/
template<int D>
double fast_cos(const Vect<D>& V)
{
	return V[D-1]/V.norm();
}

//===================================================================

template<int D>
double angCos(const Vect<D>& V, const Vect<D>& W)
{
	return Alg<D>::innerProduct(V, W)/(V.norm()*W.norm());
}

//===================================================================

template<int D>
Vect<D-1> projection(const Vect<D>& V, unsigned int p)
{
	double w[D];
	int j=0;

	for(int i=0; i<D; i++){
		if(i==p){
			continue;
		}
		w[j++]=V[i];
	}

	return w;
}
//===================================================================

inline Vect<3> crossProduct(const Vect<3>& V, const Vect<3>& W)
{
	Vect<3> U({V[1]*W[2]-V[2]*W[1], V[2]*W[0]-V[0]*W[2], V[0]*W[1]-V[1]*W[0]});
	return U;
}

//===================================================================

template<int D>
inline Vect<D> getCentroid(const Vect<D>& V, const Vect<D>& W)
{
	Vect<D> centroid=V;
	centroid+=W;
	centroid*=0.5;
	return centroid;
}

//===================================================================
//###################################################################
//=================================================================== 

template<int D>
class Rotation
{
	public:		
		Rotation()=delete;

		//line of rotation is the line through origin 
		//determined by the vector V, angle
		Rotation(const Vect<D>& V, double angle);

		// line of rotation is the line determined by the vectors 
		// vector V and W, angle
		Rotation(const Vect<D>& V, const Vect<D>& W, double angle);
		Rotation(const Rotation<D>& Rt);
		double getRotationAngle() const;

		void rotate(Vect<D>& V, const Vect<D>& OffV) const;
		void rotate(Vect<D>& V) const;
		void revRotate(Vect<D>& V, const Vect<D>& OffV) const;
		void revRotate(Vect<D>& V) const;
		Vect<D> reverse(const Vect<D>& V);

	private:
		Vect<D> m_p1;
		Vect<D> m_p2;
		Vect<D> m_offset;
		double m_rad;
		double m_COS;
		double m_t;

		void setup(const Vect<D>& rotation_axe);
		double set_angle(double angle);
};

//======================================================================

template<int D>
Rotation<D>::Rotation(const Rotation<D>& Rt)
:m_p1(Rt.m_p1), 
m_p2(Rt.m_p2), 
m_offset(Rt.m_offset),
m_rad(Rt.m_rad), m_COS(Rt.m_COS)
{}

//======================================================================

template<int D>
Rotation<D>::Rotation(const Vect<D>& rotation_axe, double angle)
:m_offset(0, 0), m_rad(set_angle(angle)), m_COS(cos(m_rad))
{
	setup(rotation_axe);
}

//======================================================================

template<int D>
Rotation<D>::Rotation(const Vect<D>& V, const Vect<D>& W, double angle)
:m_offset(W), m_rad(set_angle(angle)), m_COS(cos(m_rad))
{
	setup(V-m_offset);
}

//======================================================================

template<int D>
double Rotation<D>::set_angle(double angle)
{
	if(fabs(angle)>=360.0){
		angle=(fabs(angle)/angle)*(fabs(angle)-360.0);
	}
	return -1.0*((angle/2)*M_PI)/180;
}


//======================================================================

template<int D>
inline double Rotation<D>::getRotationAngle() const
{
	return m_rad;
}

//======================================================================

template<int D>
void Rotation<D>::setup(const Vect<D>& rotationAxe)
{
	/*
	First we get a canonical solution to the equation axe dot v=0.
	This is everything zero except 2 for example
	axe =(a,b,c,d);
	v=(0,c,-b,0); where b!=0 || c!=0
	*/
	
	//std::cout<<rotationAxe<<"<<--\n";
	
	double a[D]={0, 0};
	double b[D]={1, 1};
	double s[D]={1, 1};
	int P[2]={0,1};
	// we get two non zero values
	if(D>2){
		for(int i=2; i<D; i++){
			a[i]=0;
			b[i]=1;
			s[i]=1;
			if(fabs(rotationAxe[P[0]])+fabs(rotationAxe[P[1]])==0){
				if(rotationAxe[i]!=0.0){
					P[0]=i-1;
					P[1]=i;
				}
			}
		}
		
		a[P[0]]=rotationAxe[P[1]];
		a[P[1]]=-1.0*rotationAxe[P[0]];
		Vect<D> p(a);

		b[P[0]]=0;
		b[P[1]]=0;
		Vect<D> B(b);

	   /*
		Now, we use p to construct an orthogonal vector W 
		WE CAN USE the cross product!! 
		NOOO, of course NOT!!! 
		we need something easier... and fast!
		*/
		double r=rotationAxe[P[0]]*p[P[1]]-rotationAxe[P[1]]*p[P[0]];
		s[P[0]]=(-1.0*Alg<D>::innerProduct(B, rotationAxe)*p[P[1]])/r;
		s[P[1]]=(Alg<D>::innerProduct(B, rotationAxe)*p[P[0]])/r;
		Vect<D> W(s);
		m_p1=p.normalize();
		Vect<D> vp=W.normalize();
		m_p2=sin(m_rad)*vp+cos(m_rad)*m_p1;
	}
	else{		
		m_p1=Vect<D>{1, 0};
		Vect<D> w{0, 1};
		m_p2=sin(m_rad)*w+cos(m_rad)*m_p1;
	}
}

//===================================================================

template<int D>
Vect<D> Rotation<D>::reverse(const Vect<D>& V)
{
	Vect<D> W(V);
	W-=m_offset;
	double t=2.0*Alg<D>::innerProduct(W, m_p1);
	return V-t*m_p1+(t*2.0*m_COS-2.0*Alg<D>::innerProduct(W, m_p2))*m_p2;
}
 
//======================================================================

template<int D>
void Rotation<D>::rotate(Vect<D>& V, const Vect<D>& OffV) const 
{
	Vect<D> W(V);
	W-=OffV;
	double t=2.0*Alg<D>::innerProduct(W, m_p2);

	V.fastOp(-1.0*t, m_p2);
	V.fastOp(2.0*(t*m_COS-Alg<D>::innerProduct(W, m_p1)), m_p1);
}

//===================================================================

template<int D>
void Rotation<D>::rotate(Vect<D>& V) const
{
	rotate(V, m_offset);
}

//===================================================================

template<int D>
void Rotation<D>::revRotate(Vect<D>& V, const Vect<D>& OffV) const
{
	Vect<D> W(V);
	W-=OffV;
	double t=2.0*Alg<D>::innerProduct(W, m_p1);

	V.fastOp(-1.0*t, m_p1);
	V.fastOp(2.0*(t*m_COS-Alg<D>::innerProduct(W, m_p2)), m_p2);
}

//===================================================================

template<int D>
void Rotation<D>::revRotate(Vect<D>& V) const 
{
	revRotate(V, m_offset);
}

//======================================================================

template<int D>
Vect<D> mkRotation(const Vect<D>& V, const Vect<D>& W){
	Vect<D> rotationAxe=crossProduct(V, W);

	double ang=(std::acos(ang_cos(V, W))*180.0)/M_PI;

	return Rotation<D>(rotationAxe, ang);
}

//======================================================================

#endif
