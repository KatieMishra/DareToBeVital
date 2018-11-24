/*
 * CS 106B/X
 * This file declares a general user interface abstract class.
 *
 * @version 2018/11/10
 * - 18au 106B initial version
 *
 * @author Marty Stepp
 */

#include "ui.h"
#include <iostream>
#include "simpio.h"

UI::UI() {
    // empty
}

UI::~UI() {
    // empty
}

ConsoleUI::ConsoleUI() {
    // empty
}

ConsoleUI::~ConsoleUI() {
    // empty
}

void ConsoleUI::print(const std::string& message) {
    std::cout << message << std::endl;
}

std::string ConsoleUI::readLine(const std::string& prompt, const std::string& initialValue) {
    return getLine(prompt);
}

bool ConsoleUI::readBoolean(const std::string& prompt) {
    return getYesOrNo(prompt);
}
