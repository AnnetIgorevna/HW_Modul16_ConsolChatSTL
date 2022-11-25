#pragma once
#include "Sha1.h"

class User
{
public:
	enum enUserStatus {
		free, // свободен
		engaged, //занят
		deleted //удален
	};

    User() :
        _login(),
        _pass_sha1_hash(0),
        _status(enUserStatus::free) {}

    //~User() {
    //    if (_pass_sha1_hash != 0)
    //        delete[] _pass_sha1_hash;
    //}

    User(std::string& login, uint* sh1) {
        _login = login;
        _pass_sha1_hash = sh1;
        _status = enUserStatus::engaged;
    }

    User& operator = (const User& other) {
        _login = other._login;

        if (_pass_sha1_hash != 0)
            delete[] _pass_sha1_hash;
        _pass_sha1_hash = new uint[SHA1HASHLENGTHUINTS];

        memcpy(_pass_sha1_hash, other._pass_sha1_hash, SHA1HASHLENGTHBYTES);
        _status = other._status;

        return *this;
    }

    const std::string& getUserLogin() const { return _login; }
    int getUserStatus() { return _status; }
    uint* getUserPassword() const { return _pass_sha1_hash; }
    void setUserPassword(uint* password) { memcpy(_pass_sha1_hash, password, SHA1HASHLENGTHBYTES); }

private:
	enUserStatus _status;
    std::string _login;
    uint* _pass_sha1_hash;
};