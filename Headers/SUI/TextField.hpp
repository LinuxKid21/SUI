#pragma once
#include "Widget.hpp"

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

namespace sui {
    class TextField : public Widget {
    public:
        TextField();

    protected:
        virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void onInput(sf::Event e);
        virtual void onUpdate(const bool posChanged, const bool sizeChanged);
    private:
        sf::RectangleShape mRectangleShape;
        sf::Text mText;
        
        sf::RectangleShape mCursorShape;
        sf::RectangleShape mHighlightShape;
        unsigned int mCursorPositionStart;
        unsigned int mCursorPositionEnd;
        
        // hack to make these work within the SFML draw cycle which is const
        // unfortunately I don't see a better place to put the blinking cursor
        // then in the draw cycle without adding a dedicated update function
        // which I might just do, but I would like to avoid that
        mutable bool mCursorVisible;
        mutable sf::Clock mCursorTimer;

        void resetCursorBlink();
        void moveCursorLeft(bool shift);
        void moveCursorRight(bool shift);
        
        
        void onEntered() {
            const Property &p = getProperty("onEntered");
            if(p) p.asFunc()();
        }
        void onExited() {
            const Property &p = getProperty("onExited");
            if(p) p.asFunc()();
        }
        void onActivated() {
            const Property &p = getProperty("onActivated");
            if(p) p.asFunc()();
        }
        void onDeactivated() {
            const Property &p = getProperty("onDeactivated");
            if(p) p.asFunc()();
        }
        void onChanged() {
            const Property &p = getProperty("onChanged");
            if(p) p.asFunc()();
        }
        float getTextPadding() {
            const Property &p = getProperty("textPadding");
            if(p) return p.as<float>();
            return 5;
        }
        // assumes text has been set!
        sf::String &getText() {
            const Property &p = getProperty("text");
            return *p.asPointer<sf::String>();
        }
        
        
        // x_diff is how far away the cursor is in the x direction from start of the textbox
        int findNearestCursorPoint(float x_diff);
        
        bool mActive;
        bool mHovered;
        bool mSelecting;
        void updateText();
        void updateCursor();
        
        void deleteInsideCursor();
    };
}
