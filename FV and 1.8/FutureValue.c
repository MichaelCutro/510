#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
@dev Future Value of a single cashflow
*/

double futureValue (double percent, double presentValue, double time) {
  double ans = 0.00;
  double x = 1+percent*.01;
  double powerOfN = pow(x, time);
  ans = presentValue*powerOfN;
  printf( "If you invested $%.2f at %.2f percent for %.2f years, " , presentValue, percent, time );
  printf( "you would have $%.2f !\n", ans);
  return 0;
}

int main (void) {
  futureValue(5.00, 20000.00, 18.00);
}
