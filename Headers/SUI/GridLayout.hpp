#pragma once
#include "Container.hpp"

#include <SFML/Graphics.hpp>

namespace sui {
    /* children must have gridPosX and gridPosY int properties set */
    class GridLayout : public Container {
    protected:
        struct GridChildData;
    public:
        GridLayout();
    protected:
        virtual void onUpdate();
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
    };
}