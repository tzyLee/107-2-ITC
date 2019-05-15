#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

int main() {
    std::ifstream ifs("bonus.txt");
    std::unordered_map<std::string, unsigned> freq;
    std::string temp;
    unsigned k = 0;
    for (char temp; (temp = ifs.peek()); ifs.get())  // TODO remove this
        if (temp >= '0' && temp <= '9')
            break;

    ifs >> k;  // Get number of words to output
    freq.reserve(k);

    while (ifs && ifs >> temp)  // Count word frequency
        ++freq[std::move(temp)];

    using Counter = std::pair<std::string, unsigned>;
    std::vector<Counter> container;
    for (auto &&i : freq)  // Add all pair to vector
        container.emplace_back(std::move(i.first), i.second);

    std::sort(container.begin(), container.end(),
              [](const Counter &a, const Counter &b) {
                  return a.second != b.second ? a.second > b.second
                                              : a.first < b.first;
              });
    for (unsigned i = 0; i < k; ++i)
        std::cout << container[i].first << ' ';
    std::cout.flush();
    return 0;
}