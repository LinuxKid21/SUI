#include <SUI/GridLayout.hpp>


namespace sui {
    GridLayout::GridLayout(Theme &theme) : Container(theme) {
        mPadding = 5;
        lockLocation(true);
        lockSize(true);
    }
    void GridLayout::layoutChanged() {
        lockLocation(false);
        lockSize(false);
        float x;
        float y;
        sf::Vector2f s = getSize();
        switch(mOriginX) {
            case ORIGIN_START:
                x = 0;
                break;
            case ORIGIN_MIDDLE:
                x = -s.x/2.f;
                break;
            case ORIGIN_END:
                x = -s.x;
                break;
        }
        
        switch(mOriginY) {
            case ORIGIN_START:
                y = 0;
                break;
            case ORIGIN_MIDDLE:
                y = -s.y/2.f;
                break;
            case ORIGIN_END:
                y = -s.y;
                break;
        }
        
        const float marginSpaceStolenX = mPadding*(mXCells - 1.f);
        const float marginSpaceStolenY = mPadding*(mYCells - 1.f);
        sf::Vector2f cell_size = sf::Vector2f(
            (s.x-marginSpaceStolenX)/(float)mXCells,
            (s.y-marginSpaceStolenY)/(float)mYCells);
        for(auto &c : getChildren()) {
            GridChildData *data = getGridChildData(c);
            c->setOrigin(sui::ORIGIN_START, sui::ORIGIN_START);
            c->setPosition(sf::Vector2f(
                x+data->x*(mPadding+cell_size.x),
                y+data->y*(mPadding+cell_size.y)));
            c->setSize(cell_size);
        }

        lockLocation(true);
        lockSize(true);
        
        Container::layoutChanged();
    }
    void GridLayout::setPadding(float padding) {
        mPadding = padding;
    }
    
    
    Widget *GridLayout::addChild(Widget *widget) {
        auto *child = Container::addChild(widget);
        if(child) {
            initiateChildData(getChildCustomData(child));
        }
        return child;
    }
    Widget *GridLayout::insertChild(Widget *widget, unsigned int pos) {
        throw "GridLayout does not support insertChild";
    }
    Widget *GridLayout::removeChild(Widget *widget) {
        auto *child = Container::removeChild(widget);
        if(child) {
            GridChildData *child_data = getGridChildData(child);
            if(child_data) {
                delete child_data;
            }
        }
        return child;
    }

    Widget *GridLayout::addChild(Widget *child, unsigned int x, unsigned int y) { 
        Widget *added_child = addChild(child);
        if(added_child) {
            moveChildTo(added_child, x, y);
        }
        return added_child;
    }
    bool GridLayout::moveChildTo(Widget *child, unsigned int x, unsigned int y) {
        if(!isChild(child)) return false;
        if(x > mXCells || y > mYCells) {
            return false;
        }
        GridChildData *data = getGridChildData(child);
        data->x = x;
        data->y = y;
        return true;
    }
    void GridLayout::setCells(unsigned int x_cells, unsigned int y_cells) {
        mXCells = x_cells;
        mYCells = y_cells;
    }
    
    void GridLayout::initiateChildData(void **data) {
        *data = new GridChildData;
        GridChildData *child_data = static_cast<GridChildData *>(*data);
        child_data->x = 0;
        child_data->y = 0;
    }
    
    GridLayout::GridChildData *GridLayout::getGridChildData(Widget *child) {
        return static_cast<GridChildData *>(*getChildCustomData(child));
    }
}