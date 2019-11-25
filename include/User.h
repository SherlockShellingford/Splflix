#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "Watchable.h"
#include "Session.h"


class User{
public:
    User(const std::string& name);
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    static User* createUser(std::string type, std::string name);
    virtual void addWatchable(Watchable* w);

protected:
    std::vector<Watchable*> history;
private:
    const std::string name;



};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual void addWatchable(Watchable* w);

private:
    int abs(int x);
    int avg;

};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);

private:
    int index;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual void addWatchable(Watchable* w);
private:
    std::pair<std::string, int>  getMostPopularTag ( const Watchable* a);
    std::string lexo(std::string a, std::string b);
    bool compareTwoWatchables(std::pair<std::string, int> a,std::pair<std::string, int> b);
    std::vector<std::string> tags;
    std::vector<int> tagcount;
};

#endif