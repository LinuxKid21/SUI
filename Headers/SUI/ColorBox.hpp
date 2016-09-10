#pragma once
#include "Widget.hpp"

#include <SFML/Graphics.hpp>


namespace sui {
    class ColorBox : public Widget {
    public:
        ColorBox();
        
    protected:
        virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void onInput(sf::Event e);
        virtual void onUpdate();
    private:
        sf::RectangleShape mRectangleShape;
    };
}