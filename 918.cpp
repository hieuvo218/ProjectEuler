#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

long long int solve(long long int s)
{
    long long int a = 1;
    long long int ns = 1;   // value of the last number
    while(s % 2 == 0) {
        s /= 2;
        ns *= 2;    // Update value
    }
    vector <long long int> nums(s+1);
    nums[1] = 1;
    for(long long int i = 2; i < s+1; i++) {
        if (i % 2 == 0) {
            nums[i] = 2*nums[i/2];
        }
        else {
            nums[i] = nums[i/2] - 3*nums[i/2 + 1];
        }
    }
    return 4*a - ns*nums[s];
}

int main()
{
    long long int s = pow(10, 12);
    long long int res = solve(s/2);
    cout << res << endl;
    return 0;
}