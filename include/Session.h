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
    std::vector<Watchable*> getContent();
    std::vector<BaseAction*> getAtionsLog();
    std::unordered_map<std::string, User*> getUserMap();
    User* getActiveUser();
private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    void copy(const Session& other);
    void clear();
};
#endif
