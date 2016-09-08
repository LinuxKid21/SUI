#pragma once
#include "Container.hpp"

#include <SFML/Graphics.hpp>

namespace sui {
    class GridLayout : public Container {
    protected:
        struct GridChildData;
    public:
        GridLayout(Theme &theme);
        virtual void layoutChanged();
        
        void setPadding(float padding);
        
        
        virtual Widget *addChild(Widget *widget);

        // simply throws an error because it has no meaning here
        virtual Widget *insertChild(Widget *widget, unsigned int pos);
        virtual Widget *removeChild(Widget *widget);
        
        // adds a child directly into a slot
        Widget *addChild(Widget *child, unsigned int x, unsigned int y);
        
        // moves a child to a new 
        bool moveChildTo(Widget *child, unsigned int x, unsigned int y);
        
        void setCells(unsigned int x_cells, unsigned int y_cells);
    protected:
        void initiateChildData(void **data);
        float mPadding;
        unsigned int mXCells; // amount of cells in the x-direction(aka column count)
        unsigned int mYCells; // amount of cells in the y-direction(aka row count)
        
        GridChildData *getGridChildData(Widget *child);
        
        // stores where on the grid this child is
        struct GridChildData {
            unsigned int x;
            unsigned int y;
        };
    };
}