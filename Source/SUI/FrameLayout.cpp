#include <SUI/FrameLayout.hpp>


namespace sui {
    FrameLayout::FrameLayout(Theme &theme) : Container(theme) {
        lockLocation(true);
        lockSize(true);
    }
    void FrameLayout::layoutChanged() {
        lockLocation(false);
        lockSize(false);
        
        // only one child but this works just the same either way
        for(auto &child : getChildren()) {
            child->setOrigin(ORIGIN_START,ORIGIN_START);

            auto *data = getFrameChildData(child);
            float x;
            float y;
            float width;
            float height;
            if(data->size_type_x == RELATIVE) {
                width = getSize().x*data->size.x;
            } else {
                width = data->size.x;
            }
            
            if(data->size_type_y == RELATIVE) {
                height = getSize().y*data->size.y;
            } else {
                height = data->size.y;
            }
            if(width < 0) width = 0;
            if(width > getSize().x) width = getSize().x;
            if(height < 0) height = 0;
            if(height > getSize().y) height = getSize().y;
            child->setSize(sf::Vector2f(width, height));
            
            
            if(data->pos_type_x == RELATIVE) {
                if(data->pos.x < 0) data->pos.x = 0;
                if(data->pos.x > 1) data->pos.x = 1;
                x = getSize().x*data->pos.x-width*data->pos.x;
            } else {
                x = data->pos.x;
            }
            
            if(data->pos_type_y == RELATIVE) {
                if(data->pos.y < 0) data->pos.y = 0;
                if(data->pos.y > 1) data->pos.y = 1;
                y = getSize().y*data->pos.y-height*data->pos.y;
            } else {
                y = data->pos.y;
            }
            if(x < 0) x = 0;
            if(x > getSize().x+width) x = getSize().x+width;
            if(y < 0) y = 0;
            if(y > getSize().y+height) y = getSize().y+height;
            child->setPosition(sf::Vector2f(x, y));
            
        }
        lockLocation(true);
        lockSize(true);
        
        
        Container::layoutChanged();
    }
    
    
    bool FrameLayout::setChildSize(Widget *child, sf::Vector2f size, DIRECTION_TYPE x_dir, DIRECTION_TYPE y_dir) {
        if(isChild(child)) {
            auto *data = getFrameChildData(child);
            data->size = size;
            data->size_type_x = x_dir;
            data->size_type_y = y_dir;
            return true;
        }
        return false;
    }
    bool FrameLayout::setChildPosition(Widget *child, sf::Vector2f pos, DIRECTION_TYPE x_dir, DIRECTION_TYPE y_dir) {
        if(isChild(child)) {
            auto *data = getFrameChildData(child);
            data->pos = pos;
            data->pos_type_x = x_dir;
            data->pos_type_y = y_dir;
            return true;
        }
        return false;
    }
    
    Widget *FrameLayout::addChild(Widget *widget) {
        if(getChildren().size() != 0) {
            return nullptr;
        }
        auto *child = Container::addChild(widget);
        if(child) {
            initiateChildData(getChildCustomData(child));
        }
        return child;
    }
    Widget *FrameLayout::insertChild(Widget *widget, unsigned int pos) {
        return nullptr;
    }
    Widget *FrameLayout::removeChild(Widget *widget) {
        auto *child = Container::removeChild(widget);
        if(child) {
            FrameChildData *child_data = getFrameChildData(child);
            if(child_data) {
                delete child_data;
            }
        }
        return child;
    }
    
    void FrameLayout::initiateChildData(void **data) {
        *data = new FrameChildData;
        FrameChildData *child_data = static_cast<FrameChildData *>(*data);
        child_data->size_type_x = RELATIVE;
        child_data->size_type_y = RELATIVE;
        child_data->pos_type_x = RELATIVE;
        child_data->pos_type_y = RELATIVE;
        child_data->pos = sf::Vector2f(0,0);
        child_data->size = sf::Vector2f(1,1);
    }
    FrameLayout::FrameChildData *FrameLayout::getFrameChildData(Widget *child) {
        return static_cast<FrameChildData *>(*getChildCustomData(child));
    }
}
