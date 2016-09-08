#include <SUI/Button.hpp>
#include <SUI/Utility.hpp>

namespace sui {
    Button::Button(Theme &theme) : Widget(theme) {
        mText = sf::Text();
        mText.setString("");
        mHovered = false;
        mClicked = false;
        mTextOriginX = ORIGIN_START;
        mTextOriginY = ORIGIN_START;
        mRectangleShape = sf::RectangleShape();
        updateTheme();
    }
    
    void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(mRectangleShape, states);
        target.draw(mText, states);
    }
    
    bool Button::handleInput(sf::Event e) {
        bool r = false;
        if(e.type == sf::Event::MouseMoved) {
            const auto mousePos = sf::Vector2f(e.mouseMove.x, e.mouseMove.y);
            if(getGlobalBounds().contains(mousePos)) {
                // call only while entering, not constantly while inside
                if(mOnEntered && !mHovered) {
                    mOnEntered();
                }

                mHovered = true;
                r = true;
            } else if(mHovered) {
                if(mOnExited) {
                    mOnExited();
                }
                mHovered = false;
            }
            
            // if clicking then I think all move signals should be stolen
            if(mClicked) {
                r = true;
            }
        } else if(e.type == sf::Event::MouseButtonPressed) {
            const auto mousePos = sf::Vector2f(e.mouseButton.x, e.mouseButton.y);
            if(getGlobalBounds().contains(mousePos)) {
                mClicked = true;
                r = true;
                if(mOnClickedDown) {
                    mOnClickedDown();
                }
            }
        } else if(e.type == sf::Event::MouseButtonReleased && mClicked) {
            const auto mousePos = sf::Vector2f(e.mouseButton.x, e.mouseButton.y);
            if(getGlobalBounds().contains(mousePos)) {
                mHovered = true;
            } else {
                mHovered = false;
            }
            mClicked = false;
            
            if(mOnClickedUp) {
                mOnClickedUp();
            }
        }
        updateFillColors();
        return r;
    }
    void Button::setText(sf::String str) {
        mText.setString(str);
        updateText();
    }
    
    void Button::setOnEntered(std::function<void()> func) {
        mOnEntered = func;
    }
    void Button::setOnExited(std::function<void()> func) {
        mOnExited = func;
    }
    void Button::setOnClickedDown(std::function<void()> func) {
        mOnClickedDown = func;
    }
    void Button::setOnClickedUp(std::function<void()> func) {
        mOnClickedUp = func;
    }
    
    void Button::updateTheme() {
        mRectangleShape.setOutlineColor(getColorProperty("outlineColor"));
        mRectangleShape.setOutlineThickness(getNumberProperty("outlineThickness"));
        mText.setFont(*getFontProperty("font"));
        mText.setColor(getColorProperty("textColor"));
        mText.setCharacterSize(getNumberProperty("fontSize"));
        updateFillColors();
        layoutChanged();
    }
    
    void Button::setTextAlign(ORIGIN textOriginX, ORIGIN textOriginY) {
        mTextOriginX = textOriginX;
        mTextOriginY = textOriginY;
        updateText();
    }
    
    void Button::layoutChanged() {
        Widget::layoutChanged();
        float outlineThickness = getNumberProperty("outlineThickness");
        mRectangleShape.setRotation(0); // somewhere rotation was somehow changed sometimes!!! FIXME
        mRectangleShape.setPosition(sf::Vector2f(getGlobalBounds().left+outlineThickness, getGlobalBounds().top+outlineThickness));
        mRectangleShape.setSize(getSize()-sf::Vector2f(2*outlineThickness, 2*outlineThickness));
        updateText();
    }
    std::string Button::getThemeObjectType() {
        return "sui::Button";
    }
    
    void Button::updateFillColors() {
        if(mClicked) {
            mRectangleShape.setFillColor(getColorProperty("fillColorClicked"));
        } else if(mHovered) {
            mRectangleShape.setFillColor(getColorProperty("fillColorHovered"));
        } else {
            mRectangleShape.setFillColor(getColorProperty("fillColor"));
        }
    }
    void Button::updateText() {
        setTextOrigin(mText, mTextOriginX, mTextOriginY, 'X');
        int x;
        int y;
        switch(mTextOriginX) {
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
        
        
        switch(mTextOriginY) {
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
