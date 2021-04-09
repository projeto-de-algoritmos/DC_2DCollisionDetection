#include "DynamicText.hpp"
#include "AssetsManager.hpp"

#include <cstdint>

DynamicText * DynamicText::newDynamicText(const std::string & text,
                                          const std::string & font_name,
                                          uint16_t font_size,
                                          const SDL_Color & font_color)
{
    DynamicText * dt = new DynamicText(font_name, font_size, font_color);
    dt->setText(text);

    return dt;
}

void DynamicText::setText(const std::string & text)
{
    while (_chars.size() < text.size()) {
        _chars.push_back(SolidText::newSolidText("_", _font_name, _font_size, _font_color));

        if (_chars.size() == 1)
            _chars.back()->setParent(this);
        else
            _chars.back()->setParent(_chars[_chars.size() - 2]);

        _chars.back()->setRelativeY(0);
    }

    AssetsManager * am = AssetsManager::getInstance();
    for (int32_t i = 0; i < (int32_t) text.size(); i++) {
        _chars[i]->setTexture(am->getTextTexture(_font_name, _font_size, std::string(1, text[i]), _font_color));
        _chars[i]->setRelativeX(_chars[i]->getParent()->getWidth());
        _chars[i]->show();
    }

    for (int32_t i = text.size(); i < (int32_t) _chars.size(); i++)
        _chars[i]->hide();
}

uint16_t DynamicText::getWidth() const noexcept
{
    uint16_t total_width = 0;
    for (auto child : _chars) {
        if (child->isHide())
            break;
        total_width += child->getWidth();
    }
    return total_width;
}

uint16_t DynamicText::getHeight() const noexcept
{
    uint16_t total_height = 0;
    for (auto child : _chars) {
        if (child->isHide())
            break;
        total_height += child->getHeight();
    }
    return total_height;
}

DynamicText::DynamicText(const std::string & font_name, uint16_t font_size, const SDL_Color & font_color):
VisualComponent(0,0),
_font_color(font_color),
_font_size(font_size),
_font_name(font_name)
{

}

DynamicText::DynamicText(DynamicText & dt):
VisualComponent(0,0)
{

}

DynamicText::~DynamicText()
{

}