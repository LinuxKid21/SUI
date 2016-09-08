#pragma once
#include "Widget.hpp"

#include <SFML/Graphics.hpp>


namespace sui {
    class ColorBox : public Widget {
    public:
        ColorBox(Theme &theme);
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual bool handleInput(sf::Event e);
        virtual void layoutChanged();
        
        void setColor(sf::Color color);
    private:
        sf::RectangleShape mRectangleShape;
    };
}