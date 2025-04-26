/*********************************************************************
* FrameRate class                              								*
*         	                                                         *
* Version: 1.0                                                       *
* Date:    26-09-2022  (Reviewed 04/2025)                            *
* Author:  Dan Machado                                               *                                         *
**********************************************************************/
#ifndef FRAMERATE_H
#define FRAMERATE_H
#include <SFML/System/Time.hpp>

class FrameRate
{
   public:
		FrameRate(int fps);
		float operator()();
		const double& tpf();
		double reset();

   private:
		sf::Clock clock;
		double TPF; // Time Per Frame
		unsigned int FPS;    // Frame Per Seconds
      
};

//-------------------------------------------------------------------

inline FrameRate::FrameRate(int fps)
:clock(), TPF(1.0/fps), FPS(fps)
{}

//-------------------------------------------------------------------

inline float FrameRate::operator()()
{
   return clock.getElapsedTime().asSeconds()-TPF;
}

//-------------------------------------------------------------------

inline const double& FrameRate::tpf()
{
   return TPF;
}

//-------------------------------------------------------------------

inline double FrameRate::reset()
{
   return clock.restart().asSeconds();
}


//-------------------------------------------------------------------

#endif
