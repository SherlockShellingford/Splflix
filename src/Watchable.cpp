//
// Created by esev on 20/11/2019.
//
#include "../include/Watchable.h"
#include "../include/Session.h"
//todo: delete the , in the end of the toString

Watchable::Watchable(long id, int length, const std::vector<std::string> &tags) : id(id), tags() {

    this->length=length;
    for(int i=0;i<tags.size();++i) {
        this->tags.push_back(tags[i]);
    }


}
std::string Watchable::getName() const {
    return nullptr;
}

Watchable::~Watchable() {

}

const long Watchable::getId() const {
    return this->id;
}

int Watchable::getLength() const {
    return this->length;
}

std::vector<std::string> Watchable::getTags() const {
    return tags;
}

Movie::Movie(long id, const std::string &name, int length, const std::vector <std::string> &tags) : Watchable(id,length,tags) {
    this->name=name;

}

std::string Movie::getName() const {
    return this->name;
}

std::string Movie::toString() const {
    std::vector<std::string> temp=this->getTags();
    std::string* t=new std::string[temp.size()];
    std::string ret;
    ;
    ret+=std::to_string(this->getId());
    ret+=". ";
    ret+=""+this->name ;
    ret+=" ";
    ret+=std::to_string(this->getLength());
    ret+= " minutes [";
    for(int i=0;i<temp.size();++i){
        ret+=temp[i]+", ";
    }
    ret+="]";
    return ret;
}

Watchable* Movie::getNextWatchable(Session&) const {
    return nullptr;
}

Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags): Watchable(id,length,tags) {
    this->seriesName=seriesName;
    this->season=season;
    this->episode=episode;
    this->nextEpisodeId=id+1;
}

int Episode::getEpisode() const {
    return episode;
}

int Episode::getNextEpisodeId() const {
    return nextEpisodeId;
}

int Episode::getSeason() const {
    return season;
}

std::string Episode::getSeriesname() const {
    return seriesName;
}

std::string Episode::toString() const {
    std::vector<std::string> temp=this->getTags();
    std::string* t=new std::string[temp.size()];
    std::string ret=std::to_string(this->getId())+". "+ this->seriesName+" S";
    ret+=""+std::to_string(this->season);
    ret+=" E"+ std::to_string(this->episode);
    ret +=" "+std::to_string(this->getLength());
    ret +=" minutes [";
    for(int i=0;i<temp.size();++i){
        ret+=temp[i]+", ";
    }
    ret+="]";
    return ret;
}

Watchable* Episode::getNextWatchable(Session & s) const {
    std::vector<Watchable*> temp=s.getContent();
    for(int i=0;i<temp.size();++i){
        if(temp[i]->getId()==this->getNextEpisodeId()){
            return temp[i];
        }
    }
    return nullptr;
}

std::string Episode::getName() const {
    std::string ret=std::to_string(this->getId())+". "+ this->seriesName+" S";
    ret+=(""+std::to_string(this->season));
    ret+=" E"+std::to_string(this->episode);
    return ret;
}

std::vector<Watchable*> Episode::createSeason(long startid, const std::string &seriesName, int length, int season,
                                              int episodesnum, const std::vector<std::string> &tags) {
    std::vector<Watchable*> ret;

    for(int i=1;i<=episodesnum;++i){
        ret.push_back(new Episode(startid+i-1,seriesName, length,season,episodesnum,tags));
    }
    return ret;
}








