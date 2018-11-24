/*
 * CS 106B/X
 * This file declares a general user interface abstract class.
 *
 * @version 2018/11/10
 * - 18au 106B initial version
 *
 * @author Marty Stepp
 */

#ifndef _ui_h
#define _ui_h

#include <string>

class UI {
public:
    UI();
    virtual ~UI();
    virtual void print(const std::string& message) = 0;
    virtual std::string readLine(const std::string& prompt = "",
                                 const std::string& initialValue = "") = 0;
    virtual bool readBoolean(const std::string& prompt = "") = 0;
};

class ConsoleUI : public UI {
public:
    ConsoleUI();
    virtual ~ConsoleUI();
    virtual void print(const std::string& message);
    virtual std::string readLine(const std::string& prompt = "",
                                 const std::string& initialValue = "");
    virtual bool readBoolean(const std::string& prompt = "");
};

#endif // _ui_h
