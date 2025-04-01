// michael9090124@gmail.com

#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

namespace graph {

// מחלקה לתור פשוט
class Queue {
private:
    int* array;    // מערך דינמי לאחסון התור
    int capacity;  // גודל המערך
    int front;     // אינדקס הראש של התור
    int rear;      // אינדקס הסוף של התור
    int size;      // מספר האיברים כרגע בתור

public:
    Queue(int cap);       // בנאי שמקבל קיבולת התחלתית
    ~Queue();             // מפרק לשחרור זיכרון
    void enqueue(int x);  // מוסיף איבר לסוף התור
    int dequeue();        // מוציא איבר מהראש של התור
    bool isEmpty();       // בודק אם התור ריק
};

// מחלקה לתור עדיפויות (מינימום)
class PriorityQueue {
public:
    struct Item {      // מבנה לאיבר בתור עדיפויות
        int vertex;    // קודקוד
        int priority;  // עדיפות (משקל)
    };

private:
    Item* array;       // מערך דינמי של איברים
    int capacity;      // גודל המערך
    int size;          // מספר האיברים כרגע

public:
    PriorityQueue(int cap);     // בנאי שמקבל קיבולת התחלתית
    ~PriorityQueue();           // מפרק לשחרור זיכרון
    void insert(int v, int p);  // מוסיף איבר עם עדיפות
    Item extractMin();          // מוציא את האיבר עם העדיפות הכי נמוכה
    bool isEmpty();             // בודק אם התור ריק
};

// מחלקה ל-Union-Find
class UnionFind {
private:
    int* parent;   // מערך של הורים
    int* rank;     // מערך של דרגות (לשמור על עץ שטוח)
    int size;      // מספר האיברים

public:
    UnionFind(int n);      // בנאי שמאתחל את המבנה
    ~UnionFind();          // מפרק לשחרור זיכרון
    void makeSet();        // יוצר קבוצות נפרדות לכל איבר
    int find(int x);       // מוצא את השורש של קבוצה
    void unionSets(int x, int y); // מאחד שתי קבוצות
};

} // סוף מרחב השמות graph

#endif
