//
// Created by esev on 20/11/2019.
//

#include "../include/User.h"


User::User(const std::string &name): name(name) {

}

std::string User::getName() const {
    return name;
}

std::vector<Watchable*> User::get_history() const {
    return this->history;
}

User* User::createUser(std::string type, std::string name ) {
    if(type.compare("len")){
        return new LengthRecommenderUser(name);
    }
    if(type.compare("rer")){
        return new RerunRecommenderUser(name);
    }
    if(type.compare("gen")){
        return new GenreRecommenderUser(name);
    }

}

void User::addWatchable(Watchable *w) {
    history.push_back(w);
}


int LengthRecommenderUser::abs(int x) {
    if(x<0){
        return -x;
    }
    return x;
}
LengthRecommenderUser::LengthRecommenderUser(const std::string &name) :User(name) {}

Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    std::vector<Watchable*> temp=s.getContent();
    int index=0;
    int min=0;

    if(!temp.empty()){
        min =abs(this->avg-temp[0].length());

    }
    else{
        return nullptr;
    }
    for(int i=0;i<temp.size();++i){
        int t=abs(this->avg-temp[i].length());
        if(t<=min &&temp[i]<temp[index]){
            min=t;
            index=i;
        }
    }
    return temp[index];
}

RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name){
    this->index=-1;

}


Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
    this->index=this->index+1;

    if(this->history.empty()){
        return nullptr;
    }
    else{

        return this->history[index%this->history.size()];

    }
}

