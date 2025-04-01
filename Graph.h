// michael9090124@gmail.com

#ifndef GRAPH_H
#define GRAPH_H

namespace graph {

class Graph {
private:
    int numVertices;  // מספר הקודקודים בגרף
    int** adjList;    // מערך של רשימות שכנים
    int** weights;    // מערך של משקלות הקשתות
    int* sizes;       // מערך של גדלי רשימות השכנים

public:
    Graph(int vertices); // בנאי
    Graph(const Graph& other); // קונסטרקטור העתקה
    Graph& operator=(const Graph& other); // אופרטור השמה
    ~Graph(); // מפרק

    void addEdge(int src, int dest, int weight = 1); // מוסיף קשת (עם ברירת מחדל למשקל)
    void removeEdge(int src, int dest); // מוחק קשת
    void print_graph(); // מדפיס את הגרף

    // פונקציות גישה
    int getNumVertices() const;
    int getSize(int v) const;
    int* getAdjList(int v) const;
    int* getWeights(int v) const;
};

} // סוף מרחב השמות graph

#endif
