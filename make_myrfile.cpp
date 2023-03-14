#include <iostream>
using namespace std;

int main() {
    srand(time(0));
    int max_num = 50000;
    cout << max_num << endl;
    for (int i = 0; i < max_num; i++) {
        cout << (rand() % INT32_MAX) + (rand() % 10 - 5) << endl;
    }
    return 0;
}