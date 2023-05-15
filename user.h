#ifndef USER_H
#define USER_H

#include <QMultiMap>

class User
{
public:
    User(int ID);
    int getID();
    QMultiMap<QString, int> UserIntMap;
private:
    int ID;
};

#endif // USER_H
