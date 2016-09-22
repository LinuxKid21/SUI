
properties = {
    {"number", "fontSize", 24},
    {"number", "outlineThickness", 2},
    {"color", "outlineColor", {0,0,255}},
    {"color", "fillColor", {255,255,255}},
    {"color", "fillColorHovered", {200,200,200}},
    {"color", "fillColorClicked", {100,100,100}},
    {"color", "textColor", {0,0,0}},
    {"font", "font", "arial.ttf"},
    {"string", "text", "hello world!"},
    {"function", "onClickedDown", "test"},
}

b = addButton()
setPosition(b, {100,100})
setSize(b, {300,50})
setProperties(b, properties)

c = addCheckBox()
setPosition(c, {400,200})
setSize(c, {30,30})
setProperties(c, properties)