#pragma once
#include "Widget.hpp"

#include <SFML/Graphics.hpp>

namespace sui {
    class Text : public Widget {
    public:
        Text();

    protected:
        virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void onInput(sf::Event e);
        virtual void onUpdate(const bool posChanged, const bool sizeChanged);
    private:
        void insureTextFitsHorizontally();
        void insureTextFitsVertically();
        sf::String getText() {
            const Property &p = getProperty("text");
            if(p) return p.as<sf::String>();
            return "";
        }
        sf::Text mText;
    };
}