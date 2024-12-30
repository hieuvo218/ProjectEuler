#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

// long long int solve(long long int s)
// {
//     long long int a = 1, sum = 1;
//     vector <int> nums(s+1);
//     for(long long int i = 2; i < s+1; i++) {
//         if (i % 2 == 0) {
//             nums[i] = 2*nums[i/2];
//         }
//         else {
//             nums[i] = nums[i/2] - 3*nums[i/2 + 1];
//         }
//         sum += nums[i];
//     }
//     return sum;
// }

int main()
{
    long long int s = pow(10, 12);
    int *nums = new int[1000000000000];
    delete [] nums;
    nums = nullptr;
    // long long int res = solve(s);
    // cout << res << endl;
    return 0;
}