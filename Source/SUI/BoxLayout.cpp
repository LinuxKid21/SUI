#include <SUI/BoxLayout.hpp>


namespace sui {
    BoxLayout::BoxLayout(Theme &theme, BOX_DIRECTION direction) : Container(theme) {
        mDirection = direction;
        mPadding = 5;
        
        lockLocation(true);
        lockSize(true);
    }
    
    void BoxLayout::setDirection(BOX_DIRECTION direction) {
        mDirection = direction;
    }
    void BoxLayout::setPadding(float padding) {
        mPadding = padding;
    }
    bool BoxLayout::setChildSize(Widget *child, sf::Vector2f size, DIRECTION_TYPE x_dir, DIRECTION_TYPE y_dir) {
        if(isChild(child)) {
            auto *data = getBoxChildData(child);
            data->size = size;
            data->x = x_dir;
            data->y = y_dir;
            return true;
        }
        return false;
    }
    
    void BoxLayout::layoutChanged() {
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

        const float marginSpaceStolen = mPadding*(getChildren().size() - 1.f);
        
        if(mDirection == HORIZONTAL) {
            float total_static_width = 0;
            float total_standard_widths = 0;
            
            // computer standard width and total static width allocated
            for(auto &child : getChildren()) {
                auto *data = getBoxChildData(child);
                if(data->x == RELATIVE) {
                    total_standard_widths += data->size.x;
                } else {
                    total_static_width += data->size.x;
                }
            }
            const float standard_width = (s.x - marginSpaceStolen - total_static_width)/total_standard_widths;
            for(auto &child : getChildren()) {
                auto *data = getBoxChildData(child);
                float width;
                float height;
                if(data->x == RELATIVE) {
                    width = data->size.x*standard_width;
                } else {
                    width = data->size.x;
                }
                
                if(data->y == RELATIVE) {
                    height = data->size.y*s.y;
                    if(height > s.y) height = s.y;
                    if(height < 0) height = 0;
                } else {
                    height = data->size.y;
                }
                
                child->setSize(sf::Vector2f(width, height));
                child->setPosition(sf::Vector2f(x, y));
                child->setOrigin(ORIGIN_START, ORIGIN_START);
                x += width+mPadding;
            }
        }
        else {
            float total_static_height = 0;
            float total_standard_heights = 0;
            
            // computer standard width and total static width allocated
            for(auto &child : getChildren()) {
                auto *data = getBoxChildData(child);
                if(data->y == RELATIVE) {
                    total_standard_heights += data->size.y;
                } else {
                    total_static_height += data->size.y;
                }
            }
            const float standard_height = (s.y - marginSpaceStolen - total_static_height)/total_standard_heights;
            for(auto &child : getChildren()) {
                auto *data = getBoxChildData(child);
                float width;
                float height;
                if(data->x == RELATIVE) {
                    width = data->size.x*s.x;
                    if(width > s.x) width = s.x;
                    if(width < 0) width = 0;
                } else {
                    width = data->size.x;
                }
                
                if(data->y == RELATIVE) {
                    height = data->size.y*standard_height;
                } else {
                    height = data->size.y;
                }
                
                child->setSize(sf::Vector2f(width, height));
                child->setPosition(sf::Vector2f(x, y));
                child->setOrigin(ORIGIN_START, ORIGIN_START);
                y += height+mPadding;
            }
        }

        lockLocation(true);
        lockSize(true);

        Container::layoutChanged();
    }

    Widget *BoxLayout::addChild(Widget *widget) {
        auto *child = Container::addChild(widget);
        if(child) {
            initiateChildData(getChildCustomData(child));
        }
        return child;
    }
    Widget *BoxLayout::insertChild(Widget *widget, unsigned int pos) {
        auto *child = Container::insertChild(widget, pos);
        if(child) {
            initiateChildData(getChildCustomData(child));
        }
        return child;
    }
    Widget *BoxLayout::removeChild(Widget *widget) {
        auto *child = Container::removeChild(widget);
        if(child) {
            BoxChildData *child_data = getBoxChildData(child);
            if(child_data) {
                delete child_data;
            }
        }
        return child;
    }
    void BoxLayout::initiateChildData(void **data) {
        *data = new BoxChildData;
        BoxChildData *child_data = static_cast<BoxChildData *>(*data);
        child_data->x = RELATIVE;
        child_data->y = RELATIVE;
        child_data->size = sf::Vector2f(1,1);
    }
    
    BoxLayout::BoxChildData *BoxLayout::getBoxChildData(Widget *child) {
        return static_cast<BoxChildData *>(*getChildCustomData(child));
    }
}


