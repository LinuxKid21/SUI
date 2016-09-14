#include <SUI/SUI.hpp>
int main() {
    const int window_width = 700;
    const int window_height = 300;

    const int xCells = 4;
    const int yCells = 3;

    sui::GridLayout *grid = new sui::GridLayout();
    sf::Font font = sf::Font();
    font.loadFromFile("arial.ttf");
    grid->setProperty("font", sui::Property::make(&font));
    grid->setProperty("outlineThickness", sui::Property::make(4.f));
    grid->setProperty("fillColor", sui::Property::make(sf::Color(255,255,255,255)));
    grid->setProperty("fillColorHovered", sui::Property::make(sf::Color(200,200,200,255)));
    grid->setProperty("fillColorClicked", sui::Property::make(sf::Color(100,100,100,255)));
    grid->setProperty("textColor", sui::Property::make(sf::Color(0,0,0,255)));
    grid->setProperty("outlineColor", sui::Property::make(sf::Color(0,255,0,255)));
    grid->setProperty("fontSize", sui::Property::make(24.f));
    grid->setProperty("textAlignX", sui::Property::make(sui::ORIGIN_MIDDLE));
    grid->setProperty("textAlignY", sui::Property::make(sui::ORIGIN_MIDDLE));
    grid->setProperty("padding", sui::Property::make(5.f));
    grid->setProperty("xCells", sui::Property::make(xCells));
    grid->setProperty("yCells", sui::Property::make(yCells));
    
    grid->setOrigin(sui::ORIGIN_MIDDLE, sui::ORIGIN_MIDDLE);
    grid->setPosition(sf::Vector2f(window_width/2, window_height/2));
    grid->setSize(sf::Vector2f(400,200));
    
    for(unsigned int x = 0;x < xCells/2; x++) {
        for(unsigned int y = 0;y < yCells; y++) {
            sui::Button *button = new sui::Button();
            button->setProperty("text", sui::Property::make<sf::String>(std::to_string(x) + ", " + std::to_string(y)));
            button->setProperty<int>("gridPosX", x*2);
            button->setProperty<int>("gridPosY", y);
            button->setProperty("fillColor", sf::Color(255,255,255,255));
            button->setProperty("fillColorHovered", sf::Color(200,200,200,255));
            button->setProperty("fillColorClicked", sf::Color(100,100,100,255));
            button->setProperty("onClickedDown", sui::Property::makeFunc([x, y, button](){
                int x_pos = button->getProperty("gridPosX").as<int>();
                if(x_pos % 2 == 1) {
                    button->setProperty<int>("gridPosX", x_pos-1);
                } else {
                    button->setProperty<int>("gridPosX", x_pos+1);
                }
                std::cout << "(" << x << ", " << y << ")\n";
            }));
            grid->addChild(button);
        }
    }

    sf::RenderWindow window(sf::VideoMode(window_width, window_height, 32), "SUI demo", sf::Style::Default);
    window.setFramerateLimit(30);

    while (window.isOpen()) {
    	sf::Event event;
    	while (window.pollEvent(event)) {
    		if (event.type == sf::Event::Closed)
    			window.close();
            grid->handleInput(event);
    	}
    	window.clear();
        grid->update();
        window.draw(*grid);
    	window.display();
    }
    delete grid;
}
