#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "../include/Action.h"
#include "../include/Watchable.h"

class Watchable;
class Session;
class BaseAction;
//class DuplicateUser;


class User{
public:
    User(const std::string& name);
    virtual~User();
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    static User* createUser(std::string type, std::string name);
    virtual User* copy();
    virtual void addWatchable(Watchable* w);
    virtual void getType(DuplicateUser*, Session&) = 0;
protected:
    std::vector<Watchable*> history;
private:
    std::string name;



};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual User* copy();
    virtual void addWatchable(Watchable* w);
    virtual void getType(DuplicateUser* , Session&);
private:
    int abs(int x);

    int avg;

};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual void getType(DuplicateUser*, Session&);
    virtual User* copy();
private:
    int index;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual void addWatchable(Watchable* w);
    virtual User* copy();
    virtual void getType(DuplicateUser*, Session&);
private:
    std::pair<std::string, int>  getMostPopularTag ( const Watchable* a);
    std::string lexo(std::string a, std::string b);
    bool compareTwoWatchables(std::pair<std::string, int> a,std::pair<std::string, int> b);
    std::vector<std::string> tags;
    std::vector<int> tagcount;
};

#endif
