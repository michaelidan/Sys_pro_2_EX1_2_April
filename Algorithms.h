// michael9090124@gmail.com

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Graph.h"

namespace graph {

class Algorithms {
private:
    Graph& g; // הפניה לגרף שעליו נריץ את האלגוריתמים
    void dfsUtil(int u, bool* visited, Graph& tree); // הוספת ההצהרה של dfsUtil

public:
    Algorithms(Graph& graph); // בנאי שמקבל גרף

    Graph bfs(int source); // מחזיר עץ BFS מקודקוד מקור
    Graph dfs(int source); // מחזיר עץ או יער DFS מקודקוד מקור
    Graph dijkstra(int start); // מחזיר עץ של מסלולים קצרים ביותר
    Graph prim(); // מחזיר עץ פורש מינימלי באמצעות אלגוריתם Prim
    Graph kruskal(); // מחזיר עץ פורש מינימלי באמצעות אלגוריתם Kruskal
};

} // סוף מרחב השמות graph

#endif
