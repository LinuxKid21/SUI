#pragma once
#include "Widget.hpp"

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

namespace sui {
    class Slider : public Widget {
    public:
        enum BOX_DIRECTION {
            HORIZONTAL = 0,
            VERTICAL
        };
        Slider();
    protected:
        virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void onInput(sf::Event e);
        virtual void onUpdate();
    private:
        sf::Vector2f getSliderSize();
        sf::Vector2f getBarSize();
        void sliderChanged();
        
        void onClickedDown() {
            const Property &p = getProperty("onClickedDown");
            if(p) p.asFunc()();
        }
        void onClickedUp() {
            const Property &p = getProperty("onClickedUp");
            if(p) p.asFunc()();
        }
        void onValueChanged() {
            const Property &p = getProperty("onValueChanged");
            if(p) p.asFunc()();
        }
        float getMax() {
            const Property &p = getProperty("max");
            if(p) return p.as<float>();
            return 1;
        }
        float getMin() {
            const Property &p = getProperty("min");
            if(p) return p.as<float>();
            return 0;
        }
        BOX_DIRECTION getDirection() {
            const Property &p = getProperty("sliderDirection");
            if(p) return p.as<BOX_DIRECTION>();
            return HORIZONTAL;
        }
        
        // the button you click on
        sf::RectangleShape mRectangleShape;
        
        // the bar the button slides on
        sf::RectangleShape mBarShape;
        
        // value in the range [0,1]
        float mSliderPosition;
        
        bool mClicked;
    };
}