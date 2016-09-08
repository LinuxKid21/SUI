#pragma once
#include "Container.hpp"

#include <SFML/Graphics.hpp>

namespace sui {
    class FrameLayout : public Container {
    protected:
        struct FrameChildData;
    public:
        enum DIRECTION_TYPE {
            RELATIVE = 0, // relative size(e.g. twice as large as others)
            ABSOLUTE // absolute size(e.g. 200px exactly)
        };
        FrameLayout(Theme &theme);
        virtual void layoutChanged();
        
        bool setChildSize(Widget *child, sf::Vector2f size, DIRECTION_TYPE x_dir, DIRECTION_TYPE y_dir);
        bool setChildPosition(Widget *child, sf::Vector2f pos, DIRECTION_TYPE x_dir, DIRECTION_TYPE y_dir);
        
        virtual Widget *addChild(Widget *widget);
        virtual Widget *insertChild(Widget *widget, unsigned int pos);
        virtual Widget *removeChild(Widget *widget);
    protected:
        
        void initiateChildData(void **data);
        FrameChildData *getFrameChildData(Widget *child);
        struct FrameChildData {
            sf::Vector2f size;
            sf::Vector2f pos;
            
            DIRECTION_TYPE pos_type_x;
            DIRECTION_TYPE pos_type_y;
            
            DIRECTION_TYPE size_type_x;
            DIRECTION_TYPE size_type_y;
        };
    };
}