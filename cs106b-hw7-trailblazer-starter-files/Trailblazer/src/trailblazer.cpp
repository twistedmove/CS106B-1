// This file contains functions which carry out different kinds of searches for paths in a graph.
#include "costs.h"
#include "trailblazer.h"
#include "stack.h"
#include "queue.h"
#include "pqueue.h"
#include "map.h"

using namespace std;

Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end);
bool depthFirstSearchHelper(Vertex* start, Vertex* end, Vector<Vertex*> &path);
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end);
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end);
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end);
void ufs_init(Map<Vertex*, Vertex*> &parent, const Set<Vertex *> &vertices);
Vertex* ufs_find(Map<Vertex*, Vertex*> &parent, Vertex* v);
void ufs_union(Map<Vertex*, Vertex*> &parent, Vertex* v1, Vertex* v2);
Set<Edge*> kruskal(BasicGraph& graph);

Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData(); //wipe data from previous search
    Vector<Vertex*> path;
    depthFirstSearchHelper(start, end, path);
    return path;
}

bool depthFirstSearchHelper(Vertex* start, Vertex* end, Vector<Vertex*> &path) {
    if (start->visited)
        return false;
    start->setColor(GREEN);
    start->visited = true;

    path.add(start);

    // If we reached the destination, return
    if (start == end)
        return true;

    for (Edge* e: start->edges) {
        if (depthFirstSearchHelper(e->finish, end, path))
            return true; // If an adjacent vertex can reach the destination, then we have found a path, return
    }

    // If we cannot reach the destination by coming to this current vertex,
    // clear things up and return false
    path.remove(path.size() - 1);
    start->setColor(GRAY);
    return false;
}

Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();

    Queue<Vertex*> q;
    q.enqueue(start);
    start->setColor(YELLOW);
    start->previous = NULL;
    start->visited = true;

    while (!q.isEmpty()) {
        Vertex* front = q.dequeue();
        front->setColor(GREEN);

        if (front == end) {
            // We have reached the destination, now we need to reconstruct the path
            // by tracing the previous field of each vertex from end back to start
            // We use a stack to reverse the list
            Stack<Vertex*> s;
            Vertex* tmp = end;
            while (tmp->previous != NULL) {
                s.push(tmp);
                tmp = tmp->previous;
            }

            Vector<Vertex*> path;
            path.add(start);
            while (!s.isEmpty())
                path.add(s.pop());
            return path;
        }

        for (Edge* e: front->edges) {
            if (!e->finish->visited) {
                q.enqueue(e->finish);
                e->finish->setColor(YELLOW);
                e->finish->visited = true;
                e->finish->previous = front;
            }
        }
    }

    // If code reaches here, then there does not exist a path from start to end, thus return an empty vector
    return Vector<Vertex*>();
}

Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();

    PriorityQueue<Vertex*> q;
    for (Vertex* v: graph.getVertexSet())
        v->cost = POSITIVE_INFINITY;//initialize every node to positive infinity
    start->cost = 0;

    q.enqueue(start, 0);
    start->visited = true;
    start->previous = NULL;
    start->setColor(YELLOW);

    while (!q.isEmpty()) {
        Vertex* front = q.dequeue();
        front->setColor(GREEN);

        if (front == end) {
            // We have reached the destination, now we need to reconstruct the path
            // by tracing the previous field of each vertex from end back to start
            // We use a stack to reverse the list
            Stack<Vertex*> s;
            Vertex* tmp = end;
            while (tmp->previous != NULL) {
                s.push(tmp);
                tmp = tmp->previous;
            }

            Vector<Vertex*> path;
            path.add(start);
            while (!s.isEmpty())
                path.add(s.pop());
            return path;
        }

        for (Edge* e: front->edges) {
            Vertex* v = e->finish;
            double newCost = front->cost + e->cost;
            if (newCost < v->cost) {
                if (!v->visited) {
                    v->visited = true;
                    q.enqueue(v, newCost);
                } else
                    q.changePriority(v, newCost);
                v->cost = newCost;
                v->previous = front;//update cost and path
            }
        }
    }
    return Vector<Vertex*>();
}

Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();

    PriorityQueue<Vertex*> q;
    for (Vertex* v: graph.getVertexSet())
        v->cost = POSITIVE_INFINITY;
    start->cost = 0;

    q.enqueue(start, heuristicFunction(start, end));
    start->visited = true;
    start->previous = NULL;
    start->setColor(YELLOW);

    while (!q.isEmpty()) {
        Vertex* front = q.dequeue();
        front->setColor(GREEN);

        if (front == end) {
            // We have reached the destination, now we need to reconstruct the path
            // by tracing the previous field of each vertex from end back to start
            // We use a stack to reverse the list
            Stack<Vertex*> s;
            Vertex* tmp = end;
            while (tmp->previous != NULL) {
                s.push(tmp);
                tmp = tmp->previous;
            }

            Vector<Vertex*> path;
            path.add(start);
            while (!s.isEmpty())
                path.add(s.pop());
            return path;
        }

        for (Edge* e: front->edges) {
            Vertex* v = e->finish;
            double newCost = front->cost + e->cost;
            if (newCost < v->cost) {
                double h = heuristicFunction(v, end); //weight + heuristic
                if (!v->visited) {
                    v->visited = true;
                    q.enqueue(v, newCost + h);
                } else
                    q.changePriority(v, newCost + h);
                v->cost = newCost;
                v->previous = front;
            }
        }
    }
    return Vector<Vertex*>();
}

void ufs_init(Map<Vertex*, Vertex*> &parent, const Set<Vertex* > &vertices) {
    for (Vertex* v: vertices)
        parent.put(v, NULL);
}

Vertex* ufs_find(Map<Vertex*, Vertex*> &parent, Vertex* v) {
    if (parent.get(v) == NULL)
        return v;
    Vertex* t = ufs_find(parent, parent.get(v));
    parent.put(v, t);
    return t;
}

void ufs_union(Map<Vertex*, Vertex*> &parent, Vertex* v1, Vertex* v2) {
    Vertex* p1 = ufs_find(parent, v1);
    Vertex* p2 = ufs_find(parent, v2);
    if (p1 != p2)
        parent.put(p1, p2);  //merge clusters, reference from Stack Overflow
}

Set<Edge*> kruskal(BasicGraph& graph) {
    PriorityQueue<Edge*> q;
    for (Edge* e: graph.getEdgeSet())
        q.enqueue(e, e->cost);

    Map<Vertex*, Vertex*> parent;
    ufs_init(parent, graph.getVertexSet());

    int n = graph.getVertexSet().size();
    Set<Edge*> mst;
    while (!q.isEmpty() && mst.size() < n - 1) {
        Edge* e = q.dequeue();
        if (ufs_find(parent, e->start) != ufs_find(parent, e->finish)) {
            mst.add(e);
            ufs_union(parent, e->start, e->finish);
        }
    }

    return mst;
}
