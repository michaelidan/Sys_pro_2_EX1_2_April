// michael9090124@gmail.com

#include "Algorithms.h"
#include "DataStructures.h"
#include <iostream> // עבור std::cout ו-std::endl

namespace graph {

Algorithms::Algorithms(Graph& graph) : g(graph) {}

Graph Algorithms::bfs(int start) {
    int n = g.getNumVertices();
    if (start < 0 || start >= n) {
        throw "Invalid starting vertex!";
    }

    Graph tree(n);
    bool* visited = new bool[n]();
    Queue q(n);
    visited[start] = true;
    q.enqueue(start);

    while (!q.isEmpty()) {
        int u = q.dequeue();
        for (int i = 0; i < g.getSize(u); i++) {
            int v = g.getAdjList(u)[i];
            if (!visited[v]) {
                visited[v] = true;
                q.enqueue(v);
                tree.addEdge(u, v, g.getWeights(u)[i]);
            }
        }
    }

    delete[] visited;
    return tree;
}

Graph Algorithms::dfs(int start) {
    int n = g.getNumVertices();
    if (start < 0 || start >= n) {
        throw "Invalid starting vertex!";
    }

    Graph tree(n);
    bool* visited = new bool[n]();
    Algorithms::dfsUtil(start, visited, tree);

    delete[] visited;
    return tree;
}

void Algorithms::dfsUtil(int u, bool* visited, Graph& tree) {
    visited[u] = true;
    for (int i = 0; i < g.getSize(u); i++) {
        int v = g.getAdjList(u)[i];
        if (!visited[v]) {
            tree.addEdge(u, v, g.getWeights(u)[i]);
            Algorithms::dfsUtil(v, visited, tree);
        }
    }
}

Graph Algorithms::dijkstra(int start) {
    int n = g.getNumVertices();
    if (start < 0 || start >= n) {
        throw "Invalid starting vertex!";
    }

    // בדיקה אם יש משקלים שליליים
    for (int u = 0; u < n; u++) {
        for (int i = 0; i < g.getSize(u); i++) {
            if (g.getWeights(u)[i] < 0) {
                throw "Dijkstra's algorithm does not support negative weights!";
            }
        }
    }

    const int INF = 9999999; // ערך גדול שמייצג אינסוף
    int* dist = new int[n];
    int* parent = new int[n];
    bool* inTree = new bool[n]();
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        parent[i] = -1;
    }

    Graph tree(n);
    PriorityQueue pq(n);
    dist[start] = 0;
    pq.insert(start, 0);

    while (!pq.isEmpty()) {
        PriorityQueue::Item item = pq.extractMin();
        int u = item.vertex;
        if (inTree[u]) continue;
        inTree[u] = true;

        if (parent[u] != -1) {
            for (int i = 0; i < g.getSize(parent[u]); i++) {
                if (g.getAdjList(parent[u])[i] == u) {
                    tree.addEdge(parent[u], u, g.getWeights(parent[u])[i]);
                    break;
                }
            }
        }

        for (int i = 0; i < g.getSize(u); i++) {
            int v = g.getAdjList(u)[i];
            int weight = g.getWeights(u)[i];
            if (!inTree[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.insert(v, dist[v]);
            }
        }
    }

    delete[] dist;
    delete[] parent;
    delete[] inTree;
    return tree;
}

Graph Algorithms::prim() {
    int n = g.getNumVertices();
    if (n == 0) {
        return Graph(0); // גרף ריק
    }

    bool* inMST = new bool[n](); // מסמן מי נמצא בעץ
    int* key = new int[n]; // משקל מינימלי לחיבור לעץ
    int* parent = new int[n]; // שומר את ההורה בעץ
    const int INF = 9999999; // ערך גדול שמייצג אינסוף
    for (int i = 0; i < n; i++) {
        key[i] = INF;
        parent[i] = -1;
    }

    Graph mst(n); // העץ הפורש המינימלי שיוחזר
    key[0] = 0; // מתחילים מקודקוד 0

    // שלב 1: חישוב העץ הפורש המינימלי
    for (int count = 0; count < n; count++) {
        // מצא את הקודקוד עם ה-key המינימלי מבין אלה שלא בעץ
        int minKey = INF;
        int u = -1;
        for (int v = 0; v < n; v++) {
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }

        if (u == -1) break; // אין יותר קודקודים להוסיף
        inMST[u] = true;

        // עדכן את ה-key של השכנים של u
        for (int i = 0; i < g.getSize(u); i++) {
            int v = g.getAdjList(u)[i];
            int weight = g.getWeights(u)[i];
            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
            }
        }
    }

    // שלב 2: בניית העץ על סמך מערך ה-parent הסופי
    for (int v = 0; v < n; v++) {
        if (parent[v] != -1) {
            mst.addEdge(parent[v], v, key[v]);
        }
    }

    delete[] inMST;
    delete[] key;
    delete[] parent;

    return mst;
}

Graph Algorithms::kruskal() {
    int n = g.getNumVertices();
    if (n == 0) {
        return Graph(0); // גרף ריק
    }

    struct Edge {
        int src, dest, weight;
    };

    // איסוף כל הקשתות
    Edge* edges = new Edge[n * n];
    int edgeCount = 0;
    for (int u = 0; u < n; u++) {
        for (int i = 0; i < g.getSize(u); i++) {
            int v = g.getAdjList(u)[i];
            if (u < v) { // מונע כפילויות
                edges[edgeCount] = {u, v, g.getWeights(u)[i]};
                edgeCount++;
            }
        }
    }

    // מיון הקשתות לפי משקל
    for (int i = 0; i < edgeCount - 1; i++) {
        for (int j = 0; j < edgeCount - i - 1; j++) {
            if (edges[j].weight > edges[j + 1].weight) {
                Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }

    // יצירת העץ הפורש המינימלי
    Graph mst(n);
    int* parent = new int[n]; // מערך הורים לבדיקת מחזורים
    for (int i = 0; i < n; i++) {
        parent[i] = i; // כל קודקוד הוא ההורה של עצמו בתחילה
    }

    auto find = [&](int u) -> int {
        if (u < 0 || u >= n) {
            throw "Invalid vertex in find!";
        }
        int originalU = u;
        while (u != parent[u]) {
            u = parent[u];
            if (u < 0 || u >= n) {
                throw "Invalid parent in find!";
            }
        }
        return u;
    };

    auto unionSets = [&](int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        parent[rootU] = rootV;
    };

    int edgesAdded = 0;
    for (int i = 0; i < edgeCount && edgesAdded < n - 1; i++) {
        int u = edges[i].src;
        int v = edges[i].dest;
        if (find(u) != find(v)) {
            unionSets(u, v);
            mst.addEdge(u, v, edges[i].weight);
            edgesAdded++;
        }
    }

    delete[] edges;
    delete[] parent;
    if (edgesAdded != n - 1) {
        throw "Graph is not connected - no minimum spanning tree exists";
    }
    return mst;
}

} // סוף מרחב השמות graph
