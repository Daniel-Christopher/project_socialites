#ifndef COLOR_H
#define COLOR_H

#include "Arduino.h"

#define REDPIN 3
#define GREENPIN 5
#define BLUEPIN 6
class color{
	//char colorNum;
	public:
	int red, green, blue;

	char colorNum;
    color(char Color);
	void pulse(int rate);
	color operator++(int fuck);

};
#endif