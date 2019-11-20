#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    Session(const Session& s);
    ~Session();
    Session& operator=(const Session&);
    Session(Session&& other);
    Session& operator=(Session&&);
    void start();
    std::vector<Watchable*> getContent() const;
    std::vector<BaseAction*> getAtionsLog() const;
    std::unordered_map<std::string, User*> getUserMap() const;
    User* getActiveUser() const;
    std::vector<std::string> getInput() const;
private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    void copy(const Session& other);
    void clear();
    std::vector<std::string> input;
};
#endif