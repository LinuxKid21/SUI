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
        Slider(Theme &theme, BOX_DIRECTION direction = HORIZONTAL);
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual bool handleInput(sf::Event e);
        
        void setOnClickedDown(std::function<void()> func);
        void setOnClickedUp(std::function<void()> func);
        void setOnValueChanged(std::function<void()> func);
        
        // if theme was changed then call this function to update it!
        void updateTheme();
        
        void setRange(float min, float max);
        
        float getValue();
        void setValue(float value);
        
        virtual void layoutChanged();
        virtual std::string getThemeObjectType();
    private:
        sf::Vector2f getSliderSize();
        sf::Vector2f getBarSize();
        void sliderChanged();
        
        // the button you click on
        sf::RectangleShape mRectangleShape;
        
        // the bar the button slides on
        sf::RectangleShape mBarShape;
        
        // value in the range [0,1]
        float mSliderPosition;
        float mMax;
        float mMin;
        
        BOX_DIRECTION mDirection;
        
        std::function<void()> mOnClickedDown;
        std::function<void()> mOnClickedUp;
        std::function<void()> mOnChanged;
        
        bool mClicked;
    };
}