#include <SUI/SUI.hpp>

namespace sui {
    void initiateSUI() {
        Theme::registerType("sui::Widget", "");
        Theme::registerType("sui::Button", "sui::Widget");
        Theme::registerType("sui::CheckBox", "sui::Widget");
        Theme::registerType("sui::Slider", "sui::Widget");
        Theme::registerType("sui::TextField", "sui::Widget");
        Theme::registerType("sui::Text", "sui::Widget");
        Theme::registerType("sui::Image", "sui::Widget");
        Theme::registerType("sui::ColorBox", "sui::Widget");
        
        Theme::registerType("sui::Container", "sui::Widget");
        Theme::registerType("sui::BoxLayout", "sui::Container");
        Theme::registerType("sui::GridLayout", "sui::Container");
        Theme::registerType("sui::FrameLayout", "sui::Container");
    }
}
