#pragma once
#include "Widget.hpp"

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

/* Used properties:
 * onEntered
 * onExited
 * onClickedDown
 * onClickedUp
 * outlineThickness
 * outlineColor
 * fillColor
 * fillColorHovered
 * fillColorClicked
 * textColor
 * font
 * fontSize
 * textAlignX
 * textAlignY
 * text
 */
namespace sui {
    class Button : public Widget {
    public:
        Button();
    private:
        virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void onInput(sf::Event e);
        virtual void onUpdate();
        
        sf::RectangleShape mRectangleShape;
        sf::Text mText;
        
        bool mHovered;
        bool mClicked;
        void onEntered() {
            const Property &p = getProperty("onEntered");
            if(p) p.asFunc()();
        }
        void onExited() {
            const Property &p = getProperty("onExited");
            if(p) p.asFunc()();
        }
        void onClickedDown() {
            const Property &p = getProperty("onClickedDown");
            if(p) p.asFunc()();
        }
        void onClickedUp() {
            const Property &p = getProperty("onClickedUp");
            if(p) p.asFunc()();
        }
        std::string getText() {
            const Property &p = getProperty("text");
            if(p) return p.as<sf::String>();
            return "";
        }
        ORIGIN getTextOriginX() {
            if(getProperty("textOriginX")) return getProperty("textOriginX").as<ORIGIN>();
            return ORIGIN_MIDDLE;
        }
        ORIGIN getTextOriginY() {
            if(getProperty("textOriginY")) return getProperty("textOriginY").as<ORIGIN>();
            return ORIGIN_MIDDLE;
        }
        
        
        void updateFillColors();
        void updateText();
    };
}