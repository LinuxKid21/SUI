#pragma once
#include "Widget.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

namespace sui {
    class Container : public Widget {
    public:
        Container(Theme &theme);
        virtual ~Container();
        
        virtual bool handleInput(sf::Event e);
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        
        // returns the added widget for convience or nullptr if that widget is already owned
        virtual Widget *addChild(Widget *widget);
        
        // returns the added widget for convience or nullptr if that widget is already owned
        // throws exception if child is added out of range
        virtual Widget *insertChild(Widget *widget, unsigned int pos);
        
        // returns removed widget or nullptr if that widget is not a child
        virtual Widget *removeChild(Widget *widget);
        
        virtual void layoutChanged();
        
        // if this returns false then children cannot manage their own location.
        bool childrenCanMove();
        bool childrenCanResize();
        
        void lockLocation(bool locked);
        void lockSize(bool locked);
        
        bool isChild(Widget *widget);
        std::vector<Widget *> &getChildren();
    private:
        std::vector<Widget *> mChildren;
        bool mLocLocked;
        bool mSizeLocked;
    protected:

        // unfortunately this has to be a pointer to pointer.
        // usually you can just dereference it immediately but when you need
        // to assign memory then you need the original pointer
        void **getChildCustomData(Widget *child);
    };
}