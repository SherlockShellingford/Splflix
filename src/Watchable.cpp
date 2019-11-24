//
// Created by esev on 20/11/2019.
//
#include "../include/Watchable.h"


Watchable::Watchable(long id, int length, const std::vector<std::string> &tags) : id(id), tags() {

    this->length=length;
    for(int i=0;i<tags.size();++i) {
        this->tags.push_back(tags[i]);
    }


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
    ret.append("" + this->getId());  ret.append( ". " ); ret.append( this->name );ret.append( " "); ret.append( ""+ this->getLength()); ret+= " minutes [";
    for(int i=0;i<temp.size();++i){
        ret+=t[i]+", ";
    }
    ret+="]";
    return ret;
}

Watchable* Movie::getNextWatchable(Session&) const {
    return nullptr;
}

Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags): Watchable(id,length,tags) {
    this->season=season;
    this->episode=episode;
    this->nextEpisodeId=episode+1;
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
    std::string ret=this->getId()+". "+ this->seriesName+" S";ret.append(""+this->season);ret+=" E"+this->episode;ret +=" "+this->getLength(); ret +=" minutes [";
    for(int i=0;i<temp.size();++i){
        ret+=t[i]+", ";
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

std::string Episode::getName() {
    std::string ret=this->getId()+". "+ this->seriesName+" S";ret.append(""+this->season);ret+=" E"+this->episode;
    return ret;
}








