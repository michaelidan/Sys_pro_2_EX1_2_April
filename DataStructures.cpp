// michael9090124@gmail.com

#include "DataStructures.h"
#include <iostream>

namespace graph {

// --- Queue ---

Queue::Queue(int cap) {
    capacity = cap;      // קיבולת התחלתית של התור
    array = new int[cap]; // הקצאת מערך דינמי
    front = 0;           // הראש מתחיל ב-0
    rear = -1;           // הסוף מתחיל לפני 0
    size = 0;            // התור ריק בתחילה
}

Queue::~Queue() {
    delete[] array;      // שחרור הזיכרון של המערך
}

void Queue::enqueue(int x) {
    if (size == capacity) { // אם התור מלא
        int* newArray = new int[capacity * 2]; // הכפלת הקיבולת
        for (int i = 0; i < size; i++) {
            newArray[i] = array[(front + i) % capacity]; // העתקה למערך חדש
        }
        delete[] array;      // שחרור המערך הישן
        array = newArray;    // עדכון המצביע
        front = 0;           // איפוס הראש
        rear = size - 1;     // עדכון הסוף
        capacity *= 2;       // עדכון הקיבולת
    }
    rear = (rear + 1) % capacity; // עדכון הסוף במעגליות
    array[rear] = x;     // הוספת האיבר
    size++;              // הגדלת מספר האיברים
}

int Queue::dequeue() {
    if (isEmpty()) {
        throw "התור ריק!";
    }
    int item = array[front]; // שמירת האיבר מהראש
    front = (front + 1) % capacity; // עדכון הראש במעגליות
    size--;              // הקטנת מספר האיברים
    return item;         // החזרת האיבר שהוסר
}

bool Queue::isEmpty() {
    return size == 0;    // מחזיר אמת אם התור ריק
}

// --- PriorityQueue ---

PriorityQueue::PriorityQueue(int cap) {
    capacity = cap;      // קיבולת התחלתית
    array = new Item[cap]; // הקצאת מערך דינמי של איברים
    size = 0;            // התור ריק בתחילה
}

PriorityQueue::~PriorityQueue() {
    delete[] array;      // שחרור הזיכרון של המערך
}

void PriorityQueue::insert(int v, int p) {
    if (size == capacity) { // אם התור מלא
        Item* newArray = new Item[capacity * 2]; // הכפלת הקיבולת
        for (int i = 0; i < size; i++) {
            newArray[i] = array[i]; // העתקה למערך חדש
        }
        delete[] array;      // שחרור המערך הישן
        array = newArray;    // עדכון המצביע
        capacity *= 2;       // עדכון הקיבולת
    }
    array[size].vertex = v;  // הוספת הקודקוד
    array[size].priority = p; // הוספת העדיפות
    size++;              // הגדלת מספר האיברים
}

PriorityQueue::Item PriorityQueue::extractMin() {
    if (isEmpty()) {
        throw "תור העדיפויות ריק!";
    }
    int minIndex = 0;    // אינדקס המינימום, מתחיל ב-0
    for (int i = 1; i < size; i++) { // חיפוש המינימום
        if (array[i].priority < array[minIndex].priority) {
            minIndex = i;
        }
    }
    Item minItem = array[minIndex]; // שמירת האיבר המינימלי
    // העתקת האיבר האחרון למיקום המינימום
    array[minIndex] = array[size - 1];
    size--;              // הקטנת הגודל
    return minItem;      // החזרת האיבר המינימלי
}

bool PriorityQueue::isEmpty() {
    return size == 0;    // מחזיר אמת אם התור ריק
}

// --- UnionFind ---

UnionFind::UnionFind(int n) {
    size = n;            // מספר הקודקודים
    parent = new int[n];  // הקצאת מערך ההורים
    rank = new int[n];   // הקצאת מערך הדרגות
    makeSet();           // אתחול הקבוצות
}

UnionFind::~UnionFind() {
    delete[] parent;     // שחרור מערך ההורים
    delete[] rank;       // שחרור מערך הדרגות
}

void UnionFind::makeSet() {
    for (int i = 0; i < size; i++) {
        parent[i] = i;   // כל קודקוד הוא השורש של עצמו
        rank[i] = 0;     // גובה העץ מתחיל ב-0
    }
}

int UnionFind::find(int x) {
    if (parent[x] != x) { // אם x אינו השורש
        parent[x] = find(parent[x]); // דחיסת נתיב: חיבור ישיר לשורש
    }
    return parent[x];    // החזרת השורש
}

void UnionFind::unionSets(int x, int y) {
    int rootX = find(x); // מציאת השורש של x
    int rootY = find(y); // מציאת השורש של y
    if (rootX == rootY) { // אם הם כבר באותה קבוצה
        return;
    }
    // איחוד לפי דרגה
    if (rank[rootX] < rank[rootY]) {
        parent[rootX] = rootY; // חיבור העץ הקטן לגדול
    } else if (rank[rootX] > rank[rootY]) {
        parent[rootY] = rootX; // חיבור העץ הקטן לגדול
    } else {
        parent[rootY] = rootX; // גבהים שווים, חיבור וגידול דרגה
        rank[rootX]++;
    }
}

} // סוף מרחב השמות graph
