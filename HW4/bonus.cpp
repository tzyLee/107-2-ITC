#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

int main()
{
#define _ind(i, j) ((i)*WIDTH + (j))
#define absDiff(a, b) ((a) > (b) ? (a) - (b) : (b) - (a))
    // Initialize
    std::ifstream ifs("input.txt");
    std::vector<unsigned> tasks;
    unsigned long long sum = 0;
    unsigned n;
    ifs >> n;
    tasks.reserve(n);
    for (unsigned i = 0, temp = 0; i < n; ++i)
    {
        ifs >> temp;
        tasks.push_back(temp);
        sum += temp;
    }
    const unsigned HEIGHT = n + 1;
    const unsigned long long WIDTH = sum / 2 + 1;
    // Row: [0..n/2] col: [0..n/2]
    bool *table = new bool[WIDTH * HEIGHT];
    // Pseudo polynomial time algorithm
    // table[i, j]: there is a subset of [0..i-1] with sum equals to j
    for (unsigned i = 0; i < HEIGHT; ++i) // sum 0 is always possible
        table[_ind(i, 0)] = true;
    std::fill(table + 1, table + WIDTH, false); // When subset is [0..0], only sum 0 is possible
    for (unsigned i = 1; i < HEIGHT; ++i)
        for (unsigned j = 1; j < WIDTH; ++j)
        {
            // [0..i-1] have a subset with sum <=>
            table[_ind(i, j)] = table[_ind(i - 1, j)];
            // [0..i-2] have a subset with sum = sum
            if (tasks[i - 1] <= j) // Non-negative number
                table[_ind(i, j)] |= table[_ind(i - 1, j - tasks[i - 1])];
            // or [0..i-2] have a subset with sum = sum - tasks[i-1]
        }
    unsigned long long largestPossibleSum = 0;
    for (unsigned j = WIDTH; j > 0; --j)
        if (table[_ind(n, (largestPossibleSum = j - 1))])
            break;
    std::cout << absDiff(largestPossibleSum, sum - largestPossibleSum) << std::endl;
    std::vector<unsigned> group2;
    group2.reserve(sum - largestPossibleSum);
    // Backtrace
    for (unsigned i = n, j = largestPossibleSum; i > 0 || j > 0;)
    {
        // table[i, j] is true
        if (table[_ind(i - 1, j)]) // not include tasks[i - 1], group 2
            group2.push_back(tasks[--i]);
        else if (tasks[i - 1] <= j && table[_ind(i - 1, j - tasks[i - 1])]) // include tasks[i-1], group1
        {
            j -= tasks[i - 1];
            std::cout << tasks[--i] << ' ';
        }
    }
    std::cout.put('\n');
    for (const unsigned &i : group2)
        std::cout << i << ' ';
    delete[] table;
    return 0;
}