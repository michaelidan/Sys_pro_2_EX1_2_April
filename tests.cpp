// כתובת מייל: michael9090124@gmail.com

// הגדרת doctest כך שיכלול את פונקציית main בקובץ זה
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Graph.h"
#include "Algorithms.h"
#include <vector>
#include <numeric> // עבור std::accumulate
#include <iostream> // נוסיף לצורך הדפסות אם נרצה לדבג בדיקות

// שימוש ב-namespace של הפרויקט
using namespace graph;

// ========= פונקציות עזר לבדיקות ==========

// פונקציית עזר לבדיקה אם קשת קיימת עם משקל מסוים
bool edgeExists(const Graph& g, int u, int v, int expected_weight) {
    bool found_uv = false;
    try {
        for (int i = 0; i < g.getSize(u); ++i) {
            if (g.getAdjList(u)[i] == v && g.getWeights(u)[i] == expected_weight) {
                found_uv = true;
                break;
            }
        }
    } catch (...) { /* התעלמות מחריגות בגישה אם u לא חוקי */ }

    bool found_vu = false;
     try {
        for (int i = 0; i < g.getSize(v); ++i) {
            if (g.getAdjList(v)[i] == u && g.getWeights(v)[i] == expected_weight) {
                found_vu = true;
                break;
            }
        }
     } catch (...) { /* התעלמות מחריגות בגישה אם v לא חוקי */ }
    return found_uv && found_vu;
}

// פונקציית עזר לחישוב משקל כולל של גרף (עבור בדיקות MST)
int getTotalWeight(const Graph& g) {
    int total_weight = 0;
    int n = g.getNumVertices();

    for (int u = 0; u < n; ++u) {
        try {
            for (int i = 0; i < g.getSize(u); ++i) {
                int v = g.getAdjList(u)[i];
                if (u < v) { // סופרים כל קשת רק פעם אחת
                   total_weight += g.getWeights(u)[i];
                }
            }
        } catch (...) { /* התעלמות מחריגות */ }
    }
    return total_weight;
}


// ========= בדיקות למחלקה Graph ==========

TEST_CASE("Graph Class Tests") {
    SUBCASE("Constructor") {
        Graph g0(0);
        CHECK(g0.getNumVertices() == 0);
        Graph g5(5);
        CHECK(g5.getNumVertices() == 5);
        // בדיקה שהבנאי זורק חריגה עבור מספר שלילי
        CHECK_THROWS_AS(Graph(-1), const char*);
    }

    SUBCASE("Add/Remove Edges") {
        Graph g(3);
        g.addEdge(0, 1, 10);
        CHECK(g.getSize(0) == 1);
        CHECK(g.getSize(1) == 1);
        CHECK(g.getSize(2) == 0);
        CHECK(edgeExists(g, 0, 1, 10) == true);

        g.addEdge(1, 2, 5);
        CHECK(g.getSize(0) == 1);
        CHECK(g.getSize(1) == 2);
        CHECK(g.getSize(2) == 1);
        CHECK(edgeExists(g, 1, 2, 5) == true);

        // בדיקה שהוספה כפולה לא משנה כלום
        int size_before = g.getSize(1);
        g.addEdge(1, 2, 5); // הוספה כפולה
        g.addEdge(2, 1, 5); // הוספה כפולה בכיוון ההפוך
        CHECK(g.getSize(1) == size_before);

        // בדיקה עם משקל ברירת מחדל (1)
        g.addEdge(0, 2);
        CHECK(edgeExists(g, 0, 2, 1) == true);

        // בדיקת הסרה
        g.removeEdge(1, 2); // הסרת הקשת
        CHECK(edgeExists(g, 1, 2, 5) == false);
        CHECK(g.getSize(1) == 1); // צומת 1 נשאר עם שכן 0
        CHECK(g.getSize(2) == 1); // צומת 2 נשאר עם שכן 0

        g.removeEdge(0, 1);
        CHECK(edgeExists(g, 0, 1, 10) == false);
        CHECK(g.getSize(0) == 1); // נשאר עם שכן 2
        CHECK(g.getSize(1) == 0); // נשאר ללא שכנים
    }

    SUBCASE("Error Handling") {
        Graph g(3);
        // הוספה עם צמתים לא חוקיים
        CHECK_THROWS_AS(g.addEdge(-1, 0, 1), const char*);
        CHECK_THROWS_AS(g.addEdge(0, 3, 1), const char*);
        // הוספת לולאה עצמית
        CHECK_THROWS_AS(g.addEdge(0, 0, 1), const char*);

        // הסרה עם צמתים לא חוקיים
        CHECK_THROWS_AS(g.removeEdge(-1, 0), const char*);
        CHECK_THROWS_AS(g.removeEdge(0, 3), const char*);
        // הסרת קשת לא קיימת
        g.addEdge(0, 1, 10);
        CHECK_THROWS_AS(g.removeEdge(0, 2), const char*); // קשת 0-2 לא קיימת

        // גישה עם צומת לא חוקי
        CHECK_THROWS_AS(g.getSize(-1), const char*);
        CHECK_THROWS_AS(g.getAdjList(3), const char*);
        CHECK_THROWS_AS(g.getWeights(3), const char*);
    }

    SUBCASE("Deep Copy") {
        Graph g_orig(3);
        g_orig.addEdge(0, 1, 10);

        // בדיקת בנאי העתקה
        Graph g_copy = g_orig;
        CHECK(edgeExists(g_copy, 0, 1, 10) == true);
        // שינוי העותק לא אמור להשפיע על המקור
        g_copy.addEdge(1, 2, 5);
        CHECK(edgeExists(g_orig, 1, 2, 5) == false); // ודא שהקשת לא נוספה למקור
        CHECK(g_orig.getSize(1) == 1);
        CHECK(g_copy.getSize(1) == 2);

        // בדיקת אופרטור השמה
        Graph g_assigned(2); // גרף אחר לגמרי
        g_assigned = g_orig;
        CHECK(edgeExists(g_assigned, 0, 1, 10) == true);
        CHECK(g_assigned.getNumVertices() == 3); // ודא שהגודל התעדכן
        // שינוי המקור לא אמור לשנות את המוקצה
        g_orig.addEdge(0, 2, 1);
        CHECK(edgeExists(g_assigned, 0, 2, 1) == false);
    }
}


// ========= בדיקות למחלקה Algorithms ==========

TEST_CASE("Algorithms Class Tests") {
    // הגדרת גרפים לדוגמה עבור הבדיקות
    Graph g0(0); // גרף ריק
    Graph g_disconnected(4); // גרף לא קשיר
    Graph g_neg_weights(3); // גרף עם משקל שלילי
    Graph g_main_example(5); // הגרף מהדוגמה ב-main

    // --- אתחול הגרפים --- (נעשה פעם אחת בתחילת ה-TEST_CASE)
    // g_disconnected: 0-1, 2-3
    g_disconnected.addEdge(0, 1, 1);
    g_disconnected.addEdge(2, 3, 2);
    // g_neg_weights: 0-1 (-1), 1-2 (2), 0-2 (5)
    g_neg_weights.addEdge(0, 1, -1);
    g_neg_weights.addEdge(1, 2, 2);
    g_neg_weights.addEdge(0, 2, 5);
    // g_main_example: הגרף מבדיקה 1 ב-main
    g_main_example.addEdge(0, 1, 1);
    g_main_example.addEdge(0, 2, 4);
    g_main_example.addEdge(1, 2, 2);
    g_main_example.addEdge(1, 3, 5);
    g_main_example.addEdge(2, 3, 1);
    g_main_example.addEdge(2, 4, 3);
    g_main_example.addEdge(3, 4, 2);
    // ----------------------


    SUBCASE("BFS Tests") {
        Algorithms alg(g_main_example);
        Graph bfs_tree = alg.bfs(0);
        CHECK(bfs_tree.getNumVertices() == 5);
        CHECK(edgeExists(bfs_tree, 0, 1, 1) == true);
        CHECK(edgeExists(bfs_tree, 0, 2, 4) == true);
        CHECK(edgeExists(bfs_tree, 1, 3, 5) == true);
        CHECK(edgeExists(bfs_tree, 2, 4, 3) == true);
        CHECK(edgeExists(bfs_tree, 1, 2, 2) == false);

        Algorithms alg0(g0);
        CHECK_THROWS_AS(alg0.bfs(0), const char*);
    }

    SUBCASE("DFS Tests") {
        Algorithms alg(g_main_example);
        Graph dfs_tree = alg.dfs(0);
        CHECK(dfs_tree.getNumVertices() == 5);
        CHECK(edgeExists(dfs_tree, 0, 1, 1) == true);
        CHECK(edgeExists(dfs_tree, 1, 2, 2) == true);
        CHECK(edgeExists(dfs_tree, 2, 3, 1) == true);
        CHECK(edgeExists(dfs_tree, 3, 4, 2) == true);
        CHECK(edgeExists(dfs_tree, 0, 2, 4) == false);

        Algorithms alg0(g0);
        CHECK_THROWS_AS(alg0.dfs(0), const char*);
    }

    SUBCASE("Dijkstra Tests") {
        Algorithms alg(g_main_example);
        Graph sp_tree = alg.dijkstra(0);
        CHECK(sp_tree.getNumVertices() == 5);
        CHECK(edgeExists(sp_tree, 0, 1, 1) == true);
        CHECK(edgeExists(sp_tree, 1, 2, 2) == true);
        CHECK(edgeExists(sp_tree, 2, 3, 1) == true);
        CHECK(edgeExists(sp_tree, 2, 4, 3) == true);
        CHECK(edgeExists(sp_tree, 0, 2, 4) == false);

        Algorithms alg_neg(g_neg_weights);
        CHECK_THROWS_AS(alg_neg.dijkstra(0), const char*);

        Algorithms alg0(g0);
        CHECK_THROWS_AS(alg0.dijkstra(0), const char*);
    }

    SUBCASE("Prim Tests") {
        Algorithms alg(g_main_example);
        Graph mst_prim = alg.prim();
        CHECK(mst_prim.getNumVertices() == 5);
        CHECK(getTotalWeight(mst_prim) == 6);

        Algorithms alg_disc(g_disconnected);
        Graph mst_disc = alg_disc.prim();
        CHECK(mst_disc.getNumVertices() == 4);
        CHECK(getTotalWeight(mst_disc) == 1); // רק רכיב קשירות אחד
        CHECK(edgeExists(mst_disc, 0, 1, 1) == true);
        CHECK(edgeExists(mst_disc, 2, 3, 2) == false); // לא אמור להגיע לרכיב השני

        Algorithms alg_neg(g_neg_weights);
        Graph mst_neg = alg_neg.prim();
        CHECK(mst_neg.getNumVertices() == 3);
        CHECK(getTotalWeight(mst_neg) == 1); // (-1) + 2 = 1

        Algorithms alg0(g0);
        Graph mst_empty = alg0.prim();
        CHECK(mst_empty.getNumVertices() == 0);
        CHECK(getTotalWeight(mst_empty) == 0);
    }

    SUBCASE("Kruskal Tests") {
        Algorithms alg(g_main_example);
        Graph mst_kruskal = alg.kruskal();
        CHECK(mst_kruskal.getNumVertices() == 5);
        CHECK(getTotalWeight(mst_kruskal) == 6);

        Algorithms alg_disc(g_disconnected);
        CHECK_THROWS_AS(alg_disc.kruskal(), const char*); // קרוסקל אמור לזרוק חריגה על גרף לא קשיר

        Algorithms alg_neg(g_neg_weights);
        Graph mst_neg = alg_neg.kruskal();
        CHECK(mst_neg.getNumVertices() == 3);
        CHECK(getTotalWeight(mst_neg) == 1);

        Algorithms alg0(g0);
        Graph mst_empty = alg0.kruskal(); // בהנחה שהתיקון מ-kruskal באמת עובד
        CHECK(mst_empty.getNumVertices() == 0);
        CHECK(getTotalWeight(mst_empty) == 0);
    }
}
