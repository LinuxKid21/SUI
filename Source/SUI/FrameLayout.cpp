#include <SUI/FrameLayout.hpp>


namespace sui {
    FrameLayout::FrameLayout() : Container() {
        ;
    }
    void FrameLayout::onUpdate(const bool posChanged, const bool sizeChanged) {
        
        // only one child but this works just the same either way
        for(auto &child : getChildren()) {
            child->setOrigin(ORIGIN_START,ORIGIN_START);

            sf::Vector2f frameSize = getFrameSize(child);
            sf::Vector2f framePos = getFramePos(child);
            float x;
            float y;
            float width;
            float height;
            if(getScaleTypeX(child) == RELATIVE) {
                width = getSize().x*frameSize.x;
            } else {
                width = frameSize.x;
            }
            
            if(getScaleTypeY(child) == RELATIVE) {
                height = getSize().y*frameSize.y;
            } else {
                height = frameSize.y;
            }
            if(width < 0) width = 0;
            if(width > getSize().x) width = getSize().x;
            if(height < 0) height = 0;
            if(height > getSize().y) height = getSize().y;
            child->setSize(sf::Vector2f(width, height));
            
            
            if(getPosTypeX(child) == RELATIVE) {
                // if(getFramePos().x < 0) getFramePos().x = 0;
                // if(getFramePos().x > 1) getFramePos().x = 1;
                x = getSize().x*getFramePos(child).x-width*getFramePos(child).x;
            } else {
                x = getFramePos(child).x;
            }
            
            if(getPosTypeY(child) == RELATIVE) {
                // if(getFramePos().y < 0) getFramePos().y = 0;
                // if(getFramePos().y > 1) getFramePos().y = 1;
                y = getSize().y*getFramePos(child).y-height*getFramePos(child).y;
            } else {
                y = getFramePos(child).y;
            }
            if(x < 0) x = 0;
            if(x > getSize().x+width) x = getSize().x+width;
            if(y < 0) y = 0;
            if(y > getSize().y+height) y = getSize().y+height;
            child->setPosition(sf::Vector2f(x, y));
            
        }
    }
}
