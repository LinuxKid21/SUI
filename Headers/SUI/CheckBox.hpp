#pragma once
#include "Widget.hpp"

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

namespace sui {
    class CheckBox : public Widget {
    public:
        CheckBox(Theme &theme);
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual bool handleInput(sf::Event e);
        
        void setOnToggled(std::function<void()> func);
        void setOnToggledOn(std::function<void()> func);
        void setOnToggledOff(std::function<void()> func);

        // if theme was changed then call this function to update it!
        void updateTheme();
        
        virtual void layoutChanged();
        virtual std::string getThemeObjectType();
    private:
        sf::RectangleShape mCrossShape1; // two boxes to a cross
        sf::RectangleShape mCrossShape2; // two boxes to a cross
        sf::RectangleShape mRectangleShape;

        bool mState;

        std::function<void()> mOnToggled;
        std::function<void()> mOnToggledOn;
        std::function<void()> mOnToggledOff;
        
        void updateFillColors();
    };
}