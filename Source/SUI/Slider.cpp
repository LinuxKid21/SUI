#include <SUI/Slider.hpp>

namespace sui {
    Slider::Slider() : Widget() {
        mSliderPosition = .5;
        mRectangleShape.setOutlineThickness(-2);
        mBarShape.setOutlineThickness(-2);
        setProperty<float>("value", mSliderPosition);
        setProperty<float>("min", 0);
        setProperty<float>("max", 1);
    }
    void Slider::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mBarShape);
        target.draw(mRectangleShape);
    }
    void Slider::onInput(sf::Event e) {
        Widget::onInput(e);
        const bool clicking = getProperty("clicking").as<bool>();
        if(clicking && e.type == sf::Event::MouseMoved) {
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
    }
    void Slider::onUpdate(const bool posChanged, const bool sizeChanged) {
        Widget::onUpdate(posChanged, sizeChanged);
        if(hasPropChanged("fillColor")) {
            sf::Color c = getProperty("fillColor").as<sf::Color>();
            mRectangleShape.setFillColor(c);
            mBarShape.setFillColor(c);
        }
        if(hasPropChanged("outlineColor")) {
            sf::Color c = getProperty("outlineColor").as<sf::Color>();
            mRectangleShape.setOutlineColor(c);
            mBarShape.setOutlineColor(c);
        }
        
        if(hasPropChanged("min") || hasPropChanged("max")) {
            // sanitize data to something that makes even some sense
            float &min = *getProperty("min").asPointer<float>();
            float &max = *getProperty("max").asPointer<float>();
            if(max < min) {
                const float temp = max;
                max = min;
                min = temp;
            }
        }
        
        if(hasPropChanged("value")) {
            const float max = getProperty("max").as<float>();
            const float min = getProperty("min").as<float>();
            
            float &v = *getProperty("value").asPointer<float>();
            if(v > max) v = max;
            if(v < min) v = min;
            sliderChanged();
        }
        
        const bool slider_direction_changed = hasPropChanged("sliderDirection");
        
        if(posChanged || slider_direction_changed) {
            if(getDirection() == HORIZONTAL) {
                mBarShape.setPosition(getGlobalBounds().left,getGlobalBounds().top-getBarSize().y/2+getSize().y/2);
            } else {
                mBarShape.setPosition(getGlobalBounds().left-getBarSize().x/2+getSize().x/2,getGlobalBounds().top);
            }
            placeSlider();
        }
        if(sizeChanged || slider_direction_changed) {
            mRectangleShape.setSize(sf::Vector2f(getSliderSize()));
            mBarShape.setSize(getBarSize());
            
            placeSlider();
        }
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
    void Slider::placeSlider() {
        if(getDirection() == HORIZONTAL) {
            mRectangleShape.setPosition(getGlobalBounds().left+mSliderPosition*(getSize().x-Slider::getSliderSize().x),getGlobalBounds().top);
        } else {
            mRectangleShape.setPosition(getGlobalBounds().left,getGlobalBounds().top+mSliderPosition*(getSize().y-Slider::getSliderSize().y));
        }
    }
    void Slider::sliderChanged() {
        static float lastSliderPosition = -1; // something garunteed to be different than mSliderPosition so it runs the first time
        if(lastSliderPosition != mSliderPosition) {
            const float max = getProperty("max").as<float>();
            const float min = getProperty("min").as<float>();
            float &v = *getProperty("value").asPointer<float>();
            v = mSliderPosition*(max-min)+min;
            placeSlider();
            lastSliderPosition = mSliderPosition;
            onValueChanged();
        }
    }
}