#include <SUI/ColorBox.hpp>


namespace sui {
    ColorBox::ColorBox(Theme &theme) : Widget(theme) {
        mRectangleShape = sf::RectangleShape();
        setColor(sf::Color(0,0,0,255));
    }
    
    void ColorBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mRectangleShape, states);
    }
    bool ColorBox::handleInput(sf::Event e) {
        return false;
    }
    void ColorBox::layoutChanged() {
        mRectangleShape.setPosition(getGlobalBounds().left, getGlobalBounds().top);
        mRectangleShape.setSize(getSize());
    }
    
    void ColorBox::setColor(sf::Color color) {
        mRectangleShape.setFillColor(color);
    }
}


