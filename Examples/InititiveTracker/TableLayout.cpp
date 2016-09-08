#include "TableLayout.hpp"
#include <algorithm>
#include <string>

BoxLayout::BoxLayout(sui::Theme &theme, BOX_DIRECTION direction) : sui::BoxLayout(theme, direction) {
    lockLocation(true);
    lockSize(true);
}
void BoxLayout::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sui::BoxLayout::draw(target, states);
    for(auto &l : lines) {
        target.draw(l, states);
    }
}

void BoxLayout::setLineColors(sf::Color color) {
    mColor = color;
    for(auto &l : lines) {
        l.setFillColor(color);
    }
}

void BoxLayout::layoutChanged() {
    sui::BoxLayout::layoutChanged();
    lockLocation(false);
    lockSize(false);
    lines.clear();
    // items flow horizontally so lines are vertical between them
    if(mDirection == HORIZONTAL) {
        // place bar after every item except the last one
        for(unsigned int i = 0; i < getChildren().size()-1; i++) {
            auto &c = getChildren()[i];
            const float place = c->getGlobalBounds().left + c->getSize().x;
            const float width = mPadding;
            lines.push_back(sf::RectangleShape(sf::Vector2f(width, getSize().y)));
            lines.back().setPosition(sf::Vector2f(place, getGlobalBounds().top));
            
        }
    } else if(mDirection == VERTICAL) {
        // place bar after every item except the last one
        for(unsigned int i = 0; i < getChildren().size()-1; i++) {
            auto &c = getChildren()[i];
            const float place = c->getGlobalBounds().top + c->getSize().y;
            const float height = mPadding;
            lines.push_back(sf::RectangleShape(sf::Vector2f(getSize().x, height)));
            lines.back().setPosition(sf::Vector2f(getGlobalBounds().left, place));
        }
    }
    setLineColors(mColor);
    lockLocation(true);
    lockSize(true);
}





TableLayout::TableLayout(sui::Theme &theme) : BoxLayout(theme, sui::BoxLayout::HORIZONTAL) {
    BoxLayout *vbox = new BoxLayout(theme, sui::BoxLayout::VERTICAL);
    addChild(vbox);
    setChildSize(vbox, sf::Vector2f(text_width,1), sui::BoxLayout::ABSOLUTE, sui::BoxLayout::RELATIVE);
    
    auto addText = [vbox, &theme](const std::string str) {
        sui::FrameLayout *frame = new sui::FrameLayout(theme);
        sui::Text *text = new sui::Text(theme);
        text->setText(str);
        
        frame->addChild(text);
        frame->setChildSize(text, sf::Vector2f(1,.6), sui::FrameLayout::RELATIVE, sui::FrameLayout::RELATIVE);
        frame->setChildPosition(text, sf::Vector2f(0,.5), sui::FrameLayout::RELATIVE, sui::FrameLayout::RELATIVE);
        
        vbox->addChild(frame);
    };
    
    addText("Name");
    addText("Inititive Roll");
    addText("Inititive Mod");
    addText("Health");
    
    
    add_button = new sui::Button(theme);
    addChild(add_button);
    add_button->setText("+");
    add_button->setNumberProperty("fontSize", 36);
    add_button->updateTheme();
    add_button->setTextAlign(sui::ORIGIN_MIDDLE, sui::ORIGIN_MIDDLE);
    
    shouldAutoSort = false;
    
    setLineColors(sf::Color(0,0,0,255));
    
    lockLocation(true);
    lockSize(true);
}
void TableLayout::setLineThickness(float thickness) {
    setPadding(thickness);
    for(auto &c : getChildren()) {
        static_cast<sui::BoxLayout *>(c)->setPadding(thickness);
    }
}

void TableLayout::addColumn() {
    BoxLayout *vbox = new BoxLayout(getTheme(), sui::BoxLayout::VERTICAL);
    insertChild(vbox, getChildren().size()-1);
    setChildSize(vbox, sf::Vector2f(column_width,1), sui::BoxLayout::ABSOLUTE, sui::BoxLayout::RELATIVE);
    
    for(unsigned int i = 0;i < 4; i++) {
        sui::TextField *textfield = new sui::TextField(getTheme());
        
        if(i == 1 || i == 2)
            textfield->setOnChanged([this](){
                autoSort();
            });
        vbox->addChild(textfield);
    }
}
void TableLayout::autoSort() {
    if(shouldAutoSort) {
        sort();
    }
}
void TableLayout::sort() {
    std::sort(getChildren().begin()+1, getChildren().end()-1,
        [](sui::Widget *i, sui::Widget *j){
            const sui::Widget *text_field_roll_i = static_cast<sui::BoxLayout *>(i)->getChildren()[1];
            const sui::Widget *text_field_mod_i = static_cast<sui::BoxLayout *>(i)->getChildren()[2];
            const std::string roll_string_i = static_cast<const sui::TextField *>(text_field_roll_i)->getText().toAnsiString();
            const std::string mod_string_i = static_cast<const sui::TextField *>(text_field_mod_i)->getText().toAnsiString();
            int roll_i;
            int mod_i;
            try {
                mod_i = std::stoi(mod_string_i);
            } catch(std::invalid_argument arg) {
                mod_i = 0;
            }
            try {
                roll_i = std::stoi(roll_string_i)+mod_i;
            } catch(std::invalid_argument arg) {
                return false;
            }
            
            const sui::Widget *text_field_roll_j = static_cast<sui::BoxLayout *>(j)->getChildren()[1];
            const sui::Widget *text_field_mod_j = static_cast<sui::BoxLayout *>(j)->getChildren()[2];
            const std::string roll_string_j = static_cast<const sui::TextField *>(text_field_roll_j)->getText().toAnsiString();
            const std::string mod_string_j = static_cast<const sui::TextField *>(text_field_mod_j)->getText().toAnsiString();
            int roll_j;
            int mod_j;
            try {
                mod_j = std::stoi(mod_string_j);
            } catch(std::invalid_argument arg) {
                mod_j = 0;
            }
            try {
                roll_j = std::stoi(roll_string_j)+mod_j;
            } catch(std::invalid_argument arg) {
                return false;
            }
            
            if(roll_i > roll_j) {
                return true;
            } else if(roll_i < roll_j) {
                return false;
            } else {
                if(mod_i > mod_j) {
                    return true;
                } else if(mod_i < mod_j) {
                    return false;
                } else {
                    return false; //keep order if tied cause why move?
                }
            }
        });
    layoutChanged();
}

float TableLayout::getUsedWidth() {
    return (getChildren().size()-2)*column_width + button_width + text_width;
}

void TableLayout::layoutChanged() {
    lockLocation(false);
    lockSize(false);
    BoxLayout::layoutChanged();
    lockLocation(true);
    lockSize(true);
}

