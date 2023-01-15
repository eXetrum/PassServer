#ifndef __PASSSERVER_H__
#define __PASSSERVER_H__

#include "hashtable.h"
#include <string>
#include <crypt.h>
using namespace std;
using namespace cop4530;

class PassServer
{
private:
	HashTable<string, string> accounts;
	string encrypt(const string& str);
public:
	PassServer(size_t size = 101);
	virtual ~PassServer();
	bool load(const char* filename);
	bool addUser(const std::pair<string, string>& kv);
	bool addUser(std::pair<string, string>&& kv);
	bool removeUser(const string& k);
	bool changePassword(const std::pair<string, string>& p, const string& newpassword);
	bool find(const string& user) const;
	void dump() const;
	size_t size() const;
	bool write_to_file(const char* filename) const;
};
#include "passserver.hpp"
#endif
