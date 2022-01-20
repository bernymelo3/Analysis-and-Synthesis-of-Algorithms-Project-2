#include <iostream>
#include <set>
#include <queue>
#include <vector>
#include <cstring>

using namespace std;

/*
 * state
 * - 0 // Initial
 * - v // Visited
 * - s // Visited and on stack
 */

typedef struct node {
    int id;
    char color = 'w';
    char state = '0';
    vector<struct node*> parents;
} NODE;

queue<int> stack;
vector<int> solutions;
int vertices_count;
NODE *graph;


bool dfs_search(NODE* current) {
    if (current->state == '0')
    {
        current->state = 's';

        for (NODE* parent : current->parents) {
            if (parent->state == '0' && dfs_search(parent)) {
                return true;
            } else if (parent->state == 's') {
                return true;
            }
        }
    }
    current->state = 'v';
    return false;
}

bool checkDAG() {
    for (int i = 0; i < vertices_count; i++) {
        if(dfs_search(&graph[i])) {
            return true;
        }
    }
    return false;
}


void colorizePath(NODE* source, char old_color, char new_color, bool invalidate) {

    stack.push(source->id);

    while (!stack.empty())
    {

        bool reset = false;

        NODE* next = &graph[stack.front()];
        stack.pop();

        if (next->color == old_color) {
            next->color = new_color;
            if (invalidate == true) {
                next->color = 'b';
                reset = true;
            }
        }

        for (NODE* parent : next->parents)
        {
            if (next->color == 'b') {
                parent->color = 'b';
            }
            if (parent->state == '0') {
                parent->state = 'v';
                stack.push(parent->id);
            }
        }

        if (reset) {
            reset = false;
            next->color = new_color;
        }
    }
}

int main() {
    int v1, v2, edges_count, i, v, u;

    // Parse information
    scanf("%d %d %d %d ", &v1, &v2, &vertices_count, &edges_count);

    // Init graph
    graph = new NODE[vertices_count];

    for (i = 0; i < vertices_count; i++) {
        graph[i].id = i;
    }

    // Parse vertices
    for (i = 0; i < edges_count; i++) {
        scanf("%d %d ", &v, &u);
        graph[u - 1].parents.push_back(&graph[v - 1]);

        if (graph[u - 1].parents.size() > 2) {
            cout << "0" << endl;
            return 0;
        }
    }

    // Run DAG Check

    if (checkDAG()) {
        cout << "0" << endl;
        return 0;
    }

    for(i = 0; i < vertices_count; i++)
        graph[i].state = '0';

    colorizePath(&graph[v1 - 1], 'w', 'r', false);

    for(i = 0; i < vertices_count; i++)
        graph[i].state = '0';

    colorizePath(&graph[v2 - 1], 'r', 'g', true);


    for (i = 0; i < vertices_count; i++) {
        if (graph[i].color == 'g') {
            solutions.push_back(graph[i].id);
        }
    }

    if (!solutions.empty()) {
        for(int num : solutions) {
            cout << num + 1 << " ";
        }
    } else {
        cout << "-";
    }
    cout << endl;

    return 0;

}
