#include<iostream>
#include<algorithm>
#include<map>
#include<climits>
#include<queue>
using namespace std;


struct island {
    int x, y, idx;
};

struct queue_el {
    int i;
    int dist;

    bool operator<(const queue_el &other) const {
        return other.dist < dist;
    }

};

const int N =  200000 + 42;
int n;
island islands[N];
island x_sorted[N];
island y_sorted[N];

using distance_t = map<int, int>;
distance_t currentDistance;

using edges_t = map<int, map<int, int>>;
edges_t edges;


int dist(const island &p1, const island &p2) {
    return min(abs(p1.x - p2.x), abs(p1.y - p2.y));
}

bool byX(const island &i1, const island &i2) {
    return i1.x < i2.x;
}

bool byY(const island &i1, const island &i2) {
    return i1.y < i2.y;
}



int main() {
    ios_base::sync_with_stdio(false);

    cin>>n;

    int a, b;
    for (int i = 0; i < n; i++)
    {
        cin>>a>>b;
        islands[i] = {a, b, i};
        x_sorted[i] = {a, b, i};
        y_sorted[i] = {a, b, i};
    }
    
    sort(x_sorted, x_sorted + n, byX);
    sort(y_sorted, y_sorted + n, byY);

    for (int i = 0; i < n; i++)
    {
        edges[islands[i].idx] = map<int, int>();
    }

    for (int i = 0; i < n - 1; i++)
    {
        edges[x_sorted[i].idx][x_sorted[i + 1].idx] = dist(x_sorted[i], x_sorted[i+1]);
        edges[x_sorted[i+1].idx][x_sorted[i].idx] = dist(x_sorted[i], x_sorted[i+1]);

        edges[y_sorted[i].idx][y_sorted[i + 1].idx] = dist(y_sorted[i], y_sorted[i+1]);
        edges[y_sorted[i+1].idx][y_sorted[i].idx] = dist(y_sorted[i], y_sorted[i+1]);
    }
    
    priority_queue<queue_el> q;
    q.emplace(queue_el{islands[0].idx, 0});
    currentDistance[islands[0].idx] = 0;

    for (int i = 1; i < n; i++)
    {
        currentDistance[islands[i].idx] = INT_MAX;
    }
    
    
    while (q.size() > 0) 
    {
        queue_el closest = q.top();
        q.pop();

        if (closest.dist > currentDistance[closest.i]) {
            continue;
        }

        for (auto neighb : edges[closest.i]) {
            int new_dist = closest.dist + neighb.second;
            if (new_dist < currentDistance[neighb.first]) {
                currentDistance[neighb.first] = new_dist;
                queue_el el;
                el.dist = new_dist;
                el.i = neighb.first;
                q.emplace(queue_el{neighb.first, new_dist});
            }
        }
    }

    cout<<currentDistance[islands[n-1].idx]<<endl;
}
