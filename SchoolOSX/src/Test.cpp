#include "string.h"
#include "iostream"
#include "stdio.h"
#include <stdlib.h>
using namespace std;
int main(){
int *Test = (int*) malloc(sizeof(int)*16);
cout << sizeof(int)   << "\n";
cout << sizeof(Test)  << "\n";
cout << sizeof(*Test) << "\n";
return 0;
}
