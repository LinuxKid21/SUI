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
    
    void Button::onPositionChanged() {
        mRectangleShape.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
        updateText();
    }
    void Button::onSizeChanged() {
        mRectangleShape.setSize(getSize());
        updateText();
    }
    void Button::onPropertyChanged(const std::string key) {
        if(key == "outlineColor") {
            mRectangleShape.setOutlineColor(getProperty(key).as<sf::Color>());
        } else if(key == "font") {
            mText.setFont(*getProperty(key).as<sf::Font *>());
            updateText();
        } else if(key == "textColor") {
            mText.setColor(getProperty(key).as<sf::Color>());
        } else if(key == "fontSize") {
            mText.setCharacterSize(getProperty(key).as<float>());
        } else if(key == "outlineThickness") {
            mRectangleShape.setOutlineThickness(-getProperty(key).as<float>());
        } else if(key == "text") {
            mText.setString(getProperty(key).as<sf::String>());
            updateText();
        } else if(key == "textOriginX") {
            updateText();
        } else if(key == "textOriginY") {
            updateText();
        } else if(key == "fillColorClicked") {
            norm_color = getProperty(key).as<sf::Color>();
            updateFillColors();
        } else if(key == "fillColorHovered") {
            hover_color = getProperty(key).as<sf::Color>();
            updateFillColors();
        } else if(key == "fillColor") {
            click_color = getProperty(key).as<sf::Color>();
            updateFillColors();
        }
    }
    
    void Button::updateFillColors() {
        if(mClicked) {
            mRectangleShape.setFillColor(norm_color);
        } else if(mHovered) {
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
