For this assignment, you will write the first step toward your final
evaluative assignment. Ultimately, you will write a program that,
given a universe of assets, creates the optimal portfolios by
calculating the efficient frontier, as described in your portfolio
theory module. 

For this first part, you will calculate the correlation matrix for a
universe of assets, given historical price data. At a high level, your
program should:

(1) Read historical price data from a file. 

(2) For each asset, calculate the rate of return for each time step.

(3) For each asset, calculate the average return and standard
    deviation.

(4) Calculate the covariance matrix for all the assets.

(5) Calculate the correlation matrix for all the assets.

Here are the detailed specifications. You should provide a Makefile
that compiles your code into an executable named "correl_matrix". Your
program should take exactly one command line argument, the name of the
file to read. An example of this format is given in
small.csv and year.csv. 

The first line of the data file gives the time increment (which you
can ignore), followed by comma-separated asset names. All subsequent
lines should have a time label (which you can ignore), followed by
comma-separated prices, of which there should be the same number as
there are assets. Note in year.csv that some of the prices are
null. Your program should handle null or non-numeric data in some
fields by just repeating the previous valid price for that asset. (Of
course, if there is no valid data in a column, that is an error.)

For each change in time step (it does not matter if the data is daily,
monthly, or something else), you should compute the rate of return. 

Once you have the rates of return, you can compute the average return
and standard deviation for that asset. As shown in the portfolio
theory modules, standard deviation is calculated as follows:

             /  1                        \
sigma = sqrt| ----- Sum_t (r_t - r_avg)^2 |
             \n - 1                      /

Next, you will calculate the covariance matrix, where each element is
the covariance of the two assets at that row and column.

    1    2    ... n
   -                  -
1 | s_11 s_12 ... s_1n |
2 | s_12 s_22          |
: | :        `.        |
n | s_1n        ` s_nn |
   -                  -

Recall that covariance for two assets a and b is given by:

        1
s_ab = --- Sum_t (ra_t - ra_avg)(rb_t - rb_avg)
        n

Finally, you will use the single asset's standard deviation to
calculate the correlation matrix. Recall that correlation is given by:

        s_ab
p_ab = -------
       s_a s_b

Where the matrix looks like:

    1    2    ... n
   -                  -
1 |  1   p_12 ... p_1n |
2 | p_12  1            |
: | :        `.        |
n | p_1n        `  1   |
   -                  -

Recall that -1 < p_ab < 1, where positive correlation means assets
change in the same direction, and negative correlation means assets
change in opposite directions. Therefore, the correlation of an asset
with itself should be exactly 1. Note, however, that these formulas
will not give you exactly p_aa = 1 but will approach 1 for large time
series of data. For this project, let p_aa be exactly 1 instead of
doing the correlation compuation.

Your program should print the result to stdout as follows.

[list of assets, newline delimited]
[correlation matrix]

Examples are given in small.out and year.out. Note that the matrix
must be formatted with open and close square brackets and
comma-delimited values, such that each floating point number has 7
spaces and four digits after the decimal point. See ios_base::width,
setprecision, and fixed in the C++ library.

For full credit, your program must valgrind cleanly. Of course, you
should test your program on many more inputs than those provided. 

While you are free to implement this in any way that is reasonable, I
recommend making an Asset class, keeping in mind that an asset has a
name, time series rate of return, average return, and standard
deviation. Making the covariance calculation a member function of 
Asset could also be nice. Rather than write a Matrix class, I chose to
typedef a vector of vectors of doubles.

Another good abstraction would be to separate your source code into
multiple files. One idea is to have files: main.cpp, parse.cpp,
asset.hpp, and asset.cpp. 

Testing
-------

We provide a pregrader that will run your tests against our reference
implementation. Before the deadline, when you do 'grade', the
pregrader will look for a file testcases.txt with the following
format:

#error
 
small.csv small.csv

#success
small.csv
year.csv

That is, a section with the #error header containing error test cases,
which should give an appropriate error message and exit with a failure
status. Note: you do not need to specify the program name this time,
since there should only be one executable to test. You are free to
make your own test files. (If you want to test "no command line
arguments," you should put a space on that line because the pregrader
skips blank lines. 

The next section has a #success header and is followed by test cases
that should have a success status. For these the pregrader will check
that your results match ours.

You may write as many test cases as you like, but you are limited to
20 seconds of compute time by the pregrader.