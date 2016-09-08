#include <SUI/SUI.hpp>
int main() {
    sui::initiateSUI();
    const int window_width = 700;
    const int window_height = 300;

    sui::Theme theme;
    theme.loadDefaults("arial.ttf");

    const unsigned int xCells = 4;
    const unsigned int yCells = 3;

    sui::GridLayout *grid = new sui::GridLayout(theme);
    grid->setOrigin(sui::ORIGIN_MIDDLE, sui::ORIGIN_MIDDLE);
    grid->setPosition(sf::Vector2f(window_width/2, window_height/2));
    grid->setSize(sf::Vector2f(400,200));
    grid->setCells(xCells,yCells);
    
    for(unsigned int x = 0;x < xCells; x++) {
        for(unsigned int y = 0;y < yCells; y++) {
            sui::Button *button = new sui::Button(theme);
            button->setText(std::to_string(x) + std::string(", ") + std::to_string(y));
            button->setTextAlign(sui::ORIGIN_MIDDLE, sui::ORIGIN_MIDDLE);
            grid->addChild(button, x, y);
        }
    }
    grid->layoutChanged();

    sf::RenderWindow window(sf::VideoMode(window_width, window_height, 32), "SUI demo", sf::Style::Default);

    while (window.isOpen()) {
    	sf::Event event;
    	while (window.pollEvent(event)) {
    		if (event.type == sf::Event::Closed)
    			window.close();
            grid->handleInput(event);
    	}
    	window.clear();
        window.draw(*grid);
    	window.display();
    }
    delete grid;
}
