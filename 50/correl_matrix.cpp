#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <math.h>
#include <iomanip>
using namespace std;

map<string, vector<double> > input;
map<string, vector<double> > rors;

// argc is int and stores number of command line arguments passed by the user
// argv holds the name of the program
// argv[0] = ./a.out
// argv[1] = points to first command line argument
void printDoubles(map<string, double> &numbers)
{
  for (map<string, double>::iterator it = numbers.begin(); it != numbers.end(); it++)
  {
    cout << it->first<< " "<< it->second << endl;
  }
}

void printInput()
{
  for (map<string, vector<double> >::iterator it = input.begin(); it != input.end(); it++)
  {
    cout << it->first;
    for (unsigned int i = 0; i < it->second.size(); i++)
    {
      cout << " " << it->second[i];
    }
    cout << endl;
    cout << endl;
  }
}

map<string, double> average(map<string, vector<double> > &numbers)
{
  map<string, double> averages;
  for (map<string, vector<double> >::iterator it = numbers.begin(); it != numbers.end(); it++)
  {
    double sum = 0;
    for (unsigned int i = 0; i < it->second.size(); i++)
    {
      sum += it->second[i];
    }
    sum /= it->second.size();
    // sum = sum / it->second.size(); (second.size is the number of els)
    averages[it->first] = sum;
  }
  return averages;
}

map <string, double > sdev (map<string, vector<double> > &numbers) 
{
  map<string, double> sdevs;
  map<string, double> averages = average(numbers);
  for (map<string, vector<double> >::iterator it = numbers.begin(); it != numbers.end(); it++)
  {
    double sum = 0;
    for (unsigned int i = 0; i < it->second.size(); i++)
    {
      double diff = it->second[i] - averages[it->first];
      sum+=(diff*diff);
    }
    sum /= (it->second.size()- 1);
    // sum = sum / it->second.size(); (second.size is the number of els)
    sdevs[it->first] = sqrt(sum);
  }

return sdevs;

}

void freeMatrix (double ** matrix, int size) 
{
  for (int i = 0; i < size; i++) 
  {
    delete[](matrix[i]);
  }
  operator delete[](matrix);
}


double ** covariance (map < string, vector<double> > &ror, vector < string > &names) 
{
  map<string, double> averages = average(ror);
  map<string, double> sdevs = sdev(ror);
  double ** covariance = new double * [ror.size()];
  for (unsigned int i =0; i < ror.size(); i++) 
  {
    //set each pointer to an arr of doubles 
    covariance[i] = new double [ror.size()];
  }
  for (unsigned int r = 0; r < names.size(); r++) //iterating over types eg 10yr Bond
  {
    for (unsigned int c = 0; c < names.size(); c++) //iterating over types 
    {
      covariance[r][c] = 0;
      for (unsigned int t = 0; t < ror[names[r]].size(); t++) 
      {
        double deltaA = ror[names[r]][t]-averages[names[r]];
        double deltaB = ror[names[c]][t]-averages[names[c]];
        covariance[r][c] += deltaA*deltaB;
      }
      //covariance[r][c] /= ror.size();
      covariance[r][c] /= ror[names[r]].size(); // correct 
      covariance[r][c] /= sdevs[names[r]]*sdevs[names[c]];
      if(r==c)
      {
        covariance[r][c] = 1;
      }
    }
  }
  return covariance;
}

void printMatrix (double ** matrix, int size) 
{
  string firstChar = "[";
  for (int r = 0; r < size; r++)
  {
    cout << firstChar;
    for (int c = 0; c < size; c++)
    {
      if (c != 0)
      {
        cout << ",";
      }
      cout << right << setw(7) << setprecision(4) << fixed << matrix[r][c]; 
    }
    firstChar = " ";
    if (r != size-1) 
    {
      cout << endl;
    }
  }
  cout << "]";
}


int main(int argc, char *argv[])
{
  if (argc != 2)
  {
      fprintf(stderr, "Invalid command line arguments\n");
      exit(EXIT_FAILURE);
  }

  // read historical data from 'year.csv'

  string line;

  ifstream myFile(argv[1]);

  // if the file is open
  if (myFile.is_open())
  {
    // first line is the header
    // line of keys for the vector to access columns
    getline(myFile, line);
    // store single column name
    char name[32];
    int n;
    // store all of the names
    vector<string> names;
    int column = 0;

    for (unsigned int i = 0; i < line.size(); i += n, column++)
    {
      // pick up all chars until a comma, then write into name var, skip over comma, % says how man chars you skipped over and store in n
      //  if == 1 we are expecting
      if (sscanf(line.c_str() + i, " %[^,],%n", name, &n) == 1)
      {
        if (column == 0)
        {
          continue;
        }
        names.push_back(name);
        // add new entry in input map, gives it the name 'name'
        input[name].clear();
      }
      else
      {
        cerr << "unrecognized name" << endl;
      }
    }

    // read the file line by line
    while (getline(myFile, line))
    {

      double price;
      // skip over first column (date)
      int dateComma = line.find(",");
      line = line.substr(dateComma + 1);
      // //if (sscanf(line.c_str(), " %*d-%*d-%*d", &n)!=3) {
      //     cerr << "unable to find date" << line.c_str() << endl;
      // }
      unsigned int column = 0;
      for (int i = 0; i < line.size() && column < names.size(); i += n, column++)
      {
        // look for double - if yes push price back
        if (sscanf(line.c_str() + i, " %lf,%n", &price, &n) == 1)
        {
          // if we found a price we append it on the columns vector
          input[names[column]].push_back(price);
          // found an empty string and a comma then we assume we got garbage and attempt to copy the last price
        }
        else if (sscanf(line.c_str() + i, " %[^,],%n", name, &n) == 1)
        {
          if (!input[names[column]].empty())
          {
            input[names[column]].push_back(input[names[column]].back());
          }
          else
          {
            cerr << "missing first price" << endl;
            input[names[column]].push_back(0.0);
          }
        }
        else
        {
          fprintf(stderr, "Unrecognized price\n");
          exit(EXIT_FAILURE);
        }
      }
    }
    // close the file
    myFile.close();

    // printInput();
    

    for (size_t n = 0; n < names.size(); ++n)
    {
      string const &name = names[n];
      cout << name << endl;
      // create an entry in rors for name mapped to a vector
      rors[name] = vector<double>();
      // add the first rate of return entry to the map
      //rors[name].push_back(input[name][0]);
      //cout << "0"
          //<< ": " << input[name][0] << endl;
      for (size_t r = 1; r < input[name].size(); ++r)
      {
        double ror = ((input[name][r] - input[name][r - 1]) / input[name][r - 1]) * 100;
        //double sum = sum+=ror;
        //cout << r << ": " << input[name][r] << ", ror = " << ror << "%" << endl;
        rors[name].push_back(ror);
      }

    }
    //printDoubles(rorAverages);
    //printDoubles(sdevs);
    double ** covar = covariance (rors, names);
    printMatrix(covar, rors.size());
    freeMatrix(covar, names.size());
  }




  // otherwise print error
  else
  {
      fprintf(stderr, "Unable to open file\n");
      exit(EXIT_FAILURE);
  }


}
