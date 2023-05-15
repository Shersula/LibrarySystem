#ifndef USER_H
#define USER_H

#include <QMultiMap>

class User
{
public:
    User(int ID, int Role);
    int getID();
    int getRole();
    QMultiMap<QString, int> UserIntMap;
private:
    int ID;
    int Role;
};

#endif // USER_H
