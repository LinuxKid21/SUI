#pragma once
#include "Container.hpp"

#include <SFML/Graphics.hpp>

namespace sui {
    class BoxLayout : public Container {
    protected:
        struct BoxChildData;
    public:
        // used in ChildData
        // note: RELATIVE in the direction perpendicular to where the box runs
        // is relative to the size of the box and not to the other elements.
        // This means it has a valid range of [0,1] and everything else will be
        // capped to that
        enum DIRECTION_TYPE {
            RELATIVE = 0, // relative size(e.g. twice as large as others)
            ABSOLUTE // absolute size(e.g. 200px exactly)
        };
        enum BOX_DIRECTION {
            HORIZONTAL = 0,
            VERTICAL
        };
        BoxLayout(Theme &theme, BOX_DIRECTION direction);
        
        void setDirection(BOX_DIRECTION direction);
        void setPadding(float padding);
        
        bool setChildSize(Widget *child, sf::Vector2f size, DIRECTION_TYPE x_dir, DIRECTION_TYPE y_dir);
        
        virtual void layoutChanged();

        virtual Widget *addChild(Widget *widget);
        virtual Widget *insertChild(Widget *widget, unsigned int pos);
        virtual Widget *removeChild(Widget *widget);
    protected:
        void initiateChildData(void **data);
        
        BOX_DIRECTION mDirection;
        float mPadding;
        
        BoxChildData *getBoxChildData(Widget *child);
        
        // extra data to add to children
        struct BoxChildData {
            sf::Vector2f size;
            DIRECTION_TYPE x;
            DIRECTION_TYPE y;
        };
    };
}
