//
// Created by doorbellman on 20/11/2019.
//
#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/User.h"
BaseAction::BaseAction() {
    this->errorMsg = "";
    this->status = PENDING;
}

ActionStatus BaseAction::getStatus() const{
    return status;
}

void BaseAction::complete(){
    status = COMPLETED;
}

void BaseAction::error(const std::string& errorMsg){
    status = ERROR;
    this->errorMsg = errorMsg;
}

std::string BaseAction::getErrorMsg() const{
    return errorMsg;
}

void CreateUser::act(Session& sess){
    std::string name = sess.getInput()[sess.getInput().size()-2];
    if (sess.getUserMap().find(name) == sess.getUserMap().end()){
        sess.addUser(name, User::createUser(sess.getInput()[sess.getInput().size()-1], name));// //might need to delete it sometime later
        complete();
        sess.addLog(this);
        return;
    }
    error("This user already exists");
    sess.addLog(this);
}

std::string CreateUser::toString() const{
    if (getStatus() == COMPLETED){
        return "Create user: COMPLETED";
    }
    return "Create user: ERROR - " + getErrorMsg();
}

void ChangeActiveUser::act(Session& sess){
    std::string name = sess.getInput()[sess.getInput().size()-1];
    if (sess.getUserMap().find(name) != sess.getUserMap().end()){
        sess.setActiveUser(sess.getUserMap()[name]);
        complete();
        sess.addLog(this);
        return;
    }
    error("There is no such user");
    sess.addLog(this);
}

std::string ChangeActiveUser::toString() const{
    if (getStatus() == COMPLETED){
        return "Change active user: COMPLETED";
    }
    return "Change active user: ERROR - " + getErrorMsg();
}

void DeleteUser::act(Session& sess){
    std::string name = sess.getInput()[sess.getInput().size()-1];
    if (sess.getUserMap().find(name) != sess.getUserMap().end()){
        sess.removeUser(name);//delete sess.getUserMap()[name];
        sess.getUserMap().erase(name);
        complete();
        sess.addLog(this);
        return;
    }
    error("There is no such user");
    sess.addLog(this);
}

std::string DeleteUser::toString() const{
    if (getStatus() == COMPLETED){
        return "Delete user: COMPLETED";
    }
    return "Delete user: ERROR - " + getErrorMsg();
}

void DuplicateUser::act(Session& sess){
    std::string source = sess.getInput()[sess.getInput().size()-2];
    std::string target = sess.getInput()[sess.getInput().size()-1];
    if (sess.getUserMap().find(source) != sess.getUserMap().end()){
        if (sess.getUserMap().find(target) == sess.getUserMap().end()){
//            User* t = User::createUser(sess.getUserMap()[source]->getType(), sess.getUserMap()[source]->getName());
//            sess.addUser(target, t);//sess.getUserMap().insert({target, t});//.insert(target, t);
            sess.getUserMap()[source]->getType(this, sess);
            complete();
            sess.addLog(this);
            return;
        }
        error("User " + target + " exists already");
        sess.addLog(this);
        return;
    }
    error("User " + source + " does not exist");
    sess.addLog(this);
}

std::string DuplicateUser::toString() const{
    if (getStatus() == COMPLETED){
        return "Duplicate user: COMPLETED";
    }
    return "Duplicate user: ERROR - " + getErrorMsg();
}

void DuplicateUser::lenUser(User* source, Session& sess) {
    User* t = User::createUser("len", source->getName());
    sess.addUser(sess.getInput()[sess.getInput().size()-1], t);
    for (int i = 0; i < source->get_history().size(); ++i) {
        t->addWatchable(source->get_history()[i]);
    }
}

void DuplicateUser::genUser(User* source, Session& sess) {
    User* t = User::createUser("gen", source->getName());
    sess.addUser(sess.getInput()[sess.getInput().size()-1], t);
    for (int i = 0; i < source->get_history().size(); ++i) {
        t->addWatchable(source->get_history()[i]);
    }
}

void DuplicateUser::rerUser(User* source, Session& sess) {
    User* t = User::createUser("rer", source->getName());
    sess.addUser(sess.getInput()[sess.getInput().size()-1], t);
    for (int i = 0; i < source->get_history().size(); ++i) {
        t->addWatchable(source->get_history()[i]);
    }
}

void PrintContentList::act(Session& sess){
    for (Watchable* w : sess.getContent()) {
        printf("%s\n", (w->toString()).c_str());
    }
    complete();
    sess.addLog(this);
}

std::string PrintContentList::toString() const{
    if (getStatus() == COMPLETED){
        return "Print content list: COMPLETED";
    }
    return "Print content list: ERROR - " + getErrorMsg();
}

void PrintWatchHistory::act(Session& sess){
    std::string string = "Watch history for ";
    string.append((sess.getActiveUser()->getName()));
    printf("%s",string.c_str());
    for (Watchable* w : sess.getActiveUser()->get_history()) {
        string = w->getId();
        string.append(" ");
        string.append(w->getName().c_str());
        printf("%s\n", string.c_str());
    }
    complete();
    sess.addLog(this);
}

std::string PrintWatchHistory::toString() const{
    if (getStatus() == COMPLETED){
        return "Print watch history: COMPLETED";
    }
    return "Print watch history: ERROR - " + getErrorMsg();
}

void Watch::act(Session& sess){
    std::string w = "Watching ";

    w.append(sess.getContent()[atoi((sess.getInput()[sess.getInput().size()-1]).c_str()) - 1]->getName().c_str());
    printf("%s\n", w.c_str());
    sess.getActiveUser()->addWatchable(sess.getContent()[atoi(sess.getInput()[sess.getInput().size()-1].c_str()) - 1]);
    complete();
    sess.addLog(this);
}

std::string Watch::toString() const{
    if (getStatus() == COMPLETED){
        return "Watch: COMPLETED";
    }
    return "Watch: ERROR - " + getErrorMsg();
}

void PrintActionsLog::act(Session& sess){
    for (BaseAction* a : sess.getActionsLog()) {
        printf("%s\n", (a->toString()).c_str());
    }
    complete();
    sess.addLog(this);
}

std::string PrintActionsLog::toString() const{
    if (getStatus() == COMPLETED){
        return "Print actions log: COMPLETED";
    }
    return "Print actions log: ERROR - " + getErrorMsg();
}

void Exit::act(Session& sess){
    complete();
    sess.addLog(this);
}

std::string Exit::toString() const{
    if (getStatus() == COMPLETED){
        return "Exit: COMPLETED";
    }
    return "Exit: ERROR - " + getErrorMsg();
}