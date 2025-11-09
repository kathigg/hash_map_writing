//
// Created by Deb on 4/12/2025.
//

#include "HashMap.h"
#include "HNode.h"
#include <iostream>
#include <math.h>
#include <string>
using namespace std;

hashMap::hashMap(int hfn, int cfn) {
	//the constructor for teh hashMap.
	// whichHashFn should be set to hfn, and
	// whichCollFn should be set to cfn
	// the first is just the very first string used for output.  In the voice constructor
	//     I set first to "I" but you can choose any word in the input file.
	// Set teh mapSize to 57 to start with if you want to get the same ouput I got for the
	// testing data.
	// keysCt starts at 0 because there are no keys in the map to start with.
	// here it gets a bit interesting:  you have to make a new map (of type hNode *) on
	// the heap.  This is an array on the heap of pointers to nodes.  Why?  Because when
	// you rehash, you don't need to delete old nodes and make new nodes.  You just need to
	// copy the address of the node from the old map to the new map in reHash.
	// Make sure you loop through and set all your initial addresses in the map array to
	// be Null or nothing will work.
	// then set your hashCollisionsCt  and your collisionsCt to 0.  These are going to
	// keep track of how many collisions your hash function causes.  And, once there's a
	// collision, your collision function may also result in collisions.  We want to keep
	// track of that in the collisionsCt. THat way we can measure the efficiency of our
	// hashing and collision functions.
	whichHashFn = hfn;
	whichCollisionFn = cfn; 
	first = "I";
	mapSize = 57; 
	keysCt = 0;
	map = new hNode *[mapSize];
	for (int i = 0; i < mapSize; i++) {
		map[i] = nullptr;
	}
	hashCollisionsCt = 0;
	collisionsCt = 0;
}

int hashMap::getClosestPrime(int start) {
    int n = std::max(2, start);
    while (true) {
        bool isPrime = true;
        for (int d = 2; d <= static_cast<int>(sqrt(n)); ++d) {
            if (n % d == 0) { isPrime = false; break; }
        }
        if (isPrime) return n;
        ++n;
    }
}

void hashMap::addKeyandValue(string k, string v) {
	// this method finds if and how we need to incorporate the key and/or its accompanying
	//value into the hash map.
	//
	// It first calls getIndex to get the index of the key using one of the hashing functions
	// (getIndex is described below).  There are then 3 possible next steps:
	//
	// 1) If the index returned is the index of an empty location in the map, this method calls
	//    insertNewKeyandValue (described below) to insert the key and its accompanying value
	//    into the map.
	//
	// 2) If the index returned is the location in the map where the key already in the method,
	//    the node's addValue method is called so the value is added to that key's value array.
	//
	// 3) if the index returned is a non-empty location in the map, but that location is not
	//    where the key is located (this is a hashCollision - so increase the hashCollisionCt),
	//    it now calls the dealWithCollisions method (described below)
	//
	int result = getIndex(k);
	if (map[result] == NULL) {
		insertNewKeyandValue(k, v, result);
	} 
	else if (map[result]->key == k) {
		map[result]->addValue(v);
	}	
	else {
		hashCollisionsCt++;
		int newIndex = dealWithCollisions(k, result);
		if (map[newIndex] == NULL) {
			insertNewKeyandValue(k, v, newIndex);
		}
		else if (map[newIndex]->key == k){
			map[newIndex]->addValue(v);
		}
	}

}

int hashMap::getIndex(string k) {
    if (whichHashFn == 1) return hashFn1(k);
    else if (whichHashFn == 2) return hashFn2(k);   // Only if hashFn2 is a true primary hash.
    else return hashFn3(k);
}

int hashMap::dealWithCollisions(string k, int i) {
	/* this method is really short and straightforward.  It determines which collision function to
	 * call based on the whichCOllisionFn field in hashMap, then calls that collision function, and
	 * returns the index that collision function returned.  That's it.
	 * for test 1 it will call CollFn1 (the one I gave you)
	 */
	if (whichCollisionFn == 1) {
		return collFn1(k, i);
	}
	else if (whichCollisionFn == 2) {
		return collFn2(k, i);
	}
	else {
		return collFn3(k, i); 
	}
}

int hashMap::collFn1(string k, int i) {
	// My ridiculously simple collision function that uses linear probing.
	// PLEASE write something better than this for collFn2 and collFn3...

	int ct = 0;
	while (ct < mapSize) {
		i = (i + 1)%mapSize;
		if (map[i] == NULL || map[i]->key == k) {
			collisionsCt += ct;
			return i;
		}
		ct ++;
	}
	if (ct == mapSize) {cout <<"ERROR" << endl; return -1;}
	return i;
}

int hashMap::collFn2(string k,  int i) {
	// you gotta write to compare with collFn3 to see which collision function works best with the
	// data we're using.
	// using double hashing for collision resolution
	int hash1 = i; 
	int hash2 = hashFn2(k);

	if (hash2 == 0) hash2 = 1;
	for (int j = 1; j < mapSize; j++) {
		int newIndex = (hash1 + j * hash2) % mapSize;

		if (map[newIndex] == nullptr || map[newIndex]->key == k) {
			collisionsCt += (j - 1);
			return newIndex;
		}
	}

	cout << "ERROR" << endl; 
	return -1;
}
int hashMap::collFn3(string k, int i) {
	// you gotta write to see which collision function works best
	// using quadratic probing for collision resolution
	for (int j = 1; j <= mapSize; j++){
		// compute new index using quadratic probing 
		int newIndex = (i + j * j) % mapSize;
		if (map[newIndex] == nullptr) {
			collisionsCt += (j - 1);
			return newIndex;
		}
		if (map[newIndex]->key == k){
			return newIndex;
		}
	}

	return i; 
}

void hashMap::insertNewKeyandValue(string k, string v, int ind) {
	//This is a short method that adds the new key and value to the map by creating a new Node,
	// then inserting at the index ind sent in as a parameter
	//
	// It increases the number of keys in the map (aka keysCt)
	//
	// and then it calls ckIfNeedToRehash()
	hNode* newNode = new hNode(k, v);
	map[ind] = newNode;
	keysCt++;
	ckIfNeedToRehash();
}

int hashMap::hashFn1(string k) {
	// ridiculously simple hash fn that just converts each char to an int, adds the char's ints,
	// mods by the map size, and returns the index found.
	// Again, when you write yours, please do better!!!
	int h_index = 0;
	int len = k.length();
	for (int i = 0; i < len; i++) {
		h_index = h_index + (int)k[i];
	}
	//cout << "hash index " << (h_index%mapSize) << endl;
	return h_index%mapSize;
}
int hashMap::hashFn2(string k) {
	// One of two hashing functions you'll be writing to try to see which function works most
	// efficiently with the data.  

	// essentially building hash2 = R - (sumOfChars % R)
	int sum = 0; 
	for (char c : k) {
		sum += (int)c;
	}
	int R = 53; // this is a prime number smaller than the mapSize (57)
	int step = R - (sum % R);
	if (step == 0) step = 1; // avoid 0 step size 
	return step; 
}

int hashMap::hashFn3(string k) {
    int sum = 0;
    for (char c : k) sum += static_cast<int>(c);
    int primeBelow = getClosestPrime(mapSize - 1);
    return sum % primeBelow;
}

void hashMap::ckIfNeedToRehash() {
	// this is the function that checks to see if the map is 70% full or more.  If it's over 70%
	// full, it should call the rehash function.
	// BIG Mistake I see a lot: dividing an int by an int to see if it's .7 or greater.
	// int/int results in a floored int.  so 7/10 will be 0, not .7
	float loadFactor = (float)keysCt / (float)mapSize;
	if (loadFactor >= 0.7) {
		reHash();
	}
}

int hashMap::findKeyIndex(string k) {
	// this method is used by the writeFile method.  It takes as input a word (the key)
	// and, using the appropriate hashing function (and, if necessary, the appropriate
	// collision function) returns the index of where the key is located in the
	// map.
	// PLEASE NOTE:  the entire point of hashmaps is to be able to FIND quickly!!!
	// IF you start at index 0 of the map and loop through every value looking for k,
	// you will lose 50% of your grade on this project because that is the exact opposite
	// of the point of a hashmap.
	int index = getIndex(k); 
	if (map[index] != nullptr && map[index]->key == k){
		return index;
	}
	else {
		return dealWithCollisions(k, index);
	}
}
void hashMap::reHash() {
	// This is a challenging method.
	// you're doubling the hashmap size and then taking the size up to the nearest prime.
	// to do that, you need to create a new map on the heap, and initialize all the nodes
	// in the map to NULL (make sure you don't lose track of the old map!)
	// Once done, you'll need to find where to insert each of the nodes from the old map
	// into your newly created map.  You can use the function(s) you've already written
	// for this.
	int oldMapSize = mapSize;
    int newMapSize = getClosestPrime(mapSize * 2);
    hNode** oldMap = map;

    mapSize = newMapSize;
    map = new hNode*[mapSize];
    for (int i = 0; i < mapSize; ++i) map[i] = nullptr;

    // Optional: reset stats
    // hashCollisionsCt = 0;
    // collisionsCt = 0;

    keysCt = 0;
    for (int i = 0; i < oldMapSize; ++i) {
        if (!oldMap[i]) continue;
        string key = oldMap[i]->key;
        for (int j = 0; j < oldMap[i]->valuesCt; ++j) {
            addKeyandValue(key, oldMap[i]->valueArr[j]);
        }
        delete oldMap[i];
    }
    delete[] oldMap;
}

hashMap::~hashMap() {
	// Destructor.  deletes every node in the map, and then deletes the map
	for (int i = 0; i < mapSize; i++) {
		if (map[i] == nullptr) {
			continue;
		}
		else {
			delete map[i];
		}
	}
	delete[] map;
}

void hashMap::printMap() {
	cout << "In printMap(): Included for testing purposes!!! MapSize: " << mapSize << ", KeysCt: " <<keysCt <<endl;
	for (int i = 0; i < mapSize; i++) {
		//cout << "In loop" << endl;
		if (map[i] != NULL) {
			cout << map[i]->key << " ("<< i << "): ";
			for (int j = 0; j < map[i]->valuesCt;j++) {
				cout << map[i]->valueArr[j] << ", ";
			}
			cout << endl;
		}
	}
}
