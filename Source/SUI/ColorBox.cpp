#include <SUI/ColorBox.hpp>


namespace sui {
    ColorBox::ColorBox() : Widget() {
        mRectangleShape = sf::RectangleShape();
    }
    
    void ColorBox::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mRectangleShape, states);
    }
    void ColorBox::onInput(sf::Event e) {
        return;
    }
    void ColorBox::onUpdate() {
        mRectangleShape.setFillColor(getProperty("fillColor").as<sf::Color>());
        mRectangleShape.setPosition(getGlobalBounds().left, getGlobalBounds().top);
        mRectangleShape.setSize(getSize());
    }
}


