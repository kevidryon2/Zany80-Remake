#include <Zany80/API/graphics.h>
#include "stretchy_buffer.h"
#include <stdio.h>

#include "Zany80/internal/graphics.h"

#include <IMUI/IMUI.h>
#include <Gfx/Gfx.h>
using Oryol::Gfx;

static void render_widget(widget_t *widget) {
    switch (widget->type) {
        case label:
            widget->_label.wrapped ? ImGui::TextWrapped("%s", widget->label) : ImGui::Text("%s", widget->label);
            break;
        case button:
            if (ImGui::Button(widget->label)) {
                if (widget->button.handler != NULL) {
                    widget->button.handler();
                }
            }
            break;
        case menu_item:
            if (ImGui::MenuItem(widget->label)) {
                if (widget->button.handler != NULL) {
                    widget->button.handler();
                }
            }
            break;
        case checkbox:
            if (ImGui::Checkbox(widget->label, widget->checkbox.value)) {
                if (widget->checkbox.handler != NULL) {
                    widget->checkbox.handler();
                }
            }
            break;
        case group:
            for (int i = 0; i < sb_count(widget->_group.widgets); i++) {
                render_widget(widget->_group.widgets[i]);
                if (widget->_group.orientation == horizontal) {
                    ImGui::SameLine();
                }
            }
            break;
        case custom:
            widget->custom.render();
            break;
    }
}

void render_window(window_t *window) {
    ImGuiWindowFlags flags = 0;
    if (sb_count(window->menus)) {
        flags |= ImGuiWindowFlags_MenuBar;
    }
    if (window == get_root()) {
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(Gfx::DisplayAttrs().WindowWidth, Gfx::DisplayAttrs().WindowHeight));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0,0));
        flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings;
    }
    if (window->titlebar == false) {
        flags |= ImGuiWindowFlags_NoTitleBar;
    }
    if (window->minX != -1 || window->minY != -1 || window->maxX != -1 || window->maxY != -1) {
        float minX = window->minX == -1 ? 0 : window->minX;
        float minY = window->minY == -1 ? 0 : window->minY;
        float maxX = window->maxX == -1 ? FLT_MAX : window->maxX;
        float maxY = window->maxY == -1 ? FLT_MAX : window->maxY;
        ImGui::SetNextWindowSizeConstraints(ImVec2(minX, minY), ImVec2(maxX, maxY));
    }
    if (window->absX != -1 && window->absY != -1) {
        ImGui::SetNextWindowPos(ImVec2(window->absX, window->absY));
        window->absX = window->absY = -1;
    }
    if (ImGui::Begin(window->name, NULL, flags)) {
        if (window->initialX != -1 && window->initialY != -1) {
            ImGui::SetWindowSize(ImVec2(window->initialX, window->initialY), ImGuiCond_FirstUseEver);
        }
        if (window == get_root()) {
            ImGui::PopStyleVar(2);
        }
        if (sb_count(window->menus)) {
            if (ImGui::BeginMenuBar()) {
                for (int i = 0; i < sb_count(window->menus); i++) {
                    if (ImGui::BeginMenu(window->menus[i]->label)) {
                        for (int j = 0; j < sb_count(window->menus[i]->widgets); j++) {
                            render_widget(window->menus[i]->widgets[j]);
                        }
                        ImGui::EndMenu();
                    }
                }
                ImGui::EndMenuBar();
            }
        }
        for (int i = 0; i < sb_count(window->widgets); i++) {
            render_widget(window->widgets[i]);
        }
    }
    ImGui::End();
}

void window_get_pos(window_t *window, float *x, float *y) {
    if (ImGui::Begin(window->name)) {
        auto pos = ImGui::GetWindowPos();
        if (x)
            *x = pos.x;
        if (y)
            *y = pos.y;
    }
    ImGui::End();
}

void window_get_size(window_t *window, float *x, float *y) {
    if (ImGui::Begin(window->name)) {
        auto size = ImGui::GetWindowSize();
        if (x)
            *x = size.x;
        if (y)
            *y = size.y;
    }
    ImGui::End();
}