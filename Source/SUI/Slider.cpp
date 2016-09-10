#include <SUI/Slider.hpp>

namespace sui {
    Slider::Slider() : Widget() {
        mClicked = false;
        mSliderPosition = .5;
    }
    void Slider::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mBarShape);
        target.draw(mRectangleShape);
    }
    void Slider::onInput(sf::Event e) {
        if(e.type == sf::Event::MouseMoved) {
            if(mClicked) {
                const auto mousePos = sf::Vector2f(e.mouseMove.x, e.mouseMove.y);
                if(getDirection() == HORIZONTAL) {
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
                onClickedDown();
            }
        } else if(e.type == sf::Event::MouseButtonReleased) {
            if(mClicked) {
                mClicked = false;
                onClickedUp();
            }
        }
    }
    
    
    // void Slider::setRange(float min, float max) {
    //     if(max == min) {
    //         // should we throw an error?
    //         // should we just handle it even if it makes no sense?
    //         return;
    //     }
    //     mMax = max;
    //     mMin = min;
    //     // assure that min is actually the lesser one
    //     if(mMax > mMin) return;
    //     mMax = min;
    //     mMin = max;
    // }
    // void Slider::setValue(float value) {
    //     mSliderPosition = (value-mMin)/(mMax-mMin);
    //     if(mSliderPosition > 1) mSliderPosition = 1;
    //     if(mSliderPosition < 0) mSliderPosition = 0;
    // }
    
    void Slider::onUpdate() {
        mRectangleShape.setFillColor(getProperty("fillColor").as<sf::Color>());
        mRectangleShape.setOutlineColor(getProperty("outlineColor").as<sf::Color>());
        mRectangleShape.setOutlineThickness(-2);
        
        mBarShape.setFillColor(getProperty("fillColor").as<sf::Color>());
        mBarShape.setOutlineColor(getProperty("outlineColor").as<sf::Color>());
        mBarShape.setOutlineThickness(-2);
        
        
        
        mRectangleShape.setSize(sf::Vector2f(getSliderSize()));
        mBarShape.setSize(getBarSize());
        if(getDirection() == HORIZONTAL) {
            mBarShape.setPosition(getGlobalBounds().left,getGlobalBounds().top-getBarSize().y/2+getSize().y/2);
        } else {
            mBarShape.setPosition(getGlobalBounds().left-getBarSize().x/2+getSize().x/2,getGlobalBounds().top);
        }
        sliderChanged();
    }
    
    sf::Vector2f Slider::getSliderSize() {
        if(getDirection() == HORIZONTAL) {
            // 2.5 is completely arbitrary for aesthetics
            return sf::Vector2f(getSize().y/2.5,getSize().y);
        } else {
            return sf::Vector2f(getSize().x, getSize().x/2.5);
        }
    }
    sf::Vector2f Slider::getBarSize() {
        if(getDirection() == HORIZONTAL) {
            // 5 is completely arbitrary for aesthetics
            return sf::Vector2f(getSize().x,getSize().y/5);
        } else {
            return sf::Vector2f(getSize().x/5,getSize().y);
        }
    }
    
    void Slider::sliderChanged() {
        static float lastSliderPosition = -1; // something garunteed to be different than mSliderPosition so it runs the first time
        if(lastSliderPosition != mSliderPosition) {
            float max = getMax();
            float min = getMin();
            setProperty("value", sui::Property::make<float>(mSliderPosition*(max-min)+min));
            if(getDirection() == HORIZONTAL) {
                mRectangleShape.setPosition(getGlobalBounds().left+mSliderPosition*(getSize().x-Slider::getSliderSize().x),getGlobalBounds().top);
            } else {
                mRectangleShape.setPosition(getGlobalBounds().left,getGlobalBounds().top+mSliderPosition*(getSize().y-Slider::getSliderSize().y));
            }
            lastSliderPosition = mSliderPosition;
            onValueChanged();
        }
    }
}