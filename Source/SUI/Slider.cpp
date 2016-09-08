#include <SUI/Slider.hpp>

namespace sui {
    Slider::Slider(Theme &theme, BOX_DIRECTION direction) : Widget(theme) {
        mClicked = false;
        mSliderPosition = .5;
        mDirection = direction;
        setRange(0,1);
        updateTheme();
    }
    void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mBarShape);
        target.draw(mRectangleShape);
    }
    bool Slider::handleInput(sf::Event e) {
        if(e.type == sf::Event::MouseMoved) {
            if(mClicked) {
                const auto mousePos = sf::Vector2f(e.mouseMove.x, e.mouseMove.y);
                if(mDirection == HORIZONTAL) {
                    const float x = getGlobalBounds().left;
                    mSliderPosition = (mousePos.x-x)/getSize().x;
                } else {
                    const float y = getGlobalBounds().top;
                    mSliderPosition = (mousePos.y-y)/getSize().y;
                }
                if(mSliderPosition > 1) mSliderPosition = 1;
                if(mSliderPosition < 0) mSliderPosition = 0;
                sliderChanged();
            }
        } else if(e.type == sf::Event::MouseButtonPressed) {
            const auto mousePos = sf::Vector2f(e.mouseButton.x, e.mouseButton.y);
            if(getGlobalBounds().contains(mousePos)) {
                mClicked = true;
                if(mOnClickedDown) {
                    mOnClickedDown();
                }
            }
        } else if(e.type == sf::Event::MouseButtonReleased) {
            if(mClicked) {
                mClicked = false;
                if(mOnClickedUp) {
                    mOnClickedUp();
                }
            }
        }
    }
    
    void Slider::setOnClickedDown(std::function<void()> func) {
        mOnClickedDown = func;
    }
    void Slider::setOnClickedUp(std::function<void()> func) {
        mOnClickedUp = func;
    }
    void Slider::setOnValueChanged(std::function<void()> func) {
        mOnChanged = func;
    }

    void Slider::updateTheme() {
        mRectangleShape.setFillColor(getColorProperty("fillColor"));
        mRectangleShape.setOutlineColor(getColorProperty("outlineColor"));
        mRectangleShape.setOutlineThickness(-2);
        
        mBarShape.setFillColor(getColorProperty("fillColor"));
        mBarShape.setOutlineColor(getColorProperty("outlineColor"));
        mBarShape.setOutlineThickness(-2);
    }
    
    void Slider::setRange(float min, float max) {
        if(max == min) {
            // should we throw an error?
            // should we just handle it even if it makes no sense?
            return;
        }
        mMax = max;
        mMin = min;
        // assure that min is actually the lesser one
        if(mMax > mMin) return;
        mMax = min;
        mMin = max;
    }
    float Slider::getValue() {
        return mSliderPosition*(mMax-mMin)+mMin;
    }
    void Slider::setValue(float value) {
        mSliderPosition = (value-mMin)/(mMax-mMin);
        if(mSliderPosition > 1) mSliderPosition = 1;
        if(mSliderPosition < 0) mSliderPosition = 0;
    }
    
    void Slider::layoutChanged() {
        mRectangleShape.setSize(sf::Vector2f(getSliderSize()));
        mBarShape.setSize(getBarSize());
        if(mDirection == HORIZONTAL) {
            mBarShape.setPosition(getGlobalBounds().left,getGlobalBounds().top-getBarSize().y/2+getSize().y/2);
        } else {
            mBarShape.setPosition(getGlobalBounds().left-getBarSize().x/2+getSize().x/2,getGlobalBounds().top);
        }
        sliderChanged();
    }
    std::string Slider::getThemeObjectType() {
        return "sui::Slider";
    }
    
    sf::Vector2f Slider::getSliderSize() {
        if(mDirection == HORIZONTAL) {
            // 2.5 is completely arbitrary for aesthetics
            return sf::Vector2f(getSize().y/2.5,getSize().y);
        } else {
            return sf::Vector2f(getSize().x, getSize().x/2.5);
        }
    }
    sf::Vector2f Slider::getBarSize() {
        if(mDirection == HORIZONTAL) {
            // 5 is completely arbitrary for aesthetics
            return sf::Vector2f(getSize().x,getSize().y/5);
        } else {
            return sf::Vector2f(getSize().x/5,getSize().y);
        }
    }
    
    void Slider::sliderChanged() {
        static float lastSliderPosition = -1; // something garunteed to be different than mSliderPosition so it runs the first time
        if(lastSliderPosition != mSliderPosition) {
            if(mDirection == HORIZONTAL) {
                mRectangleShape.setPosition(getGlobalBounds().left+mSliderPosition*(getSize().x-Slider::getSliderSize().x),getGlobalBounds().top);
            } else {
                mRectangleShape.setPosition(getGlobalBounds().left,getGlobalBounds().top+mSliderPosition*(getSize().y-Slider::getSliderSize().y));
            }
            lastSliderPosition = mSliderPosition;
            if(mOnChanged) {
                mOnChanged();
            }
        }
    }
}