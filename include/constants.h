#ifndef HELPER_H
#define HELPER_H

#include <cmath>
//#include <iostream>
//----------------------------------------------------------------------

const int WINDOW_WIDTH=1000;
const int WINDOW_HEIGHT=800;

const double D_WINDOW_WIDTH=WINDOW_WIDTH*1.0;
const double D_WINDOW_HEIGHT=WINDOW_HEIGHT*1.0;

const double PX_CM=37.795238;

const double SL=300.0;

const double DELTA_ROTATION=2.0;

const double DELTA_ANGLE=5.0;

const double phi=0.5*(1+std::sqrt(5));
const double scale=1.7;

static const char* DOGIC_FILE{".dogic_session"};
static const char* DOGIC12_FILE{".dogic12_session"};
static const char* RUBIK_FILE{".rubik_session"};

char const FontLSR[]="LiberationSerif-Regular.ttf";
char const FontLMR[]="LiberationMono-Regular.ttf";

//----------------------------------------------------------------------

#endif
