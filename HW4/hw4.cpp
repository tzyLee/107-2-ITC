
#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <vector>
using namespace std;

/************************************
*  Note that in general using global
*  variables is a bad habit.
*  I use globals here because I don't 
*  want to scare you with "double***"
*************************************/

int m;
int n;
double **v; // n*(m+1) 2D-array
double **h; // (n+1)*m 2D-array

// For internal use only. Do not really use them directly
double *real_v;
double *real_h;

void readParameters()
{
    ifstream ifs("input/problem1/input1", ifstream::binary); // TODO change to original

    ifs.read((char *)&m, sizeof(int));
    ifs.read((char *)&n, sizeof(int));

    real_v = new double[n * (m + 1)];
    real_h = new double[(n + 1) * m];

    ifs.read((char *)real_v, sizeof(double) * n * (m + 1));
    ifs.read((char *)real_h, sizeof(double) * (n + 1) * m);

    v = new double *[n];
    for (int i = 0; i < n; ++i)
        v[i] = &(real_v[i * (m + 1)]);

    h = new double *[n + 1];
    for (int i = 0; i < n + 1; ++i)
        h[i] = &(real_h[i * m]);

    ifs.close();
}

void release()
{
    delete[] v;
    delete[] h;

    delete[] real_v;
    delete[] real_h;
}

template <class Member, class = typename std::enable_if<std::is_pointer<Member>::value>::type>
struct MinHeap
{
    using Base = typename std::remove_const<typename std::remove_pointer<Member>::type>::type;
    MinHeap(std::vector<Base> &source) : container()
    {
        container.reserve(source.size());
        // for (const Base &node : source)
        //     container.push_back(&node);
        // std::make_heap(container.begin(), container.end(), MinHeap::comp);
    }
    void pop_top(int &i, int &j)
    {
        std::pop_heap(container.begin(), container.end(), MinHeap::comp);
        const Base &smallest = *container.back();
        i = smallest._i;
        j = smallest._j;
        container.pop_back();
    }
    void push(const Member &newNode)
    {
        container.push_back(newNode);
        std::push_heap(container.begin(), container.end(), MinHeap::comp);
    }
    void update(const Member &target, double newKey)
    {
        assert(target->minDistance >= newKey); // Always update to a smaller key
        auto res = std::find(container.cbegin(), container.cend(), target);
        assert(res != container.cend());
        assert(*res == target);
        auto index = std::distance(container.cbegin(), res);
        assert(container[index] == target);
        for (; index && *container[index / 2] > newKey; index /= 2) // If parent is bigger than current
            container[index] = container[index / 2];
        container[index] = target;
    }
    static bool comp(Member &a, Member &b)
    {
        return *a > *b;
    }
    bool empty() const
    {
        return container.empty();
    }
    std::vector<Member> container;
};

enum class Direction : char
{
    Unset = '\0',
    Up = 'u',
    Down = 'd',
    Left = 'l',
    Right = 'r'
};

struct Node
{
    Node(int i, int j) : lastNode(Direction::Unset), minDistance(INFINITY), _i(i), _j(j), traversed(false) {}
    Node() : Node(-1, -1) {}
    void update(MinHeap<const Node *> &pq, double distance, Direction direction)
    {
        if (!traversed && minDistance > distance)
        {
            minDistance = distance;
            if (lastNode == Direction::Unset) // this node had not been traversed before
                pq.push(this);
            else
                pq.update(this, minDistance);
            lastNode = direction;
        }
    }
    bool operator>(const Node &other) const
    {
        return minDistance > other.minDistance;
    }
    bool operator>(double dist) const
    {
        return minDistance > dist;
    }
    bool operator=(const Node &other) const
    {
        return this == &other;
    }
    double minDistance;
    int _i, _j;
    Direction lastNode;
    bool traversed;
};

int main()
{
#define _ind(i, j) ((i)*n + (j))
    readParameters();
    /***************************
     *
     *   your code/functions here
     *
     ***************************/
    std::vector<Node> nodes;
    nodes.reserve(m * n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            nodes.emplace_back(i, j);
    nodes[_ind(0, 0)].minDistance = 0;
    MinHeap<const Node *> pq(nodes);
    Node *current = &nodes[_ind(0, 0)], *source = current;
    do
    {
        int i = current->_i, j = current->_j;
        current->traversed = true;
        if (i > 1)
            nodes[_ind(i - 1, j)].update(pq, current->minDistance + v[i - 1][j], Direction::Down);
        if (i < n)
            nodes[_ind(i + 1, j)].update(pq, current->minDistance + v[i][j], Direction::Up);
        if (j > 1)
            nodes[_ind(i, j - 1)].update(pq, current->minDistance + h[i][j - 1], Direction::Right);
        if (j < m)
            nodes[_ind(i, j + 1)].update(pq, current->minDistance + h[i][j], Direction::Left);
        pq.pop_top(i, j);
        assert(i >= 0 && i < n && j >= 0 && j < m);
        current = &nodes[_ind(i, j)];
    } while (!pq.empty());
    // Backtrace
    std::vector<char> route;
    route.reserve(m + n);                                            // minimum nodes to travel                                            // destination
    std::cout << nodes[_ind(n - 1, m - 1)].minDistance << std::endl; // total distance = min distance from source to destination
    Direction temp = Direction::Unset;
    for (int i = n - 1, j = m - 1; i != 0 || j != 0;)
    {
        switch (temp = nodes[_ind(i, j)].lastNode)
        {
        case Direction::Up:
            --i;
            break;
        case Direction::Down:
            ++i;
            break;
        case Direction::Left:
            --j;
            break;
        case Direction::Right:
            ++j;
            break;
        default:
            assert(temp != Direction::Unset);
        }
        route.emplace_back(static_cast<char>(temp));
    }
    for (auto it = route.rbegin(); it != route.rend(); ++it)
        std::cout << *it;
    release();
    return 0;
}
