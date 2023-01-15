#pragma once

template <typename K, typename V>
HashTable<K, V>::HashTable(size_t n)
	: currentSize{ 0 }, bucketVector(prime_below(n)) { }


template <typename K, typename V>
HashTable<K, V>::~HashTable() { makeEmpty(); }


template <typename K, typename V>
bool HashTable<K, V>::match(const std::pair<K, V>& kv) const {
	size_t h = myhash(kv.first);

	std::list<std::pair<K, V> > chain = bucketVector[h];
	typename std::list<std::pair<K, V>>::iterator it = chain.begin();

	for (; it != chain.end(); ++it) if (it->first == kv.first && it->second == kv.second) return true;
	return false;
}

template <typename K, typename V>
bool HashTable<K, V>::contains(const K &x) const {
	size_t h = myhash(x);

	std::list<std::pair<K, V> > chain = bucketVector[h];
	typename std::list<std::pair<K, V>>::iterator it = chain.begin();

	for (; it != chain.end(); ++it) if (it->first == x) return true;
	return false;
}

template <typename K, typename V>
void HashTable<K, V>::makeEmpty() {
	for (size_t i = 0; i < bucketVector.size(); ++i) bucketVector[i].clear();
	currentSize = 0;
}

template <typename K, typename V>
void HashTable<K, V>::clear() {
	makeEmpty();
}

template <typename K, typename V>
bool HashTable<K, V>::insert(const std::pair<K, V> & kv) {
	// Time to rehash ?
	if (currentSize > bucketVector.size()) rehash();
	// Get new hash
	size_t h = myhash(kv.first);

	std::list<std::pair<K, V> >& chain = bucketVector[h];
	typename std::list<std::pair<K, V>>::iterator it = chain.begin();

	for (; it != chain.end(); ++it) {
		if (it->first == kv.first) {
			if (it->second == kv.second) return false; // Key and value equals
			std::pair<K, V>& p(*it);
			p = kv; // Update
			return true;
		}
	}
	// Still here -> add new item
	bucketVector[h].push_back(kv);
	currentSize++;
	return true;
}

template <typename K, typename V>
bool HashTable<K, V>::insert(std::pair<K, V> && kv) {
	// Time to rehash ?
	if (currentSize > bucketVector.size()) rehash();
	// Get new hash
	size_t h = myhash(kv.first);

	std::list<std::pair<K, V> >& chain = bucketVector[h];
	typename std::list<std::pair<K, V>>::iterator it = chain.begin();

	for (; it != chain.end(); ++it) {
		if (it->first == kv.first) {
			if (it->second == kv.second) return false; // Key and value equals
			std::pair<K, V>& p(*it);
			std::swap(p, kv); // Update
			return true;
		}
	}
	// Still here -> add new item
	
	std::pair<K, V> p;
	std::swap(p.first, kv.first);
	std::swap(p.second, kv.second);
	bucketVector[h].push_back(p);

	currentSize++;
	return true;
}

template <typename K, typename V>
bool HashTable<K, V>::remove(const K &x) {
	size_t h = myhash(x);


	std::list<std::pair<K, V> >& chain = bucketVector[h];
	typename std::list<std::pair<K, V>>::iterator it = chain.begin();


	for (; it != chain.end(); ++it) {
		if (it->first == x) {
			chain.erase(it);
			--currentSize;
			return true;
		}
	}
	
	return false;
}

template <typename K, typename V>
bool HashTable<K, V>::load(const char *filename) {
	std::ifstream ifs(filename);
	if (!ifs.is_open()) return false;

	clear();
	std::string line;
	while (getline(ifs, line)) {
		K key;
		V value;
		std::istringstream iss(line);
		if (iss >> key >> value) {
			insert(std::pair<K, V>(key, value));
		}
	}

	ifs.close();
	return true;
}

template <typename K, typename V>
bool HashTable<K, V>::write_to_file(const char *filename) const {
	std::ofstream ofs(filename);
	if (!ofs.is_open()) return false;

	for (size_t i = 0; i < bucketVector.size(); ++i) {
		std::list<std::pair<K, V> > chain = bucketVector[i];
		typename std::list<std::pair<K, V>>::iterator it = chain.begin();
		for (; it != chain.end(); ++it)
			ofs << it->first << " " << it->second << std::endl;
	}
	return true;
}

template <typename K, typename V>
void HashTable<K, V>::dump() const {
	for (size_t i = 0; i < bucketVector.size(); ++i) {
		std::list<std::pair<K, V> > chain = bucketVector[i];
		typename std::list<std::pair<K, V>>::iterator it = chain.begin();
		std::cout << "v[" << i << "]: ";
		size_t p = 0;
		for (; it != chain.end(); ++it, ++p) {
			std::cout << it->first << " " << it->second;
			if(p + 1< chain.size()) std::cout << ":";
		}
		std::cout << std::endl;
	}
}

template <typename K, typename V>
size_t  HashTable<K, V>::size() const { return currentSize; }

template <typename K, typename V>
void HashTable<K, V>::rehash() {
	size_t newPrimeSize = bucketVector.size();
	do {
		newPrimeSize *= 2;
		newPrimeSize = prime_below(newPrimeSize);
		
		if (newPrimeSize == 0) return;
	} while (newPrimeSize < bucketVector.size());

	// Create copy of current bucket
	std::vector<std::list<std::pair<K, V>>> temp(bucketVector); 
	// Resize bucket
	bucketVector.clear();
	bucketVector.resize(newPrimeSize);
	// Perform rehash for old items
	for (size_t i = 0; i < temp.size(); ++i) {
		std::list<std::pair<K, V>>& chain = temp[i];
		typename std::list<std::pair<K, V>>::iterator it = chain.begin();
		for (; it != chain.end(); ++it) {
			size_t h = myhash(it->first);
			bucketVector[h].push_front(*it);
		}		
	}
}

template <typename K, typename V>
size_t HashTable<K, V>::myhash(const K &k) const {
	return std::hash<K>{}(k) % bucketVector.size();
}

// returns largest prime number <= n or zero if input is too large
// This is likely to be more efficient than prime_above(), because
// it only needs a vector of size n
template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below(unsigned long n)
{
	if (n > max_prime)
	{
		std::cerr << "** input too large for prime_below()\n";
		return 0;
	}
	if (n == max_prime)
	{
		return max_prime;
	}
	if (n <= 1)
	{
		std::cerr << "** input too small \n";
		return 0;
	}

	// now: 2 <= n < max_prime
	std::vector <unsigned long> v(n + 1);
	setPrimes(v);
	while (n > 2)
	{
		if (v[n] == 1) {
                        std::cout << n << std::endl;
			return n;
                }
		--n;
	}

	return 2;
}

//Sets all prime number indexes to 1. Called by method prime_below(n) 
template <typename K, typename V>
void HashTable<K, V>::setPrimes(std::vector<unsigned long>& vprimes)
{
	int i = 0;
	int j = 0;

	vprimes[0] = 0;
	vprimes[1] = 0;
	int n = vprimes.capacity();

	for (i = 2; i < n; ++i)
		vprimes[i] = 1;

	for (i = 2; i*i < n; ++i)
	{
		if (vprimes[i] == 1)
			for (j = i + i; j < n; j += i)
				vprimes[j] = 0;
	}
}

