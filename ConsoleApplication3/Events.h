#pragma once
#include <SFML/Graphics.hpp>
void mouseMoved(Circuit* circuit, Vector2i coordonateMouse);
void mousePressed(Circuit* circuit, Vector2i coordonateMouse, RenderWindow &window, bool butonStangApasat, char numeOptiuniSalvate[][40]);
void mouseReleased(Circuit* circuit);