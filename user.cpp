#include "user.h"

User::User(int ID, int Role)
{
    this->ID = ID;
    this->Role = Role;
}

int User::getID(){return this->ID;}

int User::getRole(){return this->Role;}
