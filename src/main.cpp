#include <iostream>
#include <bitset>

#include "rtfs.h"

using namespace std;

int main() {

    Key key(0xf0f0f0ff00f0f0f0);

    cout << bitset<64>(key.getHash()) << endl;
    cout << bitset<64>(key.getParenthash()) << endl;
    cout << bitset<64>(key.getFilehash()) << endl;

    return 0;
}