#include "../include/Session.h"
//
// Created by doorbellman on 19/11/2019.
//
#include <fstream>
#include "../include/Watchable.h"
#include "../include/Action.h"
#include "../include/User.h"
#include "../include/json.hpp"
using json = nlohmann::json;
Session::Session(const std::string &configFilePath) : content(), actionsLog(), userMap(), activeUser(), input() {
    this->activeUser=User::createUser("len", "");
    this->userMap.insert({activeUser->getName(), activeUser});
    std::ifstream  stream(configFilePath);
    json config;
    stream >> config;
    int i = 0;
    while(config["movies"][i] != nullptr){
        json m = config["movies"][i];
        int j = 0;
        std::vector<std::string> tags;
        while (m["tags"][j] != nullptr){
            tags.push_back(m["tags"][j]);
            j++;
        }
        content.push_back(new Movie((long)i + 1, m["name"], m["length"], tags));
        i++;
    }
    int c = 0;
    while(config["tv_series"][c] != nullptr){
        json m = config["tv_series"][c];
        int j = 0;
        std::vector<std::string> tags;
        while (m["tags"][j] != nullptr){
            tags.push_back(m["tags"][j]);
            j++;
        }
        for (int k = 0; m["seasons"][k] != nullptr; ++k) {
            //for (int l = 0; l < m["seasons"][k]; ++l) {
            //    content.push_back(new Episode(i + 1, m["name"], m["episode_length"], k, l + 1, tags));
            //    i++;
            //}
            int sLen = m["seasons"][k];
            std::vector<Watchable*> tmp = Episode::createSeason(i + 1, m["name"], m["episode_length"], k + 1, sLen, tags);
            this->content.insert(std::end(content), std::begin(tmp), std::end(tmp));
            i += sLen;
        }
        c++;
    }
}

Session& Session::operator=(const Session& other){
    if (this != &other){
        clear();
        copy(other);
    }
    return *this;
}   //copy assignment operator

Session::Session(const Session& other) : content(), actionsLog(), userMap(), activeUser(), input() {
    this->content = other.content;
    for (int j = 0; j < other.content.size(); ++j) {
        this->content.push_back(other.content[j]->copy());
    }
    this->activeUser = other.activeUser->copy();
    for (std::pair<std::string, User*>  p: other.userMap) {
        this->userMap.insert({p.first, p.second->copy()});
    }
    for (int i = 0; i < other.getActionsLog().size(); ++i) {
        this->actionsLog.push_back(other.actionsLog[i]->copyAction(*this));
    }
}   //copy constructor

Session::~Session(){    //dont know if this works, there's probably a better way
    clear();
}   //destructor

Session::Session(Session&& other)
        : content(other.content), actionsLog(other.actionsLog), userMap(other.userMap), activeUser(other.activeUser), input(){
    content.clear();
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
        content.clear();
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
    this->activeUser=userMap[""];
    printf("SPLFLIX is now on!\n");
    while(true){
        input.clear();
        std::string in;
        std::getline(std::cin, in);
        int  s = 0;
        for (unsigned int i = 0; i < in.length(); ++i) {
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
            printf("No such command\n");
        }
    }
}

void Session::copy(const Session& other){
    for (int i = 0; i < other.content.size(); ++i) {
        this->content[i]=other.content[i]->copy();

    }
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
    for (std::pair<std::string, User*> s : userMap) {
        delete s.second;
    }
    userMap.clear();
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
    return this->activeUser;
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
    std::string in;
    do {
        Watch *action = new Watch();
        action->act(*this);
        std::string w;
        w = "We recommend watching ";
        Watchable* rec = this->activeUser->getRecommendation(*this);
        w.append(rec->toString().c_str());
        w.append(" continue watching? [y/n]\n");
        printf("%s", w.c_str());
        std::getline(std::cin, in);
        this->input[this->input.size() - 1] = std::to_string(rec->getId());
    }
    while (in == "y" || in == "Y");
}

void Session::log(){
    PrintActionsLog *action = new PrintActionsLog();
    action->act(*this);
}

void Session::exit(){
    Exit *action = new Exit();
    action->act(*this);
}

void Session::addUser(std::string name, User* u){
    this->userMap.insert({name, u});
}

void Session::removeUser(std::string name){
    this->userMap.erase(name);
}

void Session::addLog(BaseAction* action){
    this->actionsLog.push_back(action);
}
