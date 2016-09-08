#pragma once
#include <SUI/BoxLayout.hpp>
#include <SUI/Text.hpp>
#include <SUI/TextField.hpp>
#include <SUI/FrameLayout.hpp>
#include <SUI/Button.hpp>
#include <vector>

class BoxLayout : public sui::BoxLayout {
public:
    BoxLayout(sui::Theme &theme, BOX_DIRECTION direction);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
    void setLineColors(sf::Color color);
    
    virtual void layoutChanged();
protected:
    sf::Color mColor;
    std::vector<sf::RectangleShape> lines;
};


// not well encapselated. Should not allow external use of addChild or removeChild
// because it assumes too much about them and will most likely result in a crash
class TableLayout : public BoxLayout {
public:
    TableLayout(sui::Theme &theme);
    
    void setLineThickness(float thickness);
    
    void addColumn();
    // will likely be handled internally with an X button
    // void removeColumn(unsigned int index);
    void sort();
    
    void setAutoSort(bool autoSort) {this->shouldAutoSort = autoSort;}
    
    void setOnAddClicked(std::function<void()> func) {add_button->setOnClickedDown(func);}

    float getUsedWidth();
    virtual void layoutChanged();
private:
    const static int column_width = 100;
    const static int button_width = 50;
    const static int text_width = 100;
    sui::Button *add_button;
    bool shouldAutoSort;
    
    void autoSort();
};

