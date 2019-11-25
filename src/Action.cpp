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
        sess.getUserMap().insert({name, User::createUser(sess.getInput()[sess.getInput().size()-1], name)}); //might need to delete it sometime later
        complete();
        return;
    }
    error("This user already exists");
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
        return;
    }
    error("There is no such user");
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
        delete sess.getUserMap()[name];
        sess.getUserMap().erase(name);
        complete();
        return;
    }
    error("There is no such user");
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
            User* t = User::createUser(sess.getInput()[sess.getInput().size()-1], sess.getUserMap()[source]->getName());
            sess.getUserMap().insert({target, t});//.insert(target, t);
            complete();
            return;
        }
        error("User " + target + " exists already");
        return;
    }
    error("User " + source + " does not exist");
}

std::string DuplicateUser::toString() const{
    if (getStatus() == COMPLETED){
        return "Duplicate user: COMPLETED";
    }
    return "Duplicate user: ERROR - " + getErrorMsg();
}

void PrintContentList::act(Session& sess){
    for (Watchable* w : sess.getContent()) {
        printf("%s\n", (w->toString()).c_str());
    }
    complete();
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
        printf("%s", string.c_str());
    }
    complete();
}

std::string PrintWatchHistory::toString() const{
    if (getStatus() == COMPLETED){
        return "Print watch history: COMPLETED";
    }
    return "Print watch history: ERROR - " + getErrorMsg();
}

void Watch::act(Session& sess){
    std::string w = "Watching ";

    w.append(sess.getContent()[atoi((sess.getInput()[sess.getInput().size()-1]).c_str())]->getName().c_str());
    printf("%s\n", w.c_str());
    sess.getActiveUser()->addWatchable(sess.getContent()[atoi(sess.getInput()[sess.getInput().size()-1].c_str())]);
    complete();
}

std::string Watch::toString() const{
    if (getStatus() == COMPLETED){
        return "Watch: COMPLETED";
    }
    return "Watch: ERROR - " + getErrorMsg();
}

void PrintActionsLog::act(Session& sess){
    for (BaseAction* a : sess.getActionsLog()) {
        printf("%s", (a->toString()).c_str());
    }
    complete();
}

std::string PrintActionsLog::toString() const{
    if (getStatus() == COMPLETED){
        return "Print actions log: COMPLETED";
    }
    return "Print actions log: ERROR - " + getErrorMsg();
}

void Exit::act(Session& sess){
    complete();
}

std::string Exit::toString() const{
    if (getStatus() == COMPLETED){
        return "Exit: COMPLETED";
    }
    return "Exit: ERROR - " + getErrorMsg();
}