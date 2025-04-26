/*********************************************************************
* Vect<D>, Vect<2>, Vect<3>, Rotation classes  								*
* Alg<D> struct                                                      *
*                                                                    *
* Version: 2.5                                                       *
* Date:    20-09-2022  (Reviewed 04/2025)                            *
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
	std::memset(m_V, 0, D*sizeof(double));
}

//===================================================================

template<int D>
Vect<D>::Vect(unsigned int i)
:Vect(i, 1.0)
{}

//===================================================================

template<int D>
Vect<D>::Vect(unsigned int i, double p)
:Vect()
{
	assertm(i<D, "Dimension does not match");
	m_V[i]=p;
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
	return Vect<D>(i, 1);
}

//===================================================================

template<int D>
Vect<D>::Vect(std::initializer_list<double> l)
:Vect()
{
	assertm(l.size()==D, "Dimension does not match");
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

	return std::memcmp(m_V, W.m_V, D*sizeof(double))==0;
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
	assert(i<D);
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
	(*this)*=1.0/norm();
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
	double w[D-1];
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
//===================================================================
//###################################################################
//=================================================================== 
//===================================================================

inline double deg2rad(double degs)
{
	if(std::fabs(degs)>=360.0){
		degs=(std::fabs(degs)/degs)*(std::fabs(degs)-360.0);
	}
	return -1.0*((degs/2)*M_PI)/180.0;
}

//===================================================================
//===================================================================
//===================================================================

template<int D>
class Rotation
{
	public:		
		Rotation()=delete;

		/*
		 * line of rotation is the line through origin and the point V, angle
		 * 
		 */
		Rotation(const Vect<D>& V, double angle);

		/*
		 * line of rotation is the line determined by the vectors 
		 * vector V and W, angle. 
		*/
		Rotation(const Vect<D>& V, const Vect<D>& W, double angle);

		Rotation(const Rotation<D>& Rt);

		Rotation<D>& operator=(const Rotation<D>& Rt); 

		void operator()(Vect<D>& V) const;
		void operator()(Vect<D>& V, const Vect<D>& offset) const;	

void rotate(Vect<D>& V)const;
//void rotate(Vect<D>& V, const Vect<D>& OffV);
void revRotate(Vect<D>& V) const;


		double getRotationAngle() const;

	private:
		Vect<D> m_offset;
		Vect<D> m_cols[D];
		Vect<D> m_colsR[D];
		double m_rad;

		void setCols(const Vect<D>& rotation_axe);
};

//======================================================================

template<int D>
Rotation<D>::Rotation(const Vect<D>& rotation_axe, double angle)
:m_offset(0, 0)
, m_rad(deg2rad(angle))
{
	setCols(rotation_axe);
}

//======================================================================

/*
 * line of rotation is the line determined by the vectors 
 * vector V and W, and angle angle.
 * Notice that the actuall rotation is done via the line that 
 * passes though the origin and the point V-V  and with m_offset=W.
 * 
 * More clearly: to rotate a point around the line determined by
 * the points V and W. The point P is traslate by offset (P-m_offset), do the rotation 
 * and then translate and then Rt(P)+m_offset
 * 
 * Rt(P-W) + W
*/
template<int D>
Rotation<D>::Rotation(const Vect<D>& V, const Vect<D>& W, double angle)
:m_offset(W)
, m_rad(deg2rad(angle))
{
	setCols(V-m_offset);
}

//======================================================================

template<int D>
Rotation<D>::Rotation(const Rotation<D>& Rt)
: m_offset(Rt.m_offset)
, m_rad(Rt.m_rad)
{
	for(int i=0; i<D; i++){
		m_cols[i]=Rt.m_cols[i];
		m_colsR[i]=Rt.m_colsR[i];
	}
}

//===================================================================

template<int D>
Rotation<D>& Rotation<D>::operator=(const Rotation<D>& Rt)
{
	m_offset=Rt.m_offset;
	m_rad=Rt.m_rad;

	for(int i=0; i<D; i++){
		m_cols[i]=Rt.m_cols[i];
		m_colsR[i]=Rt.m_colsR[i];
	}

	return *this;
}

//======================================================================

template<int D>
inline double Rotation<D>::getRotationAngle() const
{
	return m_rad;
}

//======================================================================

template<int D>
void Rotation<D>::setCols(const Vect<D>& rotationAxe)
{
	/*
	First we get a canonical solution to the equation axe dot v=0.
	This is everything zero except 2 for example
	axe =(a,b,c,d);
	v=(0,c,-b,0); where b!=0 || c!=0
	*/
	Vect<D> mp1;
	Vect<D> mp2;

	const double COS=cos(m_rad);
	const double SIN=sin(m_rad);

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
			if(fabs(rotationAxe[P[0]])+fabs(rotationAxe[P[1]])==0.0){
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
		We do the flexion on the two planes define by...
		*/
		double r=1.0/(rotationAxe[P[0]]*p[P[1]]-rotationAxe[P[1]]*p[P[0]]);
		s[P[0]]=(-1.0*Alg<D>::innerProduct(B, rotationAxe)*p[P[1]])*r;
		s[P[1]]=(Alg<D>::innerProduct(B, rotationAxe)*p[P[0]])*r;
		Vect<D> W(s);
		mp1=p.normalize();
		Vect<D> vp=W.normalize();
		mp2=SIN*vp+COS*mp1;
	}
	else{		
		mp1=Vect<D>{1, 0};
		Vect<D> w{0, 1};
		mp2=SIN*w+COS*mp1;
		m_offset=rotationAxe;
	}

	for(int i=0; i<D; i++){
		m_cols[i]=Vect<D>(i);
		double t=2.0*mp2[i];

		m_cols[i].fastOp(-1.0*t, mp2);
		m_cols[i].fastOp(2.0*(t*COS-mp1[i]), mp1);
	}

	for(int i=0; i<D; i++){
		m_colsR[i]=Vect<D>(i);
		double t=2.0*mp1[i];

		m_colsR[i].fastOp(-1.0*t, mp1);
		m_colsR[i].fastOp(2.0*(t*COS-mp2[i]), mp2);
		
	}
}

//===================================================================

template<int D>
void Rotation<D>::operator()(Vect<D>& V) const
{
	Vect<D> W(m_offset);
	for(int i=0; i<D; i++){
		W+=((V[i]-m_offset[i])*m_cols[i]);
	}
	V=W;
}

//===================================================================

template<int D>
void Rotation<D>::operator()(Vect<D>& V, const Vect<D>& offset) const
{
	V-=offset;
	(*this)(V);
	V+=offset;
}

//===================================================================

template<int D>
void Rotation<D>::revRotate(Vect<D>& V) const
{
	Vect<D> W(m_offset);
	for(int i=0; i<D; i++){
		W+=((V[i]-m_offset[i])*m_colsR[i]);
	}
	V=W;
}

template<int D>
void Rotation<D>::rotate(Vect<D>& V) const
{
	(*this)(V);
}

//===================================================================

#endif
