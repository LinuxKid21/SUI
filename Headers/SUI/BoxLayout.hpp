#pragma once
#include "Container.hpp"

#include <SFML/Graphics.hpp>

namespace sui {
    /* children may have boxSize, scaleTypeX and scaleTypeY*/
    class BoxLayout : public Container {
    protected:
        struct BoxChildData;
    public:
        // used in ChildData
        // note: RELATIVE in the direction perpendicular to where the box runs
        // is relative to the size of the box and not to the other elements.
        // This means it has a valid range of [0,1] and everything else will be
        // capped to that
        enum SCALE_TYPE {
            RELATIVE = 0, // relative size(e.g. twice as large as others)
            ABSOLUTE // absolute size(e.g. 200px exactly)
        };
        enum BOX_DIRECTION {
            HORIZONTAL = 0,
            VERTICAL
        };
        BoxLayout();
        
    protected:
        virtual void onUpdate(const bool posChanged, const bool sizeChanged);
        
        void _updateGeneric();
        
        float getPadding() {
            const Property &p = getProperty("padding");
            if(p) return p.as<float>();
            return 0;
        }
        BOX_DIRECTION getDirection() {
            const Property &p = getProperty("direction");
            if(p) return p.as<BOX_DIRECTION>();
            return HORIZONTAL;
        }
        SCALE_TYPE getChildDirectionTypeX(Widget *w) {
            const Property &p = w->getProperty("scaleTypeX");
            if(p) return p.as<SCALE_TYPE>();
            return RELATIVE;
        }
        SCALE_TYPE getChildDirectionTypeY(Widget *w) {
            const Property &p = w->getProperty("scaleTypeY");
            if(p) return p.as<SCALE_TYPE>();
            return RELATIVE;
        }
        SCALE_TYPE getChildDirectionTypeRunning(Widget *w) {
            if(getDirection() == HORIZONTAL) {
                return getChildDirectionTypeX(w);
            } else {
                return getChildDirectionTypeY(w);
            }
        }
        SCALE_TYPE getChildDirectionTypeOpposite(Widget *w) {
            if(getDirection() == HORIZONTAL) {
                return getChildDirectionTypeY(w);
            } else {
                return getChildDirectionTypeX(w);
            }
        }
        sf::Vector2f getChildSize(Widget *w) {
            const Property &p = w->getProperty("boxSize");
            if(p) return p.as<sf::Vector2f>();
            return sf::Vector2f(1,1);
        }
        sf::Vector2f origin_offset;
    };
}
