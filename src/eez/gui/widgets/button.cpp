/*
 * EEZ Modular Firmware
 * Copyright (C) 2015-present, Envox d.o.o.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#if OPTION_DISPLAY

#include <eez/util.h>

#include <eez/gui/gui.h>
#include <eez/gui/widgets/button.h>

namespace eez {
namespace gui {

FixPointersFunctionType BUTTON_fixPointers = [](Widget *widget, Assets *assets) {
    ButtonWidget *buttonWidget = (ButtonWidget *)widget->specific;
    buttonWidget->text = (const char *)((uint8_t *)(void *)assets->document + (uint32_t)buttonWidget->text);
};

EnumFunctionType BUTTON_enum = nullptr;

DrawFunctionType BUTTON_draw = [](const WidgetCursor &widgetCursor) {
    const Widget *widget = widgetCursor.widget;
    const ButtonWidget *button_widget = GET_WIDGET_PROPERTY(widget, specific, const ButtonWidget *);

    widgetCursor.currentState->size = sizeof(WidgetState);
    widgetCursor.currentState->flags.enabled = get(widgetCursor.cursor, button_widget->enabled).getInt() ? 1 : 0;

    const Style *style = getStyle(widgetCursor.currentState->flags.enabled ? widget->style : button_widget->disabledStyle);

    widgetCursor.currentState->flags.blinking = g_isBlinkTime && (isBlinking(widgetCursor, widget->data) || styleIsBlink(style));
    widgetCursor.currentState->data = widget->data ? get(widgetCursor.cursor, widget->data) : 0;

    bool refresh =
        !widgetCursor.previousState ||
        widgetCursor.previousState->flags.active != widgetCursor.currentState->flags.active ||
        widgetCursor.previousState->flags.enabled != widgetCursor.currentState->flags.enabled ||
        widgetCursor.previousState->flags.blinking != widgetCursor.currentState->flags.blinking ||
        widgetCursor.previousState->data != widgetCursor.currentState->data;

    if (refresh) {
        if (widget->data) {
            if (widgetCursor.currentState->data.isString()) {
                drawText(widgetCursor.currentState->data.getString(), -1, widgetCursor.x,
                         widgetCursor.y, (int)widget->w, (int)widget->h, style,
                         widgetCursor.currentState->flags.active,
                         widgetCursor.currentState->flags.blinking, false, nullptr, nullptr, nullptr, nullptr);
            } else {
                drawText(GET_WIDGET_PROPERTY(button_widget, text, const char *), -1, widgetCursor.x, widgetCursor.y, (int)widget->w, (int)widget->h,
                         style, widgetCursor.currentState->flags.active,
                         widgetCursor.currentState->flags.blinking, false, nullptr, nullptr, nullptr, nullptr);
            }
        } else {
            drawText(GET_WIDGET_PROPERTY(button_widget, text, const char *), -1, widgetCursor.x, widgetCursor.y, (int)widget->w, (int)widget->h,
                     style, widgetCursor.currentState->flags.active,
                     widgetCursor.currentState->flags.blinking, false, nullptr, nullptr, nullptr, nullptr);
        }
    }
};

OnTouchFunctionType BUTTON_onTouch = nullptr;

OnKeyboardFunctionType BUTTON_onKeyboard = nullptr;

} // namespace gui
} // namespace eez

#endif
