#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include "Structuri.h"
#include "Slidere.h"
#include "Meniu.h"
#include "Circuit.h"
#include "FereastraAvertizare.h"
#include "TransformariComponente.h"
#include "Conexiuni.h"
#include "Ajutor.h"
#include "Setari.h"
#include "Start.h"
#include "MediuLucru.h"
void mouseMoved(Circuit* circuit, Vector2i coordonateMouse)
{
	if (circuit->status == Normal)
	{
		for (int i = 0; i < circuit->nrComponente; i++)
		{
			Punct max, min;
			coordonateDupaTransformari(circuit->elemente[circuit->componente[i].idElement].chenar, &min, &max, circuit->componente[i].transformari.getTransform());
			if (coordonateMouse.x >= min.x && coordonateMouse.y >= min.y
				&& coordonateMouse.x <= max.x && coordonateMouse.y <= max.y)
			{
				circuit->componente[i].activ = true;
			}
			else
			{
				circuit->componente[i].activ = false;
			}

		}
	}
	else if (circuit->status == CreareConexiune)
	{
			circuit->legaturi[circuit->nrLegaturi - 1].punct2 = (Vector2f)coordonateMouse;
	}
	else if (circuit->status == ModificareUnghiRotire)
	{
		if (coordonateMouse.y >= varfUnghiRotire && coordonateMouse.y <= bazaUnghiRotire)
		{
			rotireSlider(circuit, coordonateMouse);
		}
	}
	else if (circuit->status == ModificareCoeficientScalare)
	{
		if (coordonateMouse.y >= varfCoefScalare && coordonateMouse.y <= bazaCoefScalare)
		{
			if (circuit->componentaSelectata != -1)
			{
				redimensionareSlider(circuit, coordonateMouse,circuit->componentaSelectata);				
			}
			else
			{
				redimensionareIntregCircuitSlider(circuit, coordonateMouse);
			}
		}
	}
	else if (circuit->status == InserareElement)
	{
		Vector2f pozitieVeche = circuit->componente[circuit->nrComponente - 1].transformari.getPosition();
		circuit->componente[circuit->nrComponente - 1].transformari.setPosition(coordonateMouse.x, coordonateMouse.y);
		if (verificareColiziune(circuit, circuit->nrComponente - 1) == true)
		{
			circuit->componente[circuit->nrComponente - 1].transformari.setPosition(pozitieVeche);
		}

	}
	else if (circuit->status == ModificarePozitieComponenta)
	{
		miscareComponenta(circuit, coordonateMouse);
	}
}
void mousePressed(Circuit* circuit, Vector2i coordonateMouse, RenderWindow &window, bool butonStangApasat, char numeOptiuniSalvate[][40])
{
	if (circuit->status == Normal)
	{	
		if (coordonateMouse.x >= axaOxSlidere - 10 && coordonateMouse.x <= axaOxSlidere + 10)
		{
			float pozitieYScalare;
			if (circuit->componentaSelectata != -1)
			{
				 pozitieYScalare = pozitieCercCoefScalare(circuit->componente[circuit->componentaSelectata].transformari.getScale().x);
			}
			else
			{
				pozitieYScalare = pozitieCercCoefScalare(circuit->sizeCoef);
			}


			if (coordonateMouse.y >= pozitieYScalare - 10 && coordonateMouse.y <= pozitieYScalare + 10)
			{
				circuit->status = ModificareCoeficientScalare;
				
			}
			else
			{
				float pozitieYRotire = pozitieCercUnghiRotire((*circuit).componente[(*circuit).componentaSelectata].transformari.getRotation());
				if (coordonateMouse.y >= pozitieYRotire - 10 && coordonateMouse.y <= pozitieYRotire + 10)
				{
					if (circuit->componentaSelectata != -1)
					{
						circuit->status = ModificareUnghiRotire;
					}
				}
			}
		}
		else if (coordonateMouse.y >= 0 && coordonateMouse.y <= INALTIME_MENIU_OPTIUNI)
		{
			unsigned int numarOptiune = optiuneMeniuSelectata(coordonateMouse);
			if (numarOptiune != -1)
			{
				char mesaj[MAX];
				bool necesitaSalvat = false;
				switch (numarOptiune)
				{
				case 0:
					strcpy(mesaj, "Circuitul nu e salvat. Sigur vrei sa parasesti mediul de lucru?");
					if (circuit->esteSalvat == true || afisareFereastraAvertizare(mesaj) == true)
					{
						stergereIstoric(circuit);
						window.close();
						afisareFereastraStart();
					}
					break; //meniu principal
				case 1:
					if (circuit->numeFisier[0] != '\0')
					{
						salvareCircuit(circuit, circuit->numeFisier, true);
						circuit->esteSalvat = true;
					}
					else
					{
						char mesaj[] = "Nu este inserat numele fisierului";
						FereastraEroare(mesaj);
					}
					break; //salvare
				case 2:
					circuit->numeFisier[0] = '\0';
					salvareCircuit(circuit, circuit->numeFisier, true);
					circuit->esteSalvat = true;
					break; //salvare ca
				case 3:
					circuit->numeFisier[0] = '\0';
					deschidereCircuit(circuit, circuit->numeFisier, true);

					circuit->topRedo = NULL;
					circuit->topUndo = NULL;
					circuit->esteSalvat = true;
					salvareIstoric(circuit);
					break; //deschidere
				case 4:
					afisareDoarComponente(window, circuit);
					exporta(window);
					break;//export
				case 5:
					strcpy(mesaj, "Circuitul nu e salvat. Sigur vrei sa resetezi circuitul?");
					if (circuit->esteSalvat == true || afisareFereastraAvertizare(mesaj) == true)
					{
						if (circuit->nrComponente != 0)
							necesitaSalvat = true;
						circuit->componente = new Componenta[MAX];
						circuit->legaturi = new Legatura[MAX];
						circuit->nrLegaturi = 0;
						circuit->nrComponente = 0;
						circuit->componentaSelectata = -1;

						if (necesitaSalvat)
							salvareIstoric(circuit);
					}
					break;//resetare

				case 6:
					deschideFereastraAjutor();
					break; //ajutor
				case 7:
					afisareFereastraSetari(circuit, numeOptiuniSalvate);
					break; //setari
				default:
					break;
				}
			}
		}
		else if (coordonateMouse.y >= INALTIME_MENIU_OPTIUNI && coordonateMouse.y <= INALTIME_MENIU_OPTIUNI + INALTIME_MENIU_ELEMENTE) //coordonate meniu elemente
		{
			for (int i = 0; i < 12; i++)
			{
				if (coordonateMouse.x > i * circuit->latimeCasuta + (i + 1) * 10 && coordonateMouse.x < (i + 1) * circuit->latimeCasuta + (i + 1) * 10)
				{
					circuit->status = InserareElement;
					circuit->componente[circuit->nrComponente].idElement = i;
					Transformable t;
					ElCircuit elementCurent = circuit->elemente[circuit->componente[circuit->nrComponente].idElement];
					circuit->componente[circuit->nrComponente].transformari = t;
					circuit->componente[circuit->nrComponente].transformari.setOrigin(elementCurent.origine.x, elementCurent.origine.y);
					circuit->componente[circuit->nrComponente].transformari.setPosition(coordonateMouse.x, coordonateMouse.y);
					strcpy(circuit->componente[circuit->nrComponente].nume, elementCurent.denumire);
					circuit->nrComponente++;
				}
			}
		}
		else if (butonStangApasat && circuit->status != ModificarePozitieComponenta)
		{
			for (int i = 0; i < circuit->nrComponente; i++)
			{
				if (circuit->componente[i].activ)
				{
					circuit->componentaSelectata = i;
					circuit->status = ModificarePozitieComponenta;
					break;

				}
				else
				{
					circuit->componente[i].activ = false;
				}
				if (circuit->status != ModificarePozitieComponenta)
					circuit->componentaSelectata = -1;
			}
		}
		if (circuit->componentaSelectata != -1)
		{
			for (int i = 0; i < circuit->elemente[circuit->componente[circuit->componentaSelectata].idElement].nrLegaturi; i++)
			{
				Vector2f PunctTransfomrat = circuit->componente[circuit->componentaSelectata].transformari.getTransform().transformPoint(circuit->elemente[circuit->componente[circuit->componentaSelectata].idElement].legaturi[i].x,
					circuit->elemente[circuit->componente[circuit->componentaSelectata].idElement].legaturi[i].y);//coordonate punct conexiune dupa transformari

				if (abs(PunctTransfomrat.x - coordonateMouse.x) < 10 && abs(PunctTransfomrat.y - coordonateMouse.y) < 10)
				{
					if (verificareExistentaConexiune(circuit->legaturi, circuit->nrLegaturi, circuit->componentaSelectata, i))
						circuit->nrLegaturi--;

					circuit->legaturi[circuit->nrLegaturi].idComponenta1 = circuit->componentaSelectata;
					circuit->legaturi[circuit->nrLegaturi].idComponenta2 = -1;
					circuit->legaturi[circuit->nrLegaturi].nrPunctComponenta1 = i;
					circuit->legaturi[circuit->nrLegaturi].punct1 = PunctTransfomrat;
					circuit->legaturi[circuit->nrLegaturi].punct2 = PunctTransfomrat;
					circuit->nrLegaturi++;
					circuit->status = CreareConexiune;
				}
			}
		}
	}
	else if (circuit->status == CreareConexiune)
	{
		inserarePunct2Conexiune(circuit, coordonateMouse);
	}
	else if (circuit->status == ModificareCoeficientScalare)
	{
		if (circuit->componentaSelectata != -1)
		{
			redimensionareSlider(circuit, coordonateMouse, circuit->componentaSelectata);
		}
		else
		{
			redimensionareIntregCircuitSlider(circuit, coordonateMouse);
		}
		
	}
	else if (circuit->status == ModificareUnghiRotire)
	{
		rotireSlider(circuit, coordonateMouse);
	}
}
void mouseReleased(Circuit* circuit)
{
	if (circuit->status == InserareElement)
	{
		Vector2f coordonateInafaraFereastra = ieseDinFereastra(circuit->componente[circuit->nrComponente - 1], circuit->elemente[circuit->componente[circuit->nrComponente - 1].idElement]);
		if (!(coordonateInafaraFereastra.x == 0 && coordonateInafaraFereastra.y == 0))
		{
			resetareDateComponenta(&circuit->componente[circuit->nrComponente - 1]);
			circuit->nrComponente--;
		}
		salvareIstoric(circuit);

		circuit->status = Normal;
	}
	else if (circuit->status == ModificarePozitieComponenta)
	{
		salvareIstoric(circuit);
		circuit->status = Normal;
	}
	else if (circuit->status == ModificareCoeficientScalare)
	{
		circuit->status = Normal;
		salvareIstoric(circuit);
	}
	else if (circuit->status == ModificareUnghiRotire)
	{
		circuit->status = Normal;
		salvareIstoric(circuit);
	}
}