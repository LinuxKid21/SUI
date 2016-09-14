#include <SUI/GridLayout.hpp>


namespace sui {
    GridLayout::GridLayout() : Container() {
        ;
    }
    void GridLayout::onUpdate(const bool posChanged, const bool sizeChanged) {
        bool recalculate = hasPropChanged("padding") || hasPropChanged("xCells") || hasPropChanged("yCells");
        if(posChanged) {
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
            const float padding = getPadding();
            const int xCells = getXCells();
            const int yCells = getYCells();
            const float marginSpaceStolenX = padding*(xCells - 1.f);
            const float marginSpaceStolenY = padding*(yCells - 1.f);
            cell_size = sf::Vector2f(
                (getSize().x-marginSpaceStolenX)/(float)xCells,
                (getSize().y-marginSpaceStolenY)/(float)yCells);
        }

        for(auto &c : getChildren()) {
            if(posChanged || recalculate || c->hasPropChanged("gridPosX") || c->hasPropChanged("gridPosY")) {
                const float padding = getPadding();
                c->setOrigin(sui::ORIGIN_START, sui::ORIGIN_START);
                c->setPosition(origin_offset + sf::Vector2f(
                    c->getProperty("gridPosX").as<int>()*(padding+cell_size.x),
                    c->getProperty("gridPosY").as<int>()*(padding+cell_size.y)));
            }
            if(sizeChanged || recalculate) {
                c->setSize(cell_size);
            }
        }
    }
}