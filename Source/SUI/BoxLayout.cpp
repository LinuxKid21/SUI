#include <SUI/BoxLayout.hpp>


namespace sui {
    float getXDirection(sf::Vector2f v) {
        return v.x;
    }
    float getYDirection(sf::Vector2f v) {
        return v.y;
    }
    
    
    BoxLayout::BoxLayout() : Container() {
        ;
    }
    
    void BoxLayout::onUpdate(const bool posChanged, const bool sizeChanged) {
        Container::onUpdate(posChanged, sizeChanged);
        bool recalculate = hasPropChanged("childAdded") || hasPropChanged("childRemoved") || hasPropChanged("padding") || hasPropChanged("direction");
        for(auto &w : mChildren) {
            if(recalculate) break;
            recalculate = w->hasPropChanged("scaleTypeX") || w->hasPropChanged("scaleTypeY") || w->hasPropChanged("boxSize");
        }
        if(posChanged || recalculate) {
            switch(mOriginX) {
                case ORIGIN_START:
                    origin_offset.x = 0;
                    break;
                case ORIGIN_MIDDLE:
                    origin_offset.x = -getSize().x/2.f;
                    break;
                case ORIGIN_END:
                    origin_offset.x = -getSize().x;
                    break;
            }
            
            switch(mOriginY) {
                case ORIGIN_START:
                    origin_offset.y = 0;
                    break;
                case ORIGIN_MIDDLE:
                    origin_offset.y = -getSize().y/2.f;
                    break;
                case ORIGIN_END:
                    origin_offset.y = -getSize().y;
                    break;
            }
        }
        
        if(sizeChanged || recalculate) {
            float (*getRunningDirection) (sf::Vector2f);
            float (*getOppositeDirection) (sf::Vector2f);
            
            if(getDirection() == HORIZONTAL) {
                getRunningDirection = getXDirection;
                getOppositeDirection = getYDirection;
            } else {
                getRunningDirection = getYDirection;
                getOppositeDirection = getXDirection;
            }
            
            float x = origin_offset.x;
            float y = origin_offset.y;
            const float padding = getPadding();
            const float marginSpaceStolen = padding*(getChildren().size() - 1.f);
            
            // compute the standard size of the children.
            // standard size is how big the average child is. This is influenced
            // by statically sized children and relatively sized children
            float total_static_space = 0;
            float total_standard_spaces = 0;
            
            for(auto &child : getChildren()) {
                if(getChildDirectionTypeRunning(child) == RELATIVE) {
                    total_standard_spaces += getRunningDirection(getChildSize(child));
                } else {
                    total_static_space += getRunningDirection(getChildSize(child));
                }
            }
            const float standard_size = (getRunningDirection(getSize()) - marginSpaceStolen - total_static_space)/total_standard_spaces;
            
            // loop through children and with the standard_size computation figure out the sizes and positions
            for(auto &child : getChildren()) {
                float running_size;
                float opposite_size;
                if(getChildDirectionTypeRunning(child) == RELATIVE) {
                    running_size = getRunningDirection(getChildSize(child))*standard_size;
                } else {
                    running_size = getRunningDirection(getChildSize(child));
                }
                
                if(getChildDirectionTypeOpposite(child) == RELATIVE) {
                    opposite_size = getOppositeDirection(getChildSize(child))*getOppositeDirection(getSize());
                    if(opposite_size > getOppositeDirection(getSize())) opposite_size = getOppositeDirection(getSize());
                    if(opposite_size < 0) opposite_size = 0;
                } else {
                    opposite_size = getOppositeDirection(getChildSize(child));
                }
                child->setOrigin(ORIGIN_START, ORIGIN_START);
                child->setPosition(sf::Vector2f(x, y));
                
                if(getDirection() == HORIZONTAL) {
                    child->setSize(sf::Vector2f(running_size, opposite_size));
                    x += running_size+padding;
                } else {
                    child->setSize(sf::Vector2f(opposite_size, running_size));
                    y += running_size+padding;
                }
            }
        }
    }
}


