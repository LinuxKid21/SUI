#pragma once
#include "Widget.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

namespace sui {
    /* Container adds the special properties to mChangedKeys on certain situations:
     * childAdded
     * childRemoved
    */
    class Container : public Widget {
    public:
        Container();
        virtual ~Container();
        
        // returns the added widget for convience or nullptr if that widget is already owned
        Widget *addChild(Widget *widget);
        
        // returns the added widget for convience or nullptr if that widget is already owned
        // throws exception if child is added out of range
        Widget *insertChild(Widget *widget, unsigned int pos);
        
        // returns removed widget or nullptr if that widget is not a child
        Widget *removeChild(Widget *widget);
        
        // if this returns false then children cannot manage their own location.
        bool childrenCanMove();
        bool childrenCanResize();
        
        bool isChild(Widget *widget);
        const std::vector<Widget *> &getChildren();
    protected:
        virtual void onInput(sf::Event e);
        virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void onUpdate(const bool posChanged, const bool sizeChanged);
        void updateChildProperties(Widget *w);
        
        std::vector<Widget *> mChildren;
    private:
        bool mLocLocked;
        bool mSizeLocked;
        
        void lockLocation(bool locked);
        void lockSize(bool locked);
        virtual void _onUpdate();
        virtual void _setPropertyChanged(const std::string key);
    };
}