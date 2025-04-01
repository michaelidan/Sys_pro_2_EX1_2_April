// michael9090124@gmail.com

#include "Graph.h"
#include <iostream> // עבור std::cout ו-std::endl

namespace graph {

// בניית גרף עם מספר קודקודים
Graph::Graph(int vertices) : numVertices(vertices) {
    if (vertices < 0) {
        throw "Number of vertices cannot be negative!";
    }

    adjList = new int*[numVertices];
    weights = new int*[numVertices];
    sizes = new int[numVertices];

    for (int i = 0; i < numVertices; i++) {
        adjList[i] = nullptr;
        weights[i] = nullptr;
        sizes[i] = 0;
    }
}

Graph::Graph(const Graph& other) : numVertices(other.numVertices) {
    adjList = new int*[numVertices];
    weights = new int*[numVertices];
    sizes = new int[numVertices];

    for (int i = 0; i < numVertices; i++) {
        sizes[i] = other.sizes[i];
        if (sizes[i] > 0) {
            adjList[i] = new int[sizes[i]];
            weights[i] = new int[sizes[i]];
            for (int j = 0; j < sizes[i]; j++) {
                adjList[i][j] = other.adjList[i][j];
                weights[i][j] = other.weights[i][j];
            }
        } else {
            adjList[i] = nullptr;
            weights[i] = nullptr;
        }
    }
}

Graph& Graph::operator=(const Graph& other) {
    if (this == &other) {
        return *this;
    }

    for (int i = 0; i < numVertices; i++) {
        if (sizes[i] > 0) {
            delete[] adjList[i];
            delete[] weights[i];
        }
    }
    delete[] adjList;
    delete[] weights;
    delete[] sizes;

    numVertices = other.numVertices;

    adjList = new int*[numVertices];
    weights = new int*[numVertices];
    sizes = new int[numVertices];

    for (int i = 0; i < numVertices; i++) {
        sizes[i] = other.sizes[i];
        if (sizes[i] > 0) {
            adjList[i] = new int[sizes[i]];
            weights[i] = new int[sizes[i]];
            for (int j = 0; j < sizes[i]; j++) {
                adjList[i][j] = other.adjList[i][j];
                weights[i][j] = other.weights[i][j];
            }
        } else {
            adjList[i] = nullptr;
            weights[i] = nullptr;
        }
    }

    return *this;
}

Graph::~Graph() {
    for (int i = 0; i < numVertices; i++) {
        if (sizes[i] > 0) {
            delete[] adjList[i];
            delete[] weights[i];
        }
    }
    delete[] adjList;
    delete[] weights;
    delete[] sizes;
}

// הוספת קשת לגרף
void Graph::addEdge(int src, int dest, int weight) {
    if (src < 0 || src >= numVertices || dest < 0 || dest >= numVertices) {
        throw "Invalid vertex!";
    }
    if (src == dest) {
        throw "Self-loops are not allowed!";
    }

    for (int i = 0; i < sizes[src]; i++) {
        if (adjList[src][i] == dest) {
            return;
        }
    }

    int newSize = sizes[src] + 1;
    int* newAdj = new int[newSize];
    int* newWeights = new int[newSize];

    for (int i = 0; i < sizes[src]; i++) {
        newAdj[i] = adjList[src][i];
        newWeights[i] = weights[src][i];
    }
    newAdj[sizes[src]] = dest;
    newWeights[sizes[src]] = weight;

    if (sizes[src] > 0) {
        delete[] adjList[src];
        delete[] weights[src];
    }
    adjList[src] = newAdj;
    weights[src] = newWeights;
    sizes[src] = newSize;

    newSize = sizes[dest] + 1;
    newAdj = new int[newSize];
    newWeights = new int[newSize];

    for (int i = 0; i < sizes[dest]; i++) {
        newAdj[i] = adjList[dest][i];
        newWeights[i] = weights[dest][i];
    }
    newAdj[sizes[dest]] = src;
    newWeights[sizes[dest]] = weight;

    if (sizes[dest] > 0) {
        delete[] adjList[dest];
        delete[] weights[dest];
    }
    adjList[dest] = newAdj;
    weights[dest] = newWeights;
    sizes[dest] = newSize;
}

// הסרת קשת מהגרף
void Graph::removeEdge(int src, int dest) {
    if (src < 0 || src >= numVertices || dest < 0 || dest >= numVertices) {
        throw "Invalid vertex!";
    }

    int index = -1;
    for (int i = 0; i < sizes[src]; i++) {
        if (adjList[src][i] == dest) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        throw "Edge does not exist!";
    }

    int newSize = sizes[src] - 1;
    int* newAdj = nullptr;
    int* newWeights = nullptr;
    if (newSize > 0) {
        newAdj = new int[newSize];
        newWeights = new int[newSize];
        for (int i = 0, j = 0; i < sizes[src]; i++) {
            if (i != index) {
                newAdj[j] = adjList[src][i];
                newWeights[j] = weights[src][i];
                j++;
            }
        }
    }

    if (sizes[src] > 0) {
        delete[] adjList[src];
        delete[] weights[src];
    }
    adjList[src] = newAdj;
    weights[src] = newWeights;
    sizes[src] = newSize;

    index = -1;
    for (int i = 0; i < sizes[dest]; i++) {
        if (adjList[dest][i] == src) {
            index = i;
            break;
        }
    }
    newSize = sizes[dest] - 1;
    newAdj = nullptr;
    newWeights = nullptr;
    if (newSize > 0) {
        newAdj = new int[newSize];
        newWeights = new int[newSize];
        for (int i = 0, j = 0; i < sizes[dest]; i++) {
            if (i != index) {
                newAdj[j] = adjList[dest][i];
                newWeights[j] = weights[dest][i];
                j++;
            }
        }
    }

    if (sizes[dest] > 0) {
        delete[] adjList[dest];
        delete[] weights[dest];
    }
    adjList[dest] = newAdj;
    weights[dest] = newWeights;
    sizes[dest] = newSize;
}

// הדפסת הגרף
void Graph::print_graph() {
    for (int i = 0; i < numVertices; i++) {
        std::cout << "Vertex " << i << ": {";
        for (int j = 0; j < sizes[i]; j++) {
            std::cout << adjList[i][j] << " (" << weights[i][j] << ")";
            if (j < sizes[i] - 1) {
                std::cout << ",  ";
            }
        }
        std::cout << "}" << std::endl;
    }
}

int Graph::getNumVertices() const {
    return numVertices;
}

int Graph::getSize(int v) const {
    if (v < 0 || v >= numVertices) {
        throw "Invalid vertex!";
    }
    return sizes[v];
}

int* Graph::getAdjList(int v) const {
    if (v < 0 || v >= numVertices) {
        throw "Invalid vertex!";
    }
    return adjList[v];
}

int* Graph::getWeights(int v) const {
    if (v < 0 || v >= numVertices) {
        throw "Invalid vertex!";
    }
    return weights[v];
}

} // סוף מרחב השמות graph
