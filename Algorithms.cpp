// michael9090124@gmail.com

#include "Algorithms.h"
#include "DataStructures.h"
#include <iostream> // עבור std::cout ו-std::endl

namespace graph {

// בנאי המחלקה Algorithms, מקבל כפרמטר גרף
Algorithms::Algorithms(Graph& graph) : g(graph) {}

// פונקציה המבצעת BFS (חיפוש לרוחב) על גרף, החל מקודקוד start
Graph Algorithms::bfs(int start) {
    // n שומר את מספר הקודקודים בגרף
    int n = g.getNumVertices();
    // בדיקה שהקודקוד start חוקי
    if (start < 0 || start >= n) {
        throw "Invalid starting vertex!";
    }

    // יצירת גרף חדש tree שייצג את עץ ה-BFS
    Graph tree(n);
    // מערך visited מציין אילו קודקודים כבר נבקרו
    bool* visited = new bool[n]();
    // תור q לניהול הקודקודים שנצטרך לבקר
    Queue q(n);
    // סימון הקודקוד ההתחלתי כנבקר
    visited[start] = true;
    // הוספת הקודקוד ההתחלתי לתור
    q.enqueue(start);

    // כל עוד התור אינו ריק
    while (!q.isEmpty()) {
        // הוצאת קודקוד מהתור
        int u = q.dequeue();
        // עבור כל השכנים של הקודקוד u
        for (int i = 0; i < g.getSize(u); i++) {
            // קבלת שכן v של u
            int v = g.getAdjList(u)[i];
            // אם השכן v עדיין לא נבקר
            if (!visited[v]) {
                // סימון v כנבקר
                visited[v] = true;
                // הוספת v לתור
                q.enqueue(v);
                // הוספת הקשת (u, v) לעץ ה-BFS
                tree.addEdge(u, v, g.getWeights(u)[i]);
            }
        }
    }

    // שחרור זיכרון
    delete[] visited;
    // החזרת עץ ה-BFS
    return tree;
}

// פונקציה המבצעת DFS (חיפוש לעומק) על גרף, החל מקודקוד start
Graph Algorithms::dfs(int start) {
    // n שומר את מספר הקודקודים בגרף
    int n = g.getNumVertices();
    // בדיקה שהקודקוד start חוקי
    if (start < 0 || start >= n) {
        throw "Invalid starting vertex!";
    }

    // יצירת גרף חדש tree שייצג את עץ ה-DFS
    Graph tree(n);
    // מערך visited מציין אילו קודקודים כבר נבקרו
    bool* visited = new bool[n]();
    // קריאה לפונקציה הרקורסיבית dfsUtil
    Algorithms::dfsUtil(start, visited, tree);

    // שחרור זיכרון
    delete[] visited;
    // החזרת עץ ה-DFS
    return tree;
}

// פונקציה רקורסיבית עבור DFS
void Algorithms::dfsUtil(int u, bool* visited, Graph& tree) {
    // סימון הקודקוד הנוכחי u כנבקר
    visited[u] = true;
    // עבור כל השכנים של הקודקוד u
    for (int i = 0; i < g.getSize(u); i++) {
        // קבלת שכן v של u
        int v = g.getAdjList(u)[i];
        // אם השכן v עדיין לא נבקר
        if (!visited[v]) {
            // הוספת הקשת (u, v) לעץ ה-DFS
            tree.addEdge(u, v, g.getWeights(u)[i]);
            // קריאה רקורסיבית ל-dfsUtil עבור v
            Algorithms::dfsUtil(v, visited, tree);
        }
    }
}

// פונקציה המבצעת את אלגוריתם דייקסטרה למציאת מסלולים קצרים ביותר
Graph Algorithms::dijkstra(int start) {
    // n שומר את מספר הקודקודים בגרף
    int n = g.getNumVertices();
    // בדיקה שהקודקוד start חוקי
    if (start < 0 || start >= n) {
        throw "Invalid starting vertex!";
    }

    // בדיקה אם יש קשתות עם משקל שלילי
    for (int u = 0; u < n; u++) {
        for (int i = 0; i < g.getSize(u); i++) {
            if (g.getWeights(u)[i] < 0) {
                throw "Dijkstra's algorithm does not support negative weights!";
            }
        }
    }

    // INF מייצג אינסוף (ערך גדול מאוד)
    const int INF = 9999999;
    // dist[i] שומר את המרחק הקצר ביותר מ-start ל-i
    int* dist = new int[n];
    // parent[i] שומר את האב של i במסלול הקצר ביותר
    int* parent = new int[n];
    // inTree[i] מציין האם הקודקוד i כבר בעץ המסלולים הקצרים
    bool* inTree = new bool[n]();

    // אתחול dist ו-parent
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        parent[i] = -1;
    }

    // יצירת גרף חדש tree שייצג את עץ המסלולים הקצרים
    Graph tree(n);
    // תור עדיפויות pq לניהול קודקודים לפי מרחק מ-start
    PriorityQueue pq(n);
    // המרחק של start מעצמו הוא 0
    dist[start] = 0;
    // הוספת start לתור העדיפויות
    pq.insert(start, 0);

    // כל עוד תור העדיפויות אינו ריק
    while (!pq.isEmpty()) {
        // הוצאת הקודקוד u עם המרחק המינימלי
        PriorityQueue::Item item = pq.extractMin();
        int u = item.vertex;
        // אם u כבר בעץ, המשך לאיטרציה הבאה
        if (inTree[u]) continue;
        // סימון u כנמצא בעץ
        inTree[u] = true;

        // אם u אינו קודקוד ההתחלה, הוסף את הקשת (parent[u], u) לעץ
        if (parent[u] != -1) {
            for (int i = 0; i < g.getSize(parent[u]); i++) {
                if (g.getAdjList(parent[u])[i] == u) {
                    tree.addEdge(parent[u], u, g.getWeights(parent[u])[i]);
                    break;
                }
            }
        }

        // עדכון מרחקים עבור השכנים של u
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

    // שחרור זיכרון
    delete[] dist;
    delete[] parent;
    delete[] inTree;
    // החזרת עץ המסלולים הקצרים
    return tree;
}

// פונקציה המבצעת את אלגוריתם פרים למציאת עץ פורש מינימלי (MST)
Graph Algorithms::prim() {
    // n שומר את מספר הקודקודים בגרף
    int n = g.getNumVertices();
    // אם הגרף ריק, החזר גרף ריק
    if (n == 0) {
        return Graph(0);
    }

    // inMST[i] מציין האם הקודקוד i כבר נמצא בעץ
    bool* inMST = new bool[n]();
    // key[i] שומר את המשקל המינימלי של קשת המחברת את i לעץ
    int* key = new int[n];
    // parent[i] שומר את האב של i בעץ
    int* parent = new int[n];
    // INF מייצג אינסוף (ערך גדול מאוד)
    const int INF = 9999999;
    // אתחול key ו-parent
    for (int i = 0; i < n; i++) {
        key[i] = INF;
        parent[i] = -1;
    }

    // יצירת גרף חדש mst שייצג את ה-MST
    Graph mst(n);
    // אתחול key[0] ל-0 (קודקוד ההתחלה)
    key[0] = 0;

    // שלב 1: בניית ה-MST
    for (int count = 0; count < n; count++) {
        // מציאת הקודקוד u עם key מינימלי
        int minKey = INF;
        int u = -1;
        for (int v = 0; v < n; v++) {
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }

        // אם לא נמצא קודקוד, סיום הלולאה
        if (u == -1) break;
        // סימון u כנמצא בעץ
        inMST[u] = true;

        // עדכון key ו-parent עבור השכנים של u
        for (int i = 0; i < g.getSize(u); i++) {
            int v = g.getAdjList(u)[i];
            int weight = g.getWeights(u)[i];
            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
            }
        }
    }

    // שלב 2: יצירת ה-MST מתוך מערך parent
    for (int v = 0; v < n; v++) {
        if (parent[v] != -1) {
            mst.addEdge(parent[v], v, key[v]);
        }
    }

    // שחרור זיכרון
    delete[] inMST;
    delete[] key;
    delete[] parent;

    // החזרת ה-MST
    return mst;
}

// פונקציה המבצעת את אלגוריתם קרוסקל למציאת עץ פורש מינימלי (MST)
Graph Algorithms::kruskal() {
    // n שומר את מספר הקודקודים בגרף
    int n = g.getNumVertices();
    // אם הגרף ריק, החזר גרף ריק
    if (n == 0) {
        return Graph(0);
    }

    // מבנה נתונים לייצוג קשת
    struct Edge {
        int src, dest, weight;
    };

    // איסוף כל הקשתות
    Edge* edges = new Edge[n * n];
    int edgeCount = 0;
    for (int u = 0; u < n; u++) {
        for (int i = 0; i < g.getSize(u); i++) {
            int v = g.getAdjList(u)[i];
            // מניעת קשתות כפולות
            if (u < v) {
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

    // יצירת ה-MST
    Graph mst(n);
    // מערך parent לבדיקת מעגלים
    int* parent = new int[n];
    // אתחול כל קודקוד כהורה של עצמו
    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }

    // פונקציה למציאת שורש של קבוצה
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

    // פונקציה לאיחוד קבוצות
    auto unionSets = [&](int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        parent[rootU] = rootV;
    };

    int edgesAdded = 0;
    // הוספת קשתות ל-MST עד שהוא מכיל n - 1 קשתות
    for (int i = 0; i < edgeCount && edgesAdded < n - 1; i++) {
        int u = edges[i].src;
        int v = edges[i].dest;
        // אם u ו-v אינם באותה קבוצה, הוסף את הקשת
        if (find(u) != find(v)) {
            unionSets(u, v);
            mst.addEdge(u, v, edges[i].weight);
            edgesAdded++;
        }
    }

    // שחרור זיכרון
    delete[] edges;
    delete[] parent;
    // אם לא נמצא MST, זרוק חריגה
    if (edgesAdded != n - 1) {
        throw "Graph is not connected - no minimum spanning tree exists";
    }
    // החזרת ה-MST
    return mst;
}

} // סוף מרחב השמות graph
