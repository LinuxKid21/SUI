#include <SUI/Container.hpp>
#include <unordered_set>

namespace sui {
    Container::Container() : Widget() {
        lockLocation(true);
        lockSize(true);
    }
    Container::~Container() {
        for(unsigned int i = 0; i < mChildren.size(); i++) {
            delete mChildren[i];
        }
    }
    
    void Container::onInput(sf::Event e) {
        Widget::onInput(e);
        for(int i = 0; i < mChildren.size(); i++) {
            mChildren[i]->onInput(e);
        }
    }
    
    void Container::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
        for(int i = 0; i < mChildren.size(); i++) {
            mChildren[i]->onDraw(target, states);
        }
    }
    void Container::onUpdate(const bool posChanged, const bool sizeChanged) {
        Widget::onUpdate(posChanged, sizeChanged);
    }

    void Container::_onUpdate() {
        lockLocation(false);
        lockSize(false);
        
        mPosChanged = mPosChanged ||
                mSizeChanged && (mOriginX != ORIGIN_START || mOriginY != ORIGIN_START);
        if(mPosChanged) mInvalidGlobalPosition = true;
        
        for(auto &child : mChildren) {
            child->mPosChanged = child->mPosChanged || mPosChanged;
            child->mSizeChanged = child->mSizeChanged || mSizeChanged;
            if(child->mPosChanged) child->mInvalidGlobalPosition = true;
        }
        onUpdate(mPosChanged, mSizeChanged);
        mSizeChanged = false, mPosChanged = false;
        mChangedKeys.clear();
        
        for(auto &child : mChildren) {
            child->update();
        }
        lockLocation(true);
        lockSize(true);
    }
    void Container::_setPropertyChanged(const std::string key) {
        Widget::_setPropertyChanged(key);
        for(auto &c : mChildren) {
            c->_setPropertyChanged(key);
        }
    }
    
    Widget *Container::addChild(Widget *widget) {
        if(widget->mParent) {
            return nullptr;
        }
        mChildren.push_back(widget);
        widget->mParent = this;
        mChangedKeys.insert("childAdded");
        updateChildProperties(widget);
        return widget;
    }
    
    Widget *Container::insertChild(Widget *widget, unsigned int pos) {
        if(widget->mParent) {
            return nullptr;
        }
        if(pos > mChildren.size()) {
            throw std::string("Error, index passed in is invalid!");
        }
        mChildren.insert(mChildren.begin() + pos, widget);
        widget->mParent = this;
        mChangedKeys.insert("childAdded");
        updateChildProperties(widget);
        return widget;
    }
    
    // collect properties from this and all parents of the new widget and apply them to this child widget
    void Container::updateChildProperties(Widget *w) {
        Widget *iter = this;
        while(iter != nullptr) {
            for(std::map<std::string,Property>::iterator it=iter->mProperties.begin(); it!=iter->mProperties.end(); ++it) {
                w->mChangedKeys.insert(it->first);
            }
            iter = iter->getParent();
        }
        Container *container_child = dynamic_cast<Container *>(w);
        if(container_child)
            for(auto &c : container_child->mChildren) {
                updateChildProperties(c);
            }
    }
    
    Widget *Container::removeChild(Widget *widget) {
        for(int i = 0;i < mChildren.size(); i++) {
            if(mChildren[i] == widget) {
                mChildren.erase(mChildren.begin()+i);
                mChangedKeys.insert("childRemoved");
                widget->mParent = nullptr;
                return widget;
            }
        }
        return nullptr;
    }

    
    bool Container::childrenCanMove() {
        return !mLocLocked;
    }
    bool Container::childrenCanResize() {
        return !mSizeLocked;
    }
    
    void Container::lockLocation(bool locked) {
        mLocLocked = locked;
    }
    void Container::lockSize(bool locked) {
        mSizeLocked = locked;
    }
    
    bool Container::isChild(Widget *widget) {
        return widget->mParent == this;
    }
    
    
    const std::vector<Widget *> &Container::getChildren() {
        return mChildren;
    }
}