#pragma 
#include <SFML/Graphics.hpp>
#include <iostream>
#define varfCoefScalare 640
#define bazaCoefScalare 840
#define varfUnghiRotire 310
#define bazaUnghiRotire 510
#define axaOxSlidere 1850
using namespace sf;
   
void drawCoefScalareBara(RenderWindow& window);

void drawUnghiRotireBara(RenderWindow& window);

float valoareUnghiRotire(Vector2i coordMouse);

float valoareCoefScalare(Vector2i coordMouse);

void drawSlider(RenderWindow& window, float pozitieCerc);

void checkSizeCoef(Vector2f& sizecoef);

float pozitieCercCoefScalare(float coefScalare);

float  pozitieCercUnghiRotire(float unghiRotire);
