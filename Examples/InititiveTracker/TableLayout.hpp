#pragma once
#include <SUI/BoxLayout.hpp>
#include <SUI/Text.hpp>
#include <SUI/TextField.hpp>
#include <SUI/FrameLayout.hpp>
#include <SUI/Button.hpp>
#include <vector>

class BoxLayout : public sui::BoxLayout {
public:
    BoxLayout();
    
protected:
    virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void onUpdate();
    void setLineColors(sf::Color color);
    
    sf::Color mColor;
    std::vector<sf::RectangleShape> lines;
};


// not well encapselated. Should not allow external use of addChild or removeChild
// because it assumes too much about them and will most likely result in a crash
class TableLayout : public BoxLayout {
public:
    TableLayout(std::function<void(sui::Widget *)>, std::function<void(sui::Widget *)>);
    
    // will likely be handled internally with an X button
    // void removeColumn(unsigned int index);
    void sort();
    
    void setAutoSort(bool autoSort) {this->shouldAutoSort = autoSort;}

    float getUsedWidth();
    std::function<void(sui::Widget *)> addedTextField;
private:
    void addColumn();
    const static int column_width = 100;
    const static int button_width = 50;
    const static int text_width = 100;
    sui::Button *add_button;
    bool shouldAutoSort;
    void onAdded() {
        const sui::Property &p = getProperty("onAdded");
        if(p) p.asFunc()();
    }
    
    void autoSort(sui::TextField *textfield);
};

