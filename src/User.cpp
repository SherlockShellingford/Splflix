//
// Created by esev on 20/11/2019.
//

#include "../include/User.h"


User::User(const std::string &name): name(name), history() {

}

std::string User::getName() const {
    return name;
}


std::vector<Watchable*> User::get_history() const {
    return this->history;
}

User* User::createUser(std::string type, std::string name ) {
    if(type=="len"){
        return new LengthRecommenderUser(name);
    }
    if(type=="rer"){
        return new RerunRecommenderUser(name);
    }
    if(type=="gen"){
        return new GenreRecommenderUser(name);
    }

}

void User::addWatchable(Watchable *w) {
    this->history.push_back(w);

}


int LengthRecommenderUser::abs(int x) {
    if(x<0){
        return -x;
    }
    return x;
}
LengthRecommenderUser::LengthRecommenderUser(const std::string &name) :User(name) {}

Watchable* LengthRecommenderUser::getRecommendation(Session &s) {

    Watchable* next=history[history.size()-1]->getNextWatchable(s);
    if(next != nullptr){

        return next;
    }

    std::vector<Watchable*> temp=s.getContent();
    int index=0;
    int min=0;

    if(!temp.empty()){
        min =abs(this->avg-temp[0]->getLength());

    }
    else{
        return nullptr;
    }
    for(int j=0;j<temp.size();++j) {
        int index=j;
        int min=0;


        min =abs(this->avg-temp[j]->getLength());


        for (int i = j; i < temp.size(); ++i) {
            int t = abs(this->avg - temp[i]->getLength());
            if (t <= min && temp[i] < temp[index]) {
                min = t;
                index = i;
            }
        }
        Watchable* switcher=temp[j];
        temp[j]=temp[index];
        temp[index]=switcher;
    }
    for(int i=0;i<temp.size();++i){
        bool didntwatchedalready=true;
        for(int j=0;j<this->history.size()-1;++j){
            if(temp[i]->getId()==history[j]->getId()){
                didntwatchedalready=false;
            }
        }
        if(didntwatchedalready){
            return temp[i];
        }
    }
    return nullptr;
}

RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name){
    this->index=-1;

}


Watchable* RerunRecommenderUser::getRecommendation(Session &s) {

    Watchable* next=history[history.size()-1]->getNextWatchable(s);

    if(next != nullptr){

        return next;
    }
    this->index=this->index+1;
    if(history.empty()){
        return nullptr;
    }

    return history[index%this->history.size()];

}

GenreRecommenderUser::GenreRecommenderUser(const std::string &name) : User(name), tags(), tagcount() {

}

void GenreRecommenderUser::addWatchable(Watchable *w) {
    this->history.push_back(w);
    for( std::string tag : w->getTags()){
        bool toadd=true;
        int index=-1;
        for(int i=0;i<this->tags.size();++i){
            if(this->tags[i]==tag){
                toadd=false;
                index=i;
            }
        }
        if(toadd){
            this->tags.push_back(tag);
            this->tagcount.push_back(1);
        }
        else{
            this->tagcount[index]=this->tagcount[index]+1;
        }
    }
}
std::string GenreRecommenderUser::lexo(std::string a, std::string b) {
    for(int i=0;i<a.length();++i){
        if(i==b.length()){
            return a;
        }
        if(a[i]>b[i]){
            return a;
        }
        if(a[i]<b[i]){
            return b;
        }
    }
    if(b.length()>a.length()){
        return b;
    }
}
std::pair<std::string,int> GenreRecommenderUser::getMostPopularTag( const Watchable *a)  {
    std::string populartaga=a->getTags()[0];
    int maxcount=0;
    std::vector<std::string> tempa=a->getTags();
    for(std::string tag : tempa){
        int count=-1;
        for(int j=0;j<this->tags.size();++j){
            if(this->tags[j]==tag){
                count=this->tagcount[j];
            }
        }

        if(count>maxcount||(count==maxcount && this->lexo(populartaga, tag)==tag)){
            maxcount=count;
            populartaga=tag;
        }



    }
    return std::pair<std::string, int>(populartaga,maxcount);
}

bool GenreRecommenderUser::compareTwoWatchables(std::pair<std::string, int> a, std::pair<std::string, int> b) {
    if(a.second>b.second){
        return true;
    }
    if(b.second>a.second){
        return false;
    }

    if(lexo(a.first,b.first)==a.first){
        return true;
    }
    return false;
}

Watchable* GenreRecommenderUser::getRecommendation(Session &s) {

    Watchable* next=history[history.size()-1]->getNextWatchable(s);

    if(next != nullptr){

        return next;
    }

    std::vector<Watchable*> content=s.getContent();
    std::vector<std::pair< Watchable*,std::pair<std::string, int>>> prioritylist;
    for(  Watchable* watchable: content){
        std::pair<std::string, int> populartagpair=this->getMostPopularTag(watchable);
        std::pair< Watchable*, std::pair<std::string, int>> toaddtopriority(watchable,populartagpair);
        prioritylist.push_back(toaddtopriority);
    }
    for(int j=0;j<prioritylist.size();++j) {
        int index=j;
        std::pair<std::string, int> min=prioritylist[0].second;




        for (int i = j; i < prioritylist.size(); ++i) {
            if (this->compareTwoWatchables(prioritylist[i].second, min)) {
                min = prioritylist[i].second;
                index = i;
            }
        }
        std::pair< Watchable*,std::pair<std::string, int>> switcher=prioritylist[j];
        prioritylist[j]=prioritylist[index];
        prioritylist[index]=switcher;
    }
    for(int i=0;i<prioritylist.size();++i){
        bool didntwatchedalready=true;
        for(int j=0;j<this->history.size()-1;++j){
            if(prioritylist[i].first->getId()==history[j]->getId()){
                didntwatchedalready=false;
            }
        }
        if(didntwatchedalready){
            return prioritylist[i].first;
        }
    }
    return nullptr;





}


