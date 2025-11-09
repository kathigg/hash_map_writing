//
// Created by Deb on 4/12/2025.
//

#include "HNode.h"
#include <iostream>
#include <time.h>
using namespace std;

hNode::hNode(string s){
    key = s;
    size = 450;        
    valueArr = new string[size];
    valuesCt = 0;
    // this constructor sets the key in the node to be s, sets the size of the node's
    // value array to be 450 (yes, this isn't a terribly space-efficient way to store the
    // values.  You're learning hashmaps in this lab.  Stay focused people.)
    // you should then create a new valueArr of size 450 on the heap.
    // since there are no values in the value array yet, it's valuesCt should be set to 0
}
hNode::hNode(){
    key = "";
    valuesCt = 0;
    size = 0;
    valueArr = nullptr;
    // this constructor is for an empty node.
    // The key is set to an empty string.
    // the valueArr is set to NULL
    // the valuesCt and the size are set to 0
    // You need this to make an array of type hNode (aka the map in hashMap)
}
hNode::hNode(string s, string v){    
    key = s;
    size = 450;
    valueArr = new string[size];
    valuesCt = 0;
    valueArr[valuesCt++] = v;
    // this constructor sets the key in the node to be s,
    // sets the size of the node's value array to be 450 (yes, this isn't a terribly space-efficient way to store the
    // values.  You're learning hashmaps in this lab.  Stay focused people.)
    // you should then create a new valueArr of size 450 on the heap.
    // Then set the first value in the valueArr to be v
    // and the valuesCt to be 1 (since you just added a value)

}
void hNode::addValue(string v) {
    if (valuesCt >= size) {
        cerr << "Warning: value array full for key '" << key << "'\n";
        return;
    }
    valueArr[valuesCt++] = v;
    // this method is short - you just add v to the end of the values in the valueArr and
    // then increase the valuesCt.
    // I put in a warning for if you have more values in the valueArr than the size of the
    // array.

}

string hNode::getRandValue() {
    if (valuesCt == 0 || valueArr == nullptr)
        return "";
    int idx = rand() % valuesCt;
    return valueArr[idx];
    // this is another short method - just generates a random number less than the
    // count of the values in teh valueArr, and then returns the word at that index
    // in the valueArr.  If the valueArr is empty, it returns an empty string.

}
hNode::~hNode() {
    delete[] valueArr;
    cout << "Deleting " << key << endl;
    // destructor - make sure you delete the valueArr since it's on hte heap
}