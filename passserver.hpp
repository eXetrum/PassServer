#include "passserver.h"


PassServer::PassServer(size_t size) : accounts(size) {}

PassServer::~PassServer() { accounts.clear(); }

// Adds a new user to the hashtable
bool PassServer::addUser(const pair<string, string> & p)
{
	// Encrypt user password and store pairs to hashtable
	return accounts.insert
	(
		std::pair<string, string>
		(
			p.first, 
			encrypt(p.second)
		) 
	);
}

//Adds a new user to the hashtable
bool PassServer::addUser(pair<string, string> && p)
{
	// Get referebce to current pair
	std::pair<string, string>& pm = p;
	// Change value (second entry is password)
	pm.second = encrypt(pm.second);
	return accounts.insert(pm);
}

//Removes a user from the hashtable
bool PassServer::removeUser(const string & k)
{
	return accounts.remove(k);
}

//Changes an exisiting user's password
bool PassServer::changePassword(const pair<string, string> & p, const
	string& newpasswd)
{
	pair<string, string> acc(p.first, encrypt(p.second));
	pair<string, string> newAcc(p.first, encrypt(newpasswd));
	// Lazy semantic
	// If user, pass pair does not match we will not move to the "remove" and "insert"
	// otherwise if pair match, we remove user record and after that insert new with new password
	return accounts.match(acc) && accounts.remove(p.first) && accounts.insert(newAcc);
}

//Checks if user is in hashtable
bool PassServer::find(const string & user) const { return accounts.contains(user); }

// Loads hashtable entries from a file
bool PassServer::load(const char * filename)
{
	return accounts.load(filename);
}

//Dumps the hashtable entries into a file
bool PassServer::write_to_file(const char * filename) const
{
	return accounts.write_to_file(filename);
}

//Dumps the hashtable structure and entries to screen
void PassServer::dump() const 
{ 
	accounts.dump(); 
}

//Returns the size of the hashtable
size_t PassServer::size() const
{
	return accounts.size();
}

//Encrypts the password
string PassServer::encrypt(const string & str)
{
	char salt[] = "$1$########";
	string password( crypt(str.c_str(), salt) );
	return password.substr(12);
}

