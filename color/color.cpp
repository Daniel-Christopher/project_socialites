#include <color.h>

color::color(char color){
	pinMode(REDPIN, OUTPUT);
  	pinMode(GREENPIN, OUTPUT);
  	pinMode(BLUEPIN, OUTPUT);
	colorNum = color;
	red = 0;
	green = 0;
	blue = 0;
	switch (color){
		case '0': red = 255; green = 0; blue = 0; break;//red
		case '1': red = 255; green = 10; blue = 0; break;//orange
		case '2': red = 255; green = 50; blue = 0; break;//yellow
		case '3': red = 0; green = 255; blue = 0; break;//green
		case '4': red = 240; green = 255; blue = 255; break;//cyan
		case '5': red = 0; green = 0; blue = 255; break;//blue
		case '6': red = 255; green = 20; blue = 147; break;//violet
		case'7': red = 255; green = 255; blue = 255; break;//white
		default: break;   
	}
	   
}

void color::pulse(int rate){
	for (int fadeValue = 245; fadeValue >= 0 ; fadeValue -= 5){
		analogWrite(REDPIN, map(red-fadeValue, red-255, red, 0, red));
		analogWrite(GREENPIN, map(green-fadeValue, green-255, green, 0, green));
		analogWrite(BLUEPIN, map(blue-fadeValue, blue-255, blue, 0, blue));	
		delay(rate);	
	}
	for (int fadeValue = 0; fadeValue <= 245; fadeValue += 5){
		analogWrite(REDPIN, map(red-fadeValue, red-255, red, 0, red));
		analogWrite(GREENPIN, map(green-fadeValue, green-255, green, 0, green));
		analogWrite(BLUEPIN, map(blue-fadeValue, blue-255, blue, 0, blue));
		delay(rate);		
	}
}

color color::operator++(int fuck){
	if (colorNum >= '6'){
		colorNum = '6';
		return color(colorNum);
	}else{
		colorNum = colorNum+1;
		return color(colorNum);
	}
}