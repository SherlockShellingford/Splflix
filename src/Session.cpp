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
}   //move assignment operator

void Session::start() {
    while(true){
        std::string in;
        std::getline(std::cin, in);
        int  s = 0;
        for (int i = 0; i < in.length(); ++i) {
            if (in[i] == ' '){
                input.push_back(in.substr(s, i-s));
                s = i + 1;
            }
        }
        input.push_back(in.substr(s, in.length()-s));
        if (input[0] == "createuser"){
            createU();
        }
        else if (input[0] == "changeuser"){
            changeU();
        }
        else if (input[0] == "deleteuser"){
            deleteU();
        }
        else if (input[0] == "dupuser"){
            dupU();
        }
        else if (input[0] == "content"){
            cont();
        }
        else if (input[0] == "watchhist"){
            hist();
        }
        else if (input[0] == "watch"){
            wat();
        }
        else if (input[0] == "log"){
            log();
        }
        else if (input[0] == "exit"){
            exit();
            break;
        }
        else{
            printf("No such command");
        }
    }
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

std::vector<Watchable*> Session::getContent() const{
    return content;
}

std::vector<BaseAction*> Session::getActionsLog() const{
    return actionsLog;
}

std::unordered_map<std::string, User*> Session::getUserMap() const{
    return userMap;
}

User* Session::getActiveUser() const{
    return activeUser;
}

std::vector<std::string> Session::getInput() const{
    return input;
}

void Session::setActiveUser(User* other){
    activeUser = other;
}

void Session::createU(){
    CreateUser *action = new CreateUser();
    action->act(*this);
}

void Session::changeU(){
    ChangeActiveUser *action = new ChangeActiveUser();
    action->act(*this);
}

void Session::deleteU(){
    DeleteUser *action = new DeleteUser();
    action->act(*this);
}

void Session::dupU(){
    DuplicateUser *action = new DuplicateUser();
    action->act(*this);
}

void Session::cont(){
    PrintContentList *action = new PrintContentList();
    action->act(*this);
}

void Session::hist(){
    PrintWatchHistory *action = new PrintWatchHistory();
    action->act(*this);
}

void Session::wat(){
    Watch *action = new Watch();
    action->act(*this);
}

void Session::log(){
    PrintActionsLog *action = new PrintActionsLog();
    action->act(*this);
}

void Session::exit(){
    Exit *action = new Exit();
    action->act(*this);
}