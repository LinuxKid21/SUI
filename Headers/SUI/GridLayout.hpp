#pragma once
#include "Container.hpp"

#include <SFML/Graphics.hpp>

namespace sui {
    /* children must have gridPosX and gridPosY int properties set */
    class GridLayout : public Container {
    public:
        GridLayout();
    protected:
        virtual void onUpdate(const bool posChanged, const bool sizeChanged);
    private:
        float getPadding() {
            if(getProperty("padding")) return getProperty("padding").as<float>();
            return 0;
        }
        int getXCells() {
            if(getProperty("xCells")) return getProperty("xCells").as<int>();
            return 0;
        }
        int getYCells() {
            if(getProperty("yCells")) return getProperty("yCells").as<int>();
            return 0;
        }
        sf::Vector2f cell_size;
        sf::Vector2f origin_offset;
    };
}