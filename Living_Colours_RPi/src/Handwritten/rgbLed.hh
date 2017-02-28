#ifndef RGBLED_HH
#define RGBLED_HH

#include <softPwm.h>

#define RED_PIN       27
#define GREEN_PIN     28
#define BLUE_PIN      29

void rgb_SetRed();
void rgb_SetGreen();
void rgb_SetBlue();
void rgb_Dim();
void rgb_SetSpecificRed(int newValue);
void rgb_SetSpecificGreen(int newValue);
void rgb_SetSpecificBlue(int newValue);
void rgb_SetColour(Colour colour);

#endif //RGBLED_HH
