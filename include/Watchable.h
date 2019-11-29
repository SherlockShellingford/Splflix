#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>
#include "../include/Session.h"
class Session;


class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags);

    virtual ~Watchable();
    virtual Watchable* copy();
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    const long getId() const;
    int getLength() const;
    virtual void accept(Session s) const;
    virtual void accept( User s) const;
    std::vector<std::string> getTags() const;
    virtual std::string getName() const;
private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    virtual void accept(Session s) const;
    virtual Watchable* copy();
    virtual void accept(User s) const;
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    std::string getName() const;
    virtual void copy(Movie* into);
private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual void accept(Session s) const;
    virtual Watchable* copy();
    virtual void accept(User s) const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual void copy(Episode* into);
    std::string getSeriesname() const;
    int getSeason() const;
    int getEpisode() const;
    int getNextEpisodeId() const;
    std::string getName() const;
    static std::vector<Watchable*> createSeason(int startid,const std::string& seriesName,int length, int season, int numepisodes ,const std::vector<std::string>& tags);
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;


};

#endif