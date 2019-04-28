
#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <queue>
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
    ifstream ifs("input/problem1/input3", ifstream::binary); // TODO change to original

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

template <class T, class = typename std::enable_if<std::is_pointer<T>::value>::type>
struct greater_ptr
{
    bool operator()(T &a, T &b) const
    {
        return *a > *b;
    }
};

template <class Member, class = typename std::enable_if<std::is_pointer<Member>::value>::type>
struct MinHeap : public std::priority_queue<Member, std::vector<Member>, greater_ptr<Member>>
{
    using Super = std::priority_queue<Member, std::vector<Member>, greater_ptr<Member>>;
    MinHeap(std::size_t size) : Super()
    {
        Super::c.reserve(size);
    }
    void update(const Member &target) // key of target should be updated
    {
        auto res = std::find(Super::c.cbegin(), Super::c.cend(), target);
        assert(res != Super::c.cend());
        for (auto index = std::distance(Super::c.cbegin(), res); index; index /= 2) // Bubble up
            Super::c[index] = Super::c[index / 2];
        this->pop(); // Remove top
        this->push(target);
    }
};

enum Direction : char
{
    Unset = '\0',
    Up = 'u',
    Down = 'd',
    Left = 'l',
    Right = 'r'
};

struct Node
{
    Node(int i, int j) : minDistance(INFINITY), _i(i), _j(j), lastNode(Direction::Unset), removed(false) {}
    Node() = delete;
    void update(MinHeap<const Node *> &pq, double distance, Direction direction)
    {
        if (!removed && minDistance > distance)
        {
            minDistance = distance;
            if (lastNode == Direction::Unset) // this node had not been traversed before
                pq.push(this);
            else
                pq.update(this);
            lastNode = direction;
        }
    }
    bool operator>(const Node &other) const
    {
        return minDistance > other.minDistance;
    }
    double minDistance;
    int _i, _j;
    Direction lastNode; // How lastNode get to this
    bool removed;       // TODO use Direction sign bit as bool
};

int main()
{
#define WIDTH (m + 1)
#define HEIGHT (n + 1)
#define _ind(i, j) ((i)*WIDTH + (j))
    readParameters();
    /***************************
     *
     *   your code/functions here
     *
     ***************************/
    std::vector<Node> nodes;
    nodes.reserve(WIDTH * HEIGHT);
    for (int i = 0; i < HEIGHT; ++i)
        for (int j = 0; j < WIDTH; ++j)
            nodes.emplace_back(i, j);
    MinHeap<const Node *> pq(nodes.size());
    // Dijkstra's algorithm
    nodes[_ind(0, 0)].minDistance = 0;
    int i = 0, j = 0;
    do
    {
        nodes[_ind(i, j)].removed = true; // mark current node as visited
        double distance = nodes[_ind(i, j)].minDistance;
        if (i > 1)
            nodes[_ind(i - 1, j)].update(pq, distance + v[i - 1][j], Direction::Up);
        if (i < n)
            nodes[_ind(i + 1, j)].update(pq, distance + v[i][j], Direction::Down);
        if (j > 1)
            nodes[_ind(i, j - 1)].update(pq, distance + h[i][j - 1], Direction::Left);
        if (j < m)
            nodes[_ind(i, j + 1)].update(pq, distance + h[i][j], Direction::Right);
        i = pq.top()->_i, j = pq.top()->_j; // get next node index
        pq.pop();                           // remove current node
        // } while (!pq.empty() && !(i == n && j == m)); // while not reach destination or have node left to check
    } while (!pq.empty()); // while not reach destination or have node left to check
    // Backtrace
    std::vector<char> route;
    route.reserve(m + n);                                    // minimum nodes to travel                                            // destination
    std::cout << nodes[_ind(n, m)].minDistance << std::endl; // total distance = min distance from source to destination
    Direction temp = Direction::Unset;
    for (int i = n, j = m; i != 0 || j != 0;)
    {
        switch (temp = nodes[_ind(i, j)].lastNode) // Use switch should be faster than if-else
        {
        case Direction::Up: // Goes down to get to lastNode
            ++i;
            break;
        case Direction::Down:
            --i;
            break;
        case Direction::Left:
            ++j;
            break;
        case Direction::Right:
            --j;
            break;
        default:
            assert(temp != Direction::Unset);
        }
        route.emplace_back(temp);
    }
    for (auto it = route.rbegin(); it != route.rend(); ++it)
        std::cout << *it;
    release();
    return 0;
}
