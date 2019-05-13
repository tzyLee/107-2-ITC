#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

int main() {
    std::ifstream ifs("bonus.txt");
    std::unordered_map<std::string, unsigned> freq;
    using Counter = std::unordered_map<std::string, unsigned>::value_type;
    std::vector<std::reference_wrapper<Counter>> word_freq;
    std::string temp;
    unsigned k = 0;
    for (char temp; (temp = ifs.peek()); ifs.get())  // TODO remove this
        if (temp >= '0' && temp <= '9')
            break;

    ifs >> k;  // Get number of words to output
    freq.reserve(k);

    while (ifs && ifs >> temp)  // Count word frequency
        ++freq[std::move(temp)];

    word_freq.reserve(freq.size());
    auto comp = [](const Counter &a, const Counter &b) {
        return a.second != b.second ? a.second > b.second : a.first < b.first;
    };
    for (auto &&i : freq) {
        word_freq.emplace_back(std::move(std::ref(i)));
        std::push_heap(word_freq.begin(), word_freq.end(), comp);
        if (word_freq.size() > k) {
            std::pop_heap(word_freq.begin(), word_freq.end(), comp);
            word_freq.pop_back();
        }
    }
    std::vector<const char *> result;
    result.reserve(k);
    for (; k; --k) {  // Pop top k from heap
        std::pop_heap(word_freq.begin(), word_freq.end(), comp);
        result.push_back(word_freq.back().get().first.c_str());
        word_freq.pop_back();
    }
    for (auto it = result.rbegin(); it != result.rend(); ++it)
        std::cout << *it << ' ';
    std::cout.flush();
    return 0;
}