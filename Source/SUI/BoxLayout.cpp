#include <SUI/BoxLayout.hpp>


namespace sui {
    BoxLayout::BoxLayout() : Container() {
        ;
    }
    
    void BoxLayout::onUpdate() {
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
        const float padding = getPadding();
        const float marginSpaceStolen = padding*(getChildren().size() - 1.f);
        
        if(getDirection() == HORIZONTAL) {
            float total_static_width = 0;
            float total_standard_widths = 0;
            
            // computer standard width and total static width allocated
            for(auto &child : getChildren()) {
                if(getChildDirectionTypeX(child) == RELATIVE) {
                    total_standard_widths += getChildSize(child).x;
                } else {
                    total_static_width += getChildSize(child).x;
                }
            }
            const float standard_width = (s.x - marginSpaceStolen - total_static_width)/total_standard_widths;
            for(auto &child : getChildren()) {
                float width;
                float height;
                if(getChildDirectionTypeX(child) == RELATIVE) {
                    width = getChildSize(child).x*standard_width;
                } else {
                    width = getChildSize(child).x;
                }
                
                if(getChildDirectionTypeY(child) == RELATIVE) {
                    height = getChildSize(child).y*s.y;
                    if(height > s.y) height = s.y;
                    if(height < 0) height = 0;
                } else {
                    height = getChildSize(child).y;
                }
                
                child->setSize(sf::Vector2f(width, height));
                child->setPosition(sf::Vector2f(x, y));
                child->setOrigin(ORIGIN_START, ORIGIN_START);
                x += width+padding;
            }
        }
        else {
            float total_static_height = 0;
            float total_standard_heights = 0;
            
            // computer standard width and total static width allocated
            for(auto &child : getChildren()) {
                if(getChildDirectionTypeY(child) == RELATIVE) {
                    total_standard_heights += getChildSize(child).y;
                } else {
                    total_static_height += getChildSize(child).y;
                }
            }
            const float standard_height = (s.y - marginSpaceStolen - total_static_height)/total_standard_heights;
            for(auto &child : getChildren()) {
                float width;
                float height;
                if(getChildDirectionTypeX(child) == RELATIVE) {
                    width = getChildSize(child).x*s.x;
                    if(width > s.x) width = s.x;
                    if(width < 0) width = 0;
                } else {
                    width = getChildSize(child).x;
                }
                
                if(getChildDirectionTypeY(child) == RELATIVE) {
                    height = getChildSize(child).y*standard_height;
                } else {
                    height = getChildSize(child).y;
                }
                
                child->setSize(sf::Vector2f(width, height));
                child->setPosition(sf::Vector2f(x, y));
                child->setOrigin(ORIGIN_START, ORIGIN_START);
                y += height+padding;
            }
        }
    }
}


