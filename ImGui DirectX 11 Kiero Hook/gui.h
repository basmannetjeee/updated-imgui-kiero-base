#pragma once
#include "sdk/entities/entity.h"

namespace menu {
	enum class State {
		PRE_MENU,
		MAIN_TAB
	};

	State currentstate = State::PRE_MENU;
    int tab = 1;

	bool menuOpen = true;

    ImVec2 windowSize(350, 250);

    bool isBhop = false;
}

void TextCenteredOnLine(const char* label)
{
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(label).x) * 0.5f);
    ImGui::Text(label);
}

bool ButtonCenteredOnLine(const char* label)
{
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(label).x - ImGui::GetStyle().FramePadding.x * 2) * 0.5f);
    return ImGui::Button(label);
}

bool InputTextCenteredOnLine(const char* label, char* input, float inputWidth)
{
    if (inputWidth == NULL)
        inputWidth = ImGui::GetWindowSize().x / 2;

    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - inputWidth) * 0.5f);
    ImGui::PushItemWidth(inputWidth);

    bool inputChanged = ImGui::InputText("", input, 256);

    ImGui::PopItemWidth();

    return inputChanged;
}

bool CheckboxCenteredOnLine(const char* label, bool* toggle) {
    if (label == NULL)
        label = "CHECKBOX LABEL";

    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(label).x - ImGui::GetStyle().FramePadding.x * 2) * 0.5);
    return ImGui::Checkbox(label, toggle);
}

void preMenu() {
    ImGui::Dummy(ImVec2(0, menu::windowSize.y / 2 / 4 ));
    TextCenteredOnLine("Aglaea CS2");
    TextCenteredOnLine("BUILD [DEV]");
    ImGui::Dummy(ImVec2(0, 50.0f));
    if (ButtonCenteredOnLine("ok")) {
        menu::currentstate = menu::State::MAIN_TAB;
    }
}

void tablist() {
    if (ImGui::Button("Main", ImVec2(menu::windowSize.x / 4 - 9, 20))) {
        menu::tab = 1;
    } ImGui::SameLine();

    if (ImGui::Button("Visual", ImVec2(menu::windowSize.x / 4 - 9, 20))) {
        menu::tab = 2;
    } ImGui::SameLine();

    if (ImGui::Button("Movement", ImVec2(menu::windowSize.x / 4 - 9, 20))) {
        menu::tab = 3;
    } ImGui::SameLine();

    if (ImGui::Button("Exploits", ImVec2(menu::windowSize.x / 4 - 9, 20))) {
        menu::tab = 4;
    }
}

namespace tabs {
    void mainTab() {
        if (ImGui::Button("Print playernames")) {
            entity::PrintPlayerNames();
        }
    }
    void visualsTab() {

    }
    void movementTab() {
        ImGui::Checkbox("bhop", &menu::isBhop);
    }
    void exploitTab() {

    }

}

