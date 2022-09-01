#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "retirement.h"

/*
written by @MichaelCutro
*/

char const *ARGV0 = "retirement";

int main(int argc, char *argv[])
{
  // pointer variable *ARGV0 = "retirement"; this is the name of the program
  ARGV0 = argv[0];

  // Expecting 5 command line argumetns; ./retirement sp_data.csv bond_data.csv 1971.12 2017.12
  if (argc != 5)
  {
    fprintf(stderr, "%s: incorrect command line arguments\n", ARGV0);
    return EXIT_FAILURE;
  }
  // Read in the data from the S and P file first ===========================================================

  // pointer to the sandp file
  char const *sandpDataPath = argv[1];

  // open the sandp file
  FILE *sandpFile = fopen(sandpDataPath, "r");

  // check if the file is null
  if (sandpFile == NULL)
  {
    // echo's the file name we are trying to open
    fprintf(stderr, "%s: unable to open bond file, %s\n", ARGV0, sandpDataPath);
    // more context coming from OS
    perror(ARGV0);
    return EXIT_FAILURE;
  }

  // create array of data structs - can store up to 600 records or lines into the array
  data sandp_data[600];

  // sandpCount is used to store the number of values CSV that were read in successfully from each line of the file
  int sandpCount = 0;

  // sandpRecordCount will keep track of the number of records read in from the file
  int sandpRecordCount = 0;

  // skip the first line of the file
  sandpRecordCount = fscanf(sandpFile, "%*[^\n]\n");

  // read each line of the sandp file and store the data into the array of structs
  do
  {
    sandpCount = fscanf(sandpFile, "%lf,%lf,%lf", &sandp_data[sandpRecordCount].date, &sandp_data[sandpRecordCount].sandpIndexLevel, &sandp_data[sandpRecordCount].sandpDividend);
    if (sandpCount == 3)
    {
      sandpRecordCount++;
    }
    if (sandpCount != 3 && !feof(sandpFile))
    {
      fprintf(stderr, "%s: format error reading sandp file, %s\n", ARGV0, sandpDataPath);
      return EXIT_FAILURE;
    }
    if (ferror(sandpFile))
    {
      fprintf(stderr, "%s: error reading sandp file, %s\n", ARGV0, sandpDataPath);
      return EXIT_FAILURE;
    }
  } while (!feof(sandpFile));

  fclose(sandpFile);

  // Read in the data from the bond file second ===========================================================

  // pointer to the bond file
  char const *bondDataPath = argv[2];

  // open the file
  FILE *bondFile = fopen(bondDataPath, "r");

  // check if the file is null
  if (bondFile == NULL)
  {
    // echo's the file name we are trying to open
    fprintf(stderr, "%s: unable to open bond file, %s\n", ARGV0, bondDataPath);
    // more context coming from OS
    perror(ARGV0);
    return EXIT_FAILURE;
  }
  // create array of data structs - can store up to 600 records or lines into the array
  data bond_data[600];

  // bondCount is used to store the number of values CSV that were read in successfully from each line of the file
  int bondCount = 0;

  // bondRecordCount will keep track of the number of records read in from the file
  int bondRecordCount = 0;

  // skip the first line of the file
  bondRecordCount = fscanf(bondFile, "%*[^\n]\n");

  // read each line of the bond file and store the data into the array of structs
  do
  {
    bondCount = fscanf(bondFile, "%lf,%lf", &bond_data[bondRecordCount].date, &bond_data[bondRecordCount].bondPrice);
    if (bondCount == 2)
    {
      bondRecordCount++;
    }
    if (bondCount != 2 && !feof(bondFile))
    {
      fprintf(stderr, "%s: format error reading bond file, %s\n", ARGV0, bondDataPath);
      return EXIT_FAILURE;
    }
    if (ferror(bondFile))
    {
      fprintf(stderr, "%s: error reading bond file, %s\n", ARGV0, bondDataPath);
      return EXIT_FAILURE;
    }
  } while (!feof(bondFile));

  fclose(bondFile);

  // portfolio start date - argv[3] ===========================================================
  double startDate;
  if (sscanf(argv[3], "%lf", &startDate) != 1)
  {
    fprintf(stderr, "%s: incorrect start date\n", ARGV0);
    return EXIT_FAILURE;
  }
  char* periodPointer = strchr(argv[3], '.');
  int periodIndex = periodPointer-argv[3];
  int len = strlen(argv[3]);
  if (len-periodIndex != 3) 
  {
    fprintf(stderr, "%s: incorrect start date format no ints\n", ARGV0);
    return EXIT_FAILURE;
  }
  // portfolio end date  - argv[4]
  double endDate;
  if (sscanf(argv[4], "%lf", &endDate) != 1)
  {
    fprintf(stderr, "%s: incorrect end date\n", ARGV0);
    return EXIT_FAILURE;
  }
  periodPointer = strchr(argv[4], '.');
  periodIndex = periodPointer-argv[4];
  len = strlen(argv[4]);
  if (len-periodIndex != 3) 
  {
    fprintf(stderr, "%s: incorrect start date format no ints\n", ARGV0);
    return EXIT_FAILURE;
  }
  // error check start date 1-12 month and year rang
  double integral = 0.00;
  float fractional = 0.00f;
  fractional = modf(startDate, &integral);

  if (fractional > 0.12f || fractional < 0.01f)
  {
    fprintf(stderr, "%s: incorrect month range\n", ARGV0);
    return EXIT_FAILURE;
  }

  if (integral < 1970 || integral > 2019)
  {
    fprintf(stderr, "%s: incorrect year range\n", ARGV0);
    return EXIT_FAILURE;
  }

  // error check end date format 1-12 month range
  double integral_1 = 0.00;
  float fractional_1 = 0.00f;
  fractional_1 = modf(endDate, &integral_1);

  if (fractional_1 > 0.12f || fractional_1 < 0.01f)
  {
    fprintf(stderr, "%s: incorrect month range\n", ARGV0);
    return EXIT_FAILURE;
  }

  if (integral_1 < 1970 || integral_1 > 2019)
  {
    fprintf(stderr, "%s: incorrect year range\n", ARGV0);
    return EXIT_FAILURE;
  }



  // loop through the array of structs and calulate the ROR for each bond bondPrice and store it in the bondRor field of the struct
  for (int i = 0; i < bondRecordCount; i++)
  {
    bond_data[i].bondRor = ((bond_data[i].bondPrice / 100) / 12);
  }
  // calculate the ROR for S&P index
  for (int i = 0; i < sandpRecordCount; i++)
  {
    sandp_data[i].sandpRor = ((sandp_data[i].sandpIndexLevel / sandp_data[i - 1].sandpIndexLevel) - 1);
  }

  for (int i = 0; i < sandpRecordCount; i++)
  {
    sandp_data[i].divRor = ((sandp_data[i].sandpDividend / 12) / sandp_data[i].sandpIndexLevel);
  }
  // calculate the portfolio s&p and the portfolio Bond
  for (int i = 0; i < sandpRecordCount; i++)
  {
    sandp_data[i].portfolioSandP = (sandp_data[i].sandpRor + sandp_data[i].divRor);
  }
  for (int i = 0; i < bondRecordCount; i++)
  {
    bond_data[i].portfolioBond = (((bond_data[i].bondPrice / 100) / 12));
  }
  // create array that adds 100 to the contribution and adds 1 to the weighted equity
  for (int i = 0; i < sandpRecordCount; i++)
  {
    sandp_data[i].contribution = 100;
    sandp_data[i].weightEquity = 1.00;
  }

  // write to a file named 'portfolio.csv'
  FILE *portfolio = fopen("portfolio.csv", "w");
  if (!portfolio)
  {
    fprintf(stderr, "Unable to open portfolio.csv.");
    perror(argv[0]);
    exit(1);
  }

  sandp_data[0].strategy1 = 100.00;
  sandp_data[0].strategy2 = 100.00;
  sandp_data[0].strategy3 = 100.00;

  for (int i = 1; i < sandpRecordCount; i++)
  {
    if (sandp_data[i].date < startDate || sandp_data[i].date > endDate)
    {
      continue;
    }
    double full_date = sandp_data[i].date;
    double integral_part = 0.00;
    float fractional_part = 0.00f;
    fractional_part = modf(full_date, &integral_part);
    if (fractional_part == 0.01f)
    {
      for (int j = 0; j < sandpRecordCount; j++)
      {
        sandp_data[j].weightEquity = sandp_data[j].weightEquity - 0.02;
        sandp_data[j].contribution = sandp_data[j].contribution * 1.025;
      }
    }
    if (sandp_data[i].date == startDate) 
    {
      sandp_data[i].strategy1 = 100;
      sandp_data[i].strategy2 = 100;
      sandp_data[i].strategy3 = 100;
    } 
    else 
    {
    // printf("%lf %.30lf\n", sandp_data[i].date, sandp_data[i].weightEquity);
    sandp_data[i].strategy1 = sandp_data[i - 1].strategy1 * (sandp_data[i].portfolioSandP + 1) + (sandp_data[i - 1].strategy1 * (0) * (bond_data[i].portfolioBond + 1) + sandp_data[i].contribution);
    sandp_data[i].strategy2 = sandp_data[i - 1].strategy2 * (0) * (sandp_data[i].portfolioSandP + 1) + (sandp_data[i - 1].strategy2 * (1 - 0) * (bond_data[i].portfolioBond + 1)) + sandp_data[i].contribution;
    // Fix // =M2*(F3)*(1+I3)+(M2*(1-F3)*(1+J3))+E3 (strat 3)
    sandp_data[i].strategy3 = sandp_data[i - 1].strategy3 * (sandp_data[i].weightEquity) * (sandp_data[i].portfolioSandP + 1) + (sandp_data[i - 1].strategy3 * (1 - sandp_data[i].weightEquity) * (bond_data[i].portfolioBond + 1)) + sandp_data[i].contribution;
    }
    fprintf(portfolio, "%.2lf,%.2lf, %.2lf,%.2lf,%.2lf,%.2lf\n", sandp_data[i].date, sandp_data[i].contribution, sandp_data[i].weightEquity, sandp_data[i].strategy1, sandp_data[i].strategy2, sandp_data[i].strategy3);
    
  }

  fclose(portfolio);

  return EXIT_SUCCESS;
}

