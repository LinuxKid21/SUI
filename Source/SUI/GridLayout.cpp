#include <SUI/GridLayout.hpp>


namespace sui {
    GridLayout::GridLayout() : Container() {
        ;
    }
    void GridLayout::onUpdate() {
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
        const int xCells = getXCells();
        const int yCells = getYCells();
        const float padding = getPadding();
        const float marginSpaceStolenX = padding*(xCells - 1.f);
        const float marginSpaceStolenY = padding*(yCells - 1.f);
        sf::Vector2f cell_size = sf::Vector2f(
            (s.x-marginSpaceStolenX)/(float)xCells,
            (s.y-marginSpaceStolenY)/(float)yCells);
        for(auto &c : getChildren()) {
            c->setOrigin(sui::ORIGIN_START, sui::ORIGIN_START);
            c->setPosition(sf::Vector2f(
                x+c->getProperty("gridPosX").as<int>()*(padding+cell_size.x),
                y+c->getProperty("gridPosY").as<int>()*(padding+cell_size.y)));
            c->setSize(cell_size);
        }
    }
}