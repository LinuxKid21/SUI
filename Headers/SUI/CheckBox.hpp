#pragma once
#include "Widget.hpp"

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

namespace sui {
    class CheckBox : public Widget {
    public:
        CheckBox();

    protected:
        virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void onInput(sf::Event e);
        virtual void onUpdate();
    private:
        sf::RectangleShape mCrossShape1; // two boxes to a cross
        sf::RectangleShape mCrossShape2; // two boxes to a cross
        sf::RectangleShape mRectangleShape;
        
        void onToggled() {
            const Property &p = getProperty("onToggled");
            if(p) p.asFunc()();
        }
        void onToggledOn() {
            const Property &p = getProperty("onToggledOn");
            if(p) p.asFunc()();
        }
        void onToggledOff() {
            const Property &p = getProperty("onToggledOff");
            if(p) p.asFunc()();
        }
        
        bool mState;
        
        void updateFillColors();
    };
}