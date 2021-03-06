#include <SUI/ColorBox.hpp>


namespace sui {
    ColorBox::ColorBox() : Widget() {
        mRectangleShape = sf::RectangleShape();
    }
    
    void ColorBox::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mRectangleShape, states);
    }
    void ColorBox::onUpdate(const bool posChanged, const bool sizeChanged) {
        Widget::onUpdate(posChanged, sizeChanged);
        if(hasPropChanged("fillColor")) {
            mRectangleShape.setFillColor(getProperty("fillColor").as<sf::Color>());
        }
        if(posChanged) {
            mRectangleShape.setPosition(getGlobalTopLeftCorner());
        }
        if(sizeChanged) {
            mRectangleShape.setSize(getSize());
        }
    }
}


