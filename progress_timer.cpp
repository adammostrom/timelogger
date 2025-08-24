#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

int main() {
    for (int i = 0; i <= 100; i++) {
        cout << "\rProgress: " << i << "%" << flush; // overwrite same line
        this_thread::sleep_for(chrono::milliseconds(50));
    }
    cout << endl; // move to next line after done
}