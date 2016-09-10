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
        virtual void onPropertyChanged(const std::string key);
        virtual void onPositionChanged();
        virtual void onSizeChanged();
        
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
        ORIGIN getTextOriginX() {
            if(getProperty("textAlignX")) return getProperty("textAlignX").as<ORIGIN>();
            return ORIGIN_MIDDLE;
        }
        ORIGIN getTextOriginY() {
            if(getProperty("textAlignY")) return getProperty("textAlignY").as<ORIGIN>();
            return ORIGIN_MIDDLE;
        }
        float getTextPadding() {
            if(getProperty("textPadding")) return getProperty("textPadding").as<float>();
            return 5;
        }
        
        sf::Color norm_color;
        sf::Color hover_color;
        sf::Color click_color;
        void updateFillColors();
        void updateText();
    };
}