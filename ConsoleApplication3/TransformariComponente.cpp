#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Structuri.h"
#include "Conexiuni.h"
#include "Slidere.h"

void coordonateDupaTransformari(RectangleShape patrat, Punct* Min, Punct* Max, Transform tp)
{
	Vector2f p1, p2, p3, p4;
	FloatRect patrat2;

	patrat2 = tp.transformRect(patrat.getGlobalBounds());
	p1.x = patrat2.left;
	p1.y = patrat2.top;

	p2.x = patrat2.left + patrat2.width;
	p2.y = patrat2.top;

	p3.x = patrat2.left + patrat2.width;
	p3.y = patrat2.top + patrat2.height;

	p4.x = patrat2.left;
	p4.y = patrat2.top + patrat2.height;

	(*Max).x = p1.x;

	if (p2.x > (*Max).x)
	{
		(*Max).x = p2.x;
	}
	if (p3.x > (*Max).x)
	{
		(*Max).x = p3.x;
	}
	if (p4.x > (*Max).x)
	{
		(*Max).x = p4.x;
	}

	(*Min).x = p1.x;
	if (p2.x < (*Min).x)
	{
		(*Min).x = p2.x;
	}
	if (p3.x < (*Min).x)
	{
		(*Min).x = p3.x;
	}
	if (p4.x < (*Min).x)
	{
		(*Min).x = p4.x;
	}
	(*Max).y = p1.y;
	if (p2.y > (*Max).y)
	{
		(*Max).y = p2.y;
	}
	if (p3.y > (*Max).y)
	{
		(*Max).y = p3.y;
	}
	if (p4.y > (*Max).y)
	{
		(*Max).y = p4.y;
	}

	(*Min).y = p1.y;
	if (p2.y < (*Min).y)
	{
		(*Min).y = p2.y;
	}
	if (p3.y < (*Min).y)
	{
		(*Min).y = p3.y;
	}
	if (p4.y < (*Min).y)
	{
		(*Min).y = p4.y;
	}
}
Vector2f ieseDinFereastra(Componenta comp, ElCircuit element)
{
	Punct max, min;
	coordonateDupaTransformari(element.chenar, &min, &max, comp.transformari.getTransform());

	Vector2f coordonateInAfara;

	if (min.y < INALTIME_MENIU_ELEMENTE + INALTIME_MENIU_OPTIUNI)
	{
		coordonateInAfara.y = INALTIME_MENIU_ELEMENTE + INALTIME_MENIU_OPTIUNI - min.y;
	}
	if (min.x < 0)
	{
		coordonateInAfara.x = 0 - min.x;
	}
	if (max.y > INALTIME_FEREASTRA)
	{
		coordonateInAfara.y = INALTIME_FEREASTRA - max.y;
	}
	if (max.x > LATIME_FEREASTRA - 140)
	{
		coordonateInAfara.x = LATIME_FEREASTRA - max.x - 140;
	}
	return coordonateInAfara;
}
bool verificareColiziune(Circuit* circuit, int idComponentaVerificata)
{
	ElCircuit elementVerificat = circuit->elemente[circuit->componente[idComponentaVerificata].idElement];
	for (int i = 0; i < circuit->nrComponente; i++)
	{
		if (idComponentaVerificata != i)
		{
			ElCircuit elementCurent = circuit->elemente[circuit->componente[i].idElement];
			if (circuit->componente[i].transformari.getTransform().transformRect(elementCurent.chenar.getGlobalBounds()).intersects(
				circuit->componente[idComponentaVerificata].transformari.getTransform().transformRect(elementVerificat.chenar.getGlobalBounds())))
			{
				return true;
			}
		}
	}
	return false;
}
void rotire(Circuit* circuit)
{
	Transform transformPunctConexiune;
	circuit->componente[circuit->componentaSelectata].transformari.rotate(circuit->optiuni.unghiRotire);
	if (verificareColiziune(circuit, circuit->componentaSelectata) == false)
	{
		transformPunctConexiune.rotate(circuit->optiuni.unghiRotire, circuit->componente[circuit->componentaSelectata].transformari.getTransform().transformPoint(circuit->elemente[circuit->componente[circuit->componentaSelectata].idElement].origine));
		Vector2f coordonateInafaraFereastra = ieseDinFereastra(circuit->componente[circuit->componentaSelectata], circuit->elemente[circuit->componente[circuit->componentaSelectata].idElement]);
		if (coordonateInafaraFereastra.x == 0 && coordonateInafaraFereastra.y == 0)
		{
			modificareConexiuni(coordonateInafaraFereastra, circuit, transformPunctConexiune, circuit->componentaSelectata);
		}
		else
			circuit->componente[circuit->componentaSelectata].transformari.rotate(-circuit->optiuni.unghiRotire);
	}
	else
		circuit->componente[circuit->componentaSelectata].transformari.rotate(-circuit->optiuni.unghiRotire);
}
void rotireSlider(Circuit* circuit, Vector2i coordonateMouse)
{
	float valunghirotire;
	float valunghirotirevechi = circuit->componente[circuit->componentaSelectata].transformari.getRotation();
	valunghirotire = valoareUnghiRotire(coordonateMouse);
	if (valunghirotire >= 350)
	{
		valunghirotire = 359;
	}
	else if (valunghirotire <= 10)
	{
		valunghirotire = 0;
	}

	Transform transformPunctConexiune;
	circuit->componente[circuit->componentaSelectata].transformari.setRotation(valunghirotire);
	if (verificareColiziune(circuit, circuit->componentaSelectata) == false)
	{
		transformPunctConexiune.rotate(valunghirotire - valunghirotirevechi, circuit->componente[circuit->componentaSelectata].transformari.getTransform().transformPoint(circuit->elemente[circuit->componente[circuit->componentaSelectata].idElement].origine));
		Vector2f coordonateInafaraFereastra = ieseDinFereastra(circuit->componente[circuit->componentaSelectata], circuit->elemente[circuit->componente[circuit->componentaSelectata].idElement]);
		if (coordonateInafaraFereastra.x == 0 && coordonateInafaraFereastra.y == 0)
		{
			modificareConexiuni(coordonateInafaraFereastra, circuit, transformPunctConexiune, circuit->componentaSelectata);
		}
		else
		{
			circuit->componente[circuit->componentaSelectata].transformari.setRotation(valunghirotirevechi);
		}
	}
	else
	{
		circuit->componente[circuit->componentaSelectata].transformari.setRotation(valunghirotirevechi);
	}
}
void redimensionare(Circuit* circuit, float coeficient, unsigned idComponenta)
{
	float coefInitial = circuit->componente[idComponenta].transformari.getScale().x;
	if ((coeficient < 1 && coefInitial > 0.5) || (coeficient > 1 && coefInitial < 2))
	{
		Transform transformPunctConexiune;
		circuit->componente[idComponenta].transformari.scale(coeficient, coeficient);
		if (verificareColiziune(circuit, idComponenta) == false)
		{
			ElCircuit elementCurent = circuit->elemente[circuit->componente[idComponenta].idElement];
			transformPunctConexiune.scale(coeficient, coeficient,
				circuit->componente[idComponenta].transformari.getTransform().transformPoint(elementCurent.origine).x,
				circuit->componente[idComponenta].transformari.getTransform().transformPoint(elementCurent.origine).y);

			Vector2f coordonateInafaraFereastra = ieseDinFereastra(circuit->componente[idComponenta], elementCurent);
			if (coordonateInafaraFereastra.x == 0 && coordonateInafaraFereastra.y == 0)
			{
				modificareConexiuni(coordonateInafaraFereastra, circuit, transformPunctConexiune, idComponenta);
			}
			else
			{
				circuit->componente[idComponenta].transformari.scale(1 / coeficient, 1 / coeficient);
			}
		}
		else
		{
			circuit->componente[idComponenta].transformari.scale(1 / coeficient, 1 / coeficient);
		}
	}
}
void redimensionareSlider(Circuit* circuit, Vector2i coordonateMouse, unsigned idComponenta)
{
	float coeficient = valoareCoefScalare(coordonateMouse);
	Vector2f coeficientvechi = circuit->componente[idComponenta].transformari.getScale();
	Transform transformPunctConexiune;
	circuit->componente[idComponenta].transformari.scale(coeficient / coeficientvechi.x, coeficient / coeficientvechi.y);
	if (verificareColiziune(circuit, idComponenta) == false)
	{
		ElCircuit elementCurent = circuit->elemente[circuit->componente[idComponenta].idElement];
		transformPunctConexiune.scale(coeficient / coeficientvechi.x, coeficient / coeficientvechi.y,
			circuit->componente[idComponenta].transformari.getTransform().transformPoint(elementCurent.origine).x,
			circuit->componente[idComponenta].transformari.getTransform().transformPoint(elementCurent.origine).y);

		Vector2f coordonateInafaraFereastra = ieseDinFereastra(circuit->componente[idComponenta], elementCurent);
		if (coordonateInafaraFereastra.x == 0 && coordonateInafaraFereastra.y == 0)
		{
			modificareConexiuni(coordonateInafaraFereastra, circuit, transformPunctConexiune, idComponenta);
		}
		else
		{
			circuit->componente[idComponenta].transformari.scale(coeficientvechi.x / coeficient, coeficientvechi.x / coeficient);
		}
	}
	else
	{
		circuit->componente[idComponenta].transformari.scale(coeficientvechi.x / coeficient, coeficientvechi.x / coeficient);
	}
}
VertexArray scalareLinii(VertexArray linii, float coef)
{
	VertexArray liniiModificate(Lines);
	for (int i = 0; i < linii.getVertexCount(); i++)
	{
		Vertex mod;
		mod.position.x = linii[i].position.x * coef;
		mod.position.y = linii[i].position.y * coef;
		liniiModificate.append(mod);
	}
	return liniiModificate;
}
void miscareTastatura(Circuit* circuit, float offsetx, float offsety)
{
	Transformable transformPunctConexiune;
	circuit->componente[circuit->componentaSelectata].transformari.move(offsetx, offsety);
	if (verificareColiziune(circuit, circuit->componentaSelectata) == false)
	{
		transformPunctConexiune.move(offsetx, offsety);
		Vector2f coordonateInafaraFereastra = ieseDinFereastra(circuit->componente[circuit->componentaSelectata], circuit->elemente[circuit->componente[circuit->componentaSelectata].idElement]);
		if (abs(coordonateInafaraFereastra.x) < circuit->optiuni.distantaMiscare && abs(coordonateInafaraFereastra.y) < circuit->optiuni.distantaMiscare)
			modificareConexiuni(coordonateInafaraFereastra, circuit, transformPunctConexiune.getTransform(), circuit->componentaSelectata);

		circuit->componente[circuit->componentaSelectata].transformari.move(coordonateInafaraFereastra);
	}
	else
	{
		circuit->componente[circuit->componentaSelectata].transformari.move(-offsetx, -offsety);
	}
}
void miscareComponenta(Circuit* circuit, Vector2i coordonateMouse)
{
	unsigned idComponenta = circuit->componentaSelectata;
	Vector2f pozitieVeche = circuit->componente[idComponenta].transformari.getPosition();
	circuit->componente[idComponenta].transformari.setPosition((Vector2f)coordonateMouse);
	if (verificareColiziune(circuit, idComponenta) == false)
	{
		Vector2f coordonateInafaraFereastra = ieseDinFereastra(circuit->componente[idComponenta], circuit->elemente[circuit->componente[idComponenta].idElement]);

		circuit->componente[idComponenta].transformari.move(coordonateInafaraFereastra);

		for (int j = 0; j < circuit->nrLegaturi; j++)
		{
			if (circuit->legaturi[j].idComponenta1 == idComponenta)
			{
				Vector2f PunctTransfomrat = circuit->componente[idComponenta].transformari.getTransform().transformPoint(circuit->elemente[circuit->componente[idComponenta].idElement].legaturi[circuit->legaturi[j].nrPunctComponenta1]);
				circuit->legaturi[j].punct1 = PunctTransfomrat;
				
			}
			else if (circuit->legaturi[j].idComponenta2 == idComponenta)
			{
				Vector2f PunctTransfomrat = circuit->componente[idComponenta].transformari.getTransform().transformPoint(circuit->elemente[circuit->componente[idComponenta].idElement].legaturi[circuit->legaturi[j].nrPunctComponenta2]);
				circuit->legaturi[j].punct2 = PunctTransfomrat;
			}

		}
	}
	else
	{
		circuit->componente[idComponenta].transformari.setPosition(pozitieVeche);
	}


}
void redimensionareIntregCircuit(Circuit* circuit, float coeficient)
{
	for (int i = 0; i < circuit->nrComponente; i++)
	{
		redimensionare(circuit, coeficient, i);
	}
	circuit->sizeCoef = coeficient;
}
void redimensionareIntregCircuitSlider(Circuit* circuit, Vector2i coordMouse)
{
	for (int i = 0; i < circuit->nrComponente; i++)
	{
		redimensionareSlider(circuit, coordMouse, i);
	}
	circuit->sizeCoef = valoareCoefScalare(coordMouse);
}