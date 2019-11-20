#include "../include/Session.h"
//
// Created by doorbellman on 19/11/2019.
//
#include "../include/Session.h"
Session::Session(const std::string &configFilePath) {

}

Session& Session::operator=(const Session& other) {
    if (this != &other){
        clear();
        copy(other);
    }
    return *this;
}   //copy assignment operator

Session::Session(const Session& other) {
    this->content = other.content;
    this->activeUser = other.activeUser;
    this->userMap = other.userMap;
    this->actionsLog = other.actionsLog;
}   //copy constructor

Session::~Session(){    //dont know if this works, there's probably a better way
    clear();
}   //destructor

Session::Session(Session&& other)
    : content(other.content), actionsLog(other.actionsLog), userMap(other.userMap), activeUser(other.activeUser){
    for (Watchable* w : content) {
        w = nullptr;
    }
    content.clear();
    for (BaseAction* a : actionsLog) {
        a = nullptr;
    }
    actionsLog.clear();
    for (auto s : userMap) {
        s.second = nullptr;
        userMap.erase(s.first);
    }
    userMap.clear();
    activeUser = nullptr;
}   //move constructor

Session& Session::operator=(Session&& other) {
    if (this != &other)
    {
        clear();
        copy(other);
        for (Watchable* w : content) {
            w = nullptr;
        }
        content.clear();
        for (BaseAction* a : actionsLog) {
            a = nullptr;
        }
        actionsLog.clear();
        for (auto s : userMap) {
            s.second = nullptr;
            userMap.erase(s.first);
        }
        userMap.clear();
        activeUser = nullptr;
    }

    return *this;
}

void Session::copy(const Session& other){
    this->content = other.content;
    this->activeUser = other.activeUser;
    this->userMap = other.userMap;
    this->actionsLog = other.actionsLog;
}

void Session::clear() {
    for (Watchable* w : content) {
        delete w;
    }
    content.clear();
    for (BaseAction* a : actionsLog) {
        delete a;
    }
    actionsLog.clear();
    for (auto s : userMap) {
        delete s.second;
        userMap.erase(s.first);
    }
    userMap.clear();
    delete activeUser;
    activeUser = nullptr;
}