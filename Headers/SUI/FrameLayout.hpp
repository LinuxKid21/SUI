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
        FrameLayout();
        
    protected:
        virtual void onUpdate();
        DIRECTION_TYPE getScaleTypeX(Widget *c) {
            const Property &p = c->getProperty("scaleTypeX");
            if(p) return p.as<DIRECTION_TYPE>();
            return RELATIVE;
        }
        DIRECTION_TYPE getScaleTypeY(Widget *c) {
            const Property &p = c->getProperty("scaleTypeY");
            if(p) return p.as<DIRECTION_TYPE>();
            return RELATIVE;
        }
        sf::Vector2f getFrameSize(Widget *c) {
            const Property &p = c->getProperty("frameSize");
            if(p) return p.as<sf::Vector2f>();
            return sf::Vector2f(1,1);
        }
        
        DIRECTION_TYPE getPosTypeX(Widget *c) {
            const Property &p = c->getProperty("posTypeX");
            if(p) return p.as<DIRECTION_TYPE>();
            return RELATIVE;
        }
        DIRECTION_TYPE getPosTypeY(Widget *c) {
            const Property &p = c->getProperty("posTypeY");
            if(p) return p.as<DIRECTION_TYPE>();
            return RELATIVE;
        }
        sf::Vector2f getFramePos(Widget *c) {
            const Property &p = c->getProperty("framePos");
            if(p) return p.as<sf::Vector2f>();
            return sf::Vector2f(0,0);
        }
        
    };
}