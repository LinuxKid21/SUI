#pragma once
#include "Widget.hpp"

#include <SFML/Graphics.hpp>

namespace sui {
    class Text : public Widget {
    public:
        Text();

    protected:
        virtual void onUpdate();
        virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void onInput(sf::Event e);
    private:
        void insureTextFits();
        sf::Text mText;
    };
}