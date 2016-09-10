#pragma once
#include "Widget.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

namespace sui {
    class Container : public Widget {
    public:
        Container();
        virtual ~Container();
        
        // returns the added widget for convience or nullptr if that widget is already owned
        virtual Widget *addChild(Widget *widget);
        
        // returns the added widget for convience or nullptr if that widget is already owned
        // throws exception if child is added out of range
        virtual Widget *insertChild(Widget *widget, unsigned int pos);
        
        // returns removed widget or nullptr if that widget is not a child
        virtual Widget *removeChild(Widget *widget);
        
        // if this returns false then children cannot manage their own location.
        bool childrenCanMove();
        bool childrenCanResize();
        
        bool isChild(Widget *widget);
        const std::vector<Widget *> &getChildren();
    protected:
        virtual void onInput(sf::Event e);
        virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void onUpdate() = 0;
        virtual void onPropertyChanged(const std::string key);
        void updateChildProperties(Widget *w);

        // unfortunately this has to be a pointer to pointer.
        // usually you can just dereference it immediately but when you need
        // to assign memory then you need the original pointer
        void **getChildCustomData(Widget *child);
        
        std::vector<Widget *> mChildren;
    private:
        bool mLocLocked;
        bool mSizeLocked;
        
        void lockLocation(bool locked);
        void lockSize(bool locked);
        virtual void _onUpdate();
        virtual void _onPositionChanged();
        virtual void _onSizeChanged();
    };
}