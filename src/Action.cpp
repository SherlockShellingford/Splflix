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
        sess.getUserMap().insert(name, User::createUser(name, sess.getInput()[sess.getInput().size()-1]));   //might need to delete it sometime later
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
            User* t = new User(sess.getUserMap()[source]);
            sess.getUserMap().insert(target, t);
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
        printf((*w.toString()).c_str());
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
    printf("Watch history for " + sess.getActiveUser());
    for (Watchable* w : sess.getActiveUser().getWatchHistory()) {
        printf((w->getId().toString()).c_str() + " " + (w->getName().toString()).c_str());
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
    printf("Watching " + sess.getContent()[sess.getInput()[sess.getInput().size()-1]]);
    sess.getActiveUser()->addWatchable(sess.getContent()[sess.getInput()[sess.getInput().size()-1]]);
    printf("We recommend watching " + sess.getActiveUser()->getRecommendation(sess) + " continue watching? [y/n]")
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
        printf((*a.toString()).c_str());
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