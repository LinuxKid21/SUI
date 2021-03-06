#include <SUI/Button.hpp>
#include <SUI/Utility.hpp>

namespace sui {
    Button::Button() : Widget() {
        mText = sf::Text();
        mText.setString("");
        mRectangleShape = sf::RectangleShape();
    }
    
    void Button::onUpdate(const bool posChanged, const bool sizeChanged) {
        Widget::onUpdate(posChanged, sizeChanged);
        bool shouldUpdateText = false;
        
        if(sizeChanged) {
            mRectangleShape.setSize(getSize());
        }
        if(posChanged) {
            mRectangleShape.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
        }
        
        if(hasPropChanged("outlineColor")) {
            mRectangleShape.setOutlineColor(getProperty("outlineColor").as<sf::Color>());
        }
        if(hasPropChanged("font")) {
            mText.setFont(*getProperty("font").as<sf::Font *>());
            shouldUpdateText = true;
        }
        if(hasPropChanged("text")) {
            mText.setString(getProperty("text").as<sf::String>());
            shouldUpdateText = true;
        }
        
        bool shouldUpdateColor = hasPropChanged("hovering") || hasPropChanged("clicking");
        if(hasPropChanged("textColor")) {
            mText.setColor(getProperty("textColor").as<sf::Color>());
        }
        if(hasPropChanged("fontSize")) {
            mText.setCharacterSize(getProperty("fontSize").as<float>());
            shouldUpdateText = true;
        }
        if(hasPropChanged("outlineThickness")) {
            mRectangleShape.setOutlineThickness(-getProperty("outlineThickness").as<float>());
        }
        if(hasPropChanged("fillColorClicked")) {
            norm_color = getProperty("fillColorClicked").as<sf::Color>();
            shouldUpdateColor = true;
        }
        if(hasPropChanged("fillColorHovered")) {
            hover_color = getProperty("fillColorHovered").as<sf::Color>();
            shouldUpdateColor = true;
        }
        if(hasPropChanged("fillColor")) {
            click_color = getProperty("fillColor").as<sf::Color>();
            shouldUpdateColor = true;
        }
        if(shouldUpdateColor) {
            updateFillColors();
        }
        if(shouldUpdateText || sizeChanged || posChanged || hasPropChanged("textOriginX") || hasPropChanged("textOriginY")) {
            updateText();
        }
    }
    
    void Button::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mRectangleShape, states);
        target.draw(mText, states);
    }
    
    void Button::updateFillColors() {
        if(getProperty("clicking").as<bool>()) {
            mRectangleShape.setFillColor(norm_color);
        } else if(getProperty("hovering").as<bool>()) {
            mRectangleShape.setFillColor(hover_color);
        } else {
            mRectangleShape.setFillColor(click_color);
        }
    }
    void Button::updateText() {
        if(!getProperty("font")) return;
        setTextOrigin(mText, getTextOriginX(), getTextOriginY(), 'X');
        int x;
        int y;
        switch(getTextOriginX()) {
            case ORIGIN_START:
                x = getGlobalBounds().left + getTextPadding();
                break;
            case ORIGIN_MIDDLE:
                x = getGlobalBounds().left+getGlobalBounds().width/2;
                break;
            case ORIGIN_END:
                x = getGlobalBounds().left+getGlobalBounds().width - getTextPadding();
                break;
            default:
                throw "mTextOriginX is not a valid value!";
        }
        
        
        switch(getTextOriginY()) {
            case ORIGIN_START:
                y = getGlobalBounds().top + getTextPadding();
                break;
            case ORIGIN_MIDDLE:
                y = getGlobalBounds().top+getGlobalBounds().height/2;
                break;
            case ORIGIN_END:
                y = getGlobalBounds().top+getGlobalBounds().height - getTextPadding();
                break;
            default:
                throw "mTextOriginY is not a valid value!";
        }
        mText.setPosition(x, y);
    }
    
}
