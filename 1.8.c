// C program to illustrate
// Chapter 1 - Question 1.8
#include <math.h>
#include <stdio.h>

int main()
{
    int n = 0;
    for (int i=0; i<n*3; i++) {
      int powerOfN = pow(n, 2);
      int negPowerOfN = powerOfN * -1;
      int powerOfI = pow(i, 2);
      int ans = 0;
      ans = negPowerOfN+powerOfI;
      printf("%d\n", ans);
    }
}
