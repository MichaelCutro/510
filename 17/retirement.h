#ifndef __RETIREMENT_H__
#define __RETIREMENT_H__

#include <stdlib.h>

// In retirement.h, you should see the
// ret_portfolio_t type, which holds an array of dates, an array of the
// cumulative investments, and the size of these arrays. Your program
// should allocate and populate one of these structs for each investment
// strategy. Look for opportunities for abstraction here! 

struct ror_arr_tag {
  // WRITE ME
  double ror;
  double price;
};
typedef struct ror_arr_tag ror_arr_t;

// struct to hold array of dates and cumulative investments
// (hint: can be used for any investment strategy)
struct ret_portfolio_tag {
  double * dates;
  double * cum_invest;
  size_t n;
};
typedef struct ret_portfolio_tag ret_portfolio_t;

typedef struct
{
  /* data */
  double date;
  double bondPrice;
  double bondRor;
  double sandpIndexLevel;
  double sandpDividend;
  double sandpRor;
  double divRor;
  double portfolioSandP;
  double portfolioBond;
  double contribution;
  double weightEquity;
  double strategy1;
  double strategy2;
  double strategy3;
} data;


#endif