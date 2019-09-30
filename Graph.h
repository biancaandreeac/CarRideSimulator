// Copyright 2019 Bianca Ciuche
#ifndef GRAPH_H_
#define GRAPH_H_

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include "./Hashtable.h"

template <typename T>
struct vertex {
    std::vector<vertex<T>*> adj;  // destination vertex
    T name;
    Hashtable<T, int> dist;

    explicit vertex<T>(T s = "\0") : name(s) {}

    bool operator==(const vertex<T>& other) {
        if (this == &other) {
            return true;
        }
        return false;
    }
};

template <typename T>
class graph {
 public:
    Hashtable<T, vertex<T>*> vmap;
    ~graph();
    void add_vertex(const T& name);
    void add_edge(const T& from, const T& to);
    void remove_edge(const T& from, const T& to);
    bool has_edge(const T& from, const T& to);
    bool has_neighbors(const T v);
    std::vector<T> get_neighbors(const T v);
    void BFS(T src);
    int compute_distance(const T& from, const T& to);
    int get_distance(const T& from, const T& to);
    bool has_road(const T& from, const T& to);
    std::vector<T> roads_to(const T& to);
    std::vector<T> get_vertexs();
    bool has_dist(const T& s);
};

// Graph destructor
template <typename T>
graph<T>::~graph() {
    std::vector<T> l = vmap.get_keys();
    typename std::vector<T>::iterator i;
    for (i = l.begin(); i != l.end(); ++i) {
        auto j = vmap.get(*i);
        if (j) {
            delete j;
        }
    }
}

// add vertex to graph
template <typename T>
void graph<T>::add_vertex(const T &name) {
    if (!vmap.get(name)) {
        vertex<T> *v;
        v = new vertex<T>(name);
        vmap.put(name, v);
        return;
    }
}

// add edge to graph
template <typename T>
void graph<T>::add_edge(const T& from, const T& to) {
    if (has_edge(from, to) || from == to) {
        return;
    }
    vertex<T> *f = vmap.get(from);
    vertex<T> *t = vmap.get(to);
    f->adj.push_back(t);
}

// remove edge from graph
template <typename T>
void graph<T>::remove_edge(const T& from, const T& to) {
    vertex<T> *f = vmap.get(from);
    vertex<T> *t = vmap.get(to);
    typename std::vector<vertex<T>* >::iterator i;
    for (i = f->adj.begin(); i != f->adj.end(); ++i) {
        if (*i == t) {
            f->adj.erase(i);
            return;
        }
    }
}

// verifies if the graph has a specific edge
template <typename T>
bool graph<T>::has_edge(const T& from, const T& to) {
    vertex<T> *f = vmap.get(from);
    vertex<T> *t = vmap.get(to);
    typename std::vector<vertex<T>*>::iterator i;
    for (i = f->adj.begin(); i != f->adj.end(); ++i) {
        if (*i == t) {
            return true;
        }
    }
    return false;
}

// verifies if the vertex has neighbors
template <typename T>
bool graph<T>::has_neighbors(const T name) {
    vertex<T> *v = vmap.get(name);
    if (!v->adj.empty()) {
        return true;
    }
    return false;
}

// returns vertex's neighbors
template <typename T>
std::vector<T> graph<T>::get_neighbors(const T name) {
    vertex<T> *v = vmap.get(name);
    std::vector<T> vec;
    for (auto i = v->adj.begin(); i != v->adj.end(); ++i) {
        vec.push_back((*i)->name);
    }
    return vec;
}

// computes min distance to each vertex from a source vertex
template <typename T>
void graph<T>::BFS(T src) {
    vertex<T> *s = vmap.get(src);
    vertex<T> *sr;
    if (has_dist(src))
        s->dist.clear();

    // Create a queue for BFS
    std::list<vertex<T>*> queue;

    // Mark the current node as visited and enqueue it
    queue.push_back(s);
    s->dist.put(s->name, 0);

    // 'i' will be used to get all adjacent
    // vertices of a vertex

    while (!queue.empty()) {
        auto q = queue.front();
        queue.pop_front();
        auto d = s->dist.get(q->name);
        // Get all adjacent vertices of the dequeued vertex<T> s.
        // If a adjacent has not been visited, enqueue it
        for (auto i : q->adj) {
            sr = vmap.get(i->name);
            if (!s->dist.has_key(sr->name)) {
                s->dist.put(sr->name, d + 1);
                queue.push_back(sr);
            }
        }
    }
}

// computes distance between 2 vertex
template <typename T>
int graph<T>::compute_distance(const T& from, const T& to) {
    Hashtable<T, int> dist;
    if (from == to) {
        return 0;
    }

    vertex<T> *sr = vmap.get(from);

    // Create a queue for BFS
    std::list<vertex<T>*> queue;

    // Mark the current node as visited and enqueue it
    queue.push_back(sr);
    dist.put(sr->name, 0);

    // 'i' will be used to get all adjacent
    // vertices of a vertex

    while (!queue.empty()) {
        auto q = queue.front();
        queue.pop_front();
        auto d = dist.get(q->name) + 1;
        // Get all adjacent vertices of the dequeued vertex<T> s.
        // If a adjacent has not been visited, enqueue it
        for (auto i : q->adj) {
            if (i->name == to) {
                return d;
            }
            if (!dist.has_key(i->name)) {
                dist.put(i->name, d);
                queue.push_back(i);
            }
        }
    }
    return -1;
}

// verifies if the graph has a road between 2 vertex
template <typename T>
bool graph<T>::has_road(const T& from, const T& to) {
    if (from == to) {
        return true;
    }
    if (compute_distance(from, to) != -1) {
        return true;
    }

    return false;
}

// returns the distance without doing a BFS
template <typename T>
int graph<T>::get_distance(const T& from, const T& to) {
    if (from == to) {
        return 0;
    }

    vertex<T> *f = vmap.get(from);

    if (!f->dist.has_key(to)) {
        return -1;
    }
    return f->dist.get(to);
}

// verifies if the dist vector has been made
template <typename T>
bool graph<T>::has_dist(const T& s) {
    vertex<T> *f = vmap.get(s);
    if (f->dist.get_size() == 0) {
        return false;
    }
    return true;
}

#endif  // GRAPH_H_
