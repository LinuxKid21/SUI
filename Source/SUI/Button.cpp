#include <SUI/Button.hpp>
#include <SUI/Utility.hpp>

namespace sui {
    Button::Button() : Widget() {
        mText = sf::Text();
        mText.setString("");
        mHovered = false;
        mClicked = false;
        mRectangleShape = sf::RectangleShape();
    }
    
    void Button::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mRectangleShape, states);
        target.draw(mText, states);
    }
    
    void Button::onInput(sf::Event e) {
        if(e.type == sf::Event::MouseMoved) {
            const auto mousePos = sf::Vector2f(e.mouseMove.x, e.mouseMove.y);
            if(getGlobalBounds().contains(mousePos)) {
                // call only while entering, not constantly while inside
                if(!mHovered) {
                    onEntered();
                }

                mHovered = true;
            } else if(mHovered) {
                onExited();
                mHovered = false;
            }
        } else if(e.type == sf::Event::MouseButtonPressed) {
            const auto mousePos = sf::Vector2f(e.mouseButton.x, e.mouseButton.y);
            if(getGlobalBounds().contains(mousePos)) {
                mClicked = true;
                onClickedDown();
            }
        } else if(e.type == sf::Event::MouseButtonReleased && mClicked) {
            const auto mousePos = sf::Vector2f(e.mouseButton.x, e.mouseButton.y);
            if(getGlobalBounds().contains(mousePos)) {
                mHovered = true;
            } else {
                mHovered = false;
            }
            mClicked = false;
            
            onClickedUp();
        }
        updateFillColors();
    }
    
    
    void Button::onUpdate() {
        mText.setString(getText());
        mRectangleShape.setOutlineColor(getProperty("outlineColor").as<sf::Color>());
        mRectangleShape.setOutlineThickness(-getProperty("outlineThickness").as<float>());
        mText.setFont(*getProperty("font").as<sf::Font *>());
        mText.setColor(getProperty("textColor").as<sf::Color>());
        mText.setCharacterSize(getProperty("fontSize").as<float>());
        updateFillColors();
        
        
        mRectangleShape.setRotation(0); // somewhere rotation was somehow changed sometimes!!! FIXME
        mRectangleShape.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
        mRectangleShape.setSize(getSize());
        updateText();
    }
    
    void Button::updateFillColors() {
        if(mClicked) {
            mRectangleShape.setFillColor(getProperty("fillColorClicked").as<sf::Color>());
        } else if(mHovered) {
            mRectangleShape.setFillColor(getProperty("fillColorHovered").as<sf::Color>());
        } else {
            mRectangleShape.setFillColor(getProperty("fillColor").as<sf::Color>());
        }
    }
    void Button::updateText() {
        ORIGIN textOriginX = getTextOriginX();
        ORIGIN textOriginY = getTextOriginY();
        setTextOrigin(mText, textOriginX, textOriginY, 'X');
        int x;
        int y;
        switch(textOriginX) {
            case ORIGIN_START:
                x = getGlobalBounds().left;
                break;
            case ORIGIN_MIDDLE:
                x = getGlobalBounds().left+getGlobalBounds().width/2;
                break;
            case ORIGIN_END:
                x = getGlobalBounds().left+getGlobalBounds().width;
                break;
            default:
                throw "mTextOriginX is not a valid value!";
        }
        
        
        switch(textOriginY) {
            case ORIGIN_START:
                y = getGlobalBounds().top;
                break;
            case ORIGIN_MIDDLE:
                y = getGlobalBounds().top+getGlobalBounds().height/2;
                break;
            case ORIGIN_END:
                y = getGlobalBounds().top+getGlobalBounds().height;
                break;
            default:
                throw "mTextOriginY is not a valid value!";
        }
        mText.setPosition(x, y);
    }
    
}
