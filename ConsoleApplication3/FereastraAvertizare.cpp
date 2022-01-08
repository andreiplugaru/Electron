
#include "Structuri.h"
#include "Defineuri.h"
bool verificareInteriorButonAvertizare(RectangleShape patrat, Vector2i coordonateMouse)
{
    return (coordonateMouse.x >= patrat.getGlobalBounds().left && coordonateMouse.x <= patrat.getGlobalBounds().left + patrat.getGlobalBounds().width
        && coordonateMouse.y >= patrat.getGlobalBounds().top && coordonateMouse.y <= patrat.getGlobalBounds().top + patrat.getGlobalBounds().height);
}
bool afisareFereastraAvertizare()
{
    sf::Font font;
    font.loadFromFile("fonts\\RobotoSlab-Font.ttf");
    Image image;
    image.loadFromFile("icons\\warning.png");
    RenderWindow window(sf::VideoMode(800, 150, 32), "Avertizare", sf::Style::Close);
    window.setFramerateLimit(30);
    window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());

    Text valoareButon;
    valoareButon.setString("Da");
    valoareButon.setFillColor(sf::Color::White);
    valoareButon.setCharacterSize(35);
    valoareButon.setFont(font);

    Vector2f marimeButon;
    marimeButon.x = valoareButon.getGlobalBounds().width + 2 * OFFSET;
    marimeButon.y = 50;
  
   
    Text text;
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(25);
    text.setFont(font);
    text.setString("Circuitul nu e salvat. Sigur vrei sa parasesti mediul de lucru?");
    Vector2f pozitieButon;
    pozitieButon.y = text.getGlobalBounds().height + 2 * OFFSET;
    pozitieButon.x = OFFSET;
    
    RectangleShape butonDa;
    butonDa.setSize(marimeButon);
    butonDa.setFillColor(Color::Transparent);
    butonDa.setOutlineColor(Color::White);
    butonDa.setOutlineThickness(3);


    RectangleShape butonNu;
    butonNu.setSize(marimeButon);
    butonNu.setFillColor(Color::Transparent);
    butonNu.setOutlineColor(Color::White);
    butonNu.setOutlineThickness(3);

  //  pozitieButon.x = butonDa.getSize().x + OFFSET;
 
    while (window.isOpen()) {
        sf::Event event;
        text.setPosition(OFFSET, OFFSET);
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                Vector2i coordonateMouse = Mouse::getPosition(window);
                if (verificareInteriorButonAvertizare(butonDa, coordonateMouse))
                {
                    butonDa.setOutlineColor(Color::Blue);
                    butonNu.setOutlineColor(Color::White);
                }
                else  if (verificareInteriorButonAvertizare(butonNu, coordonateMouse))
                {
                    butonNu.setOutlineColor(Color::Blue);
                    butonDa.setOutlineColor(Color::White);
                }
                else
                {
                    butonNu.setOutlineColor(Color::White);
                    butonDa.setOutlineColor(Color::White);
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                Vector2i coordonateMouse = Mouse::getPosition(window);
                if (verificareInteriorButonAvertizare(butonDa, coordonateMouse))
                {
                    return true;
                }
                else  if (verificareInteriorButonAvertizare(butonNu, coordonateMouse))
                {
                    return false;
                }
               
            }

        }
        window.clear(sf::Color::Black);
        Vector2f pozitieButon;
        Vector2f pozitieText;
        valoareButon.setString("Da");
     //   text.setString("Da");
        window.draw(text);
        pozitieButon.x = OFFSET;
        pozitieButon.y = text.getGlobalBounds().top + text.getGlobalBounds().height + OFFSET;
        butonDa.setPosition(pozitieButon);
        pozitieText = pozitieButon;
        pozitieText.x += OFFSET;
        valoareButon.setPosition(pozitieText);
        window.draw(butonDa);
        window.draw(valoareButon);

        pozitieButon.x += marimeButon.x + OFFSET;
        butonNu.setPosition(pozitieButon);
        valoareButon.setString("Nu");
        pozitieText = pozitieButon;
        pozitieText.x += OFFSET;
        valoareButon.setPosition(pozitieText);
        window.draw(butonNu);
        window.draw(valoareButon);
      
        window.display();
    }

    return true;
}