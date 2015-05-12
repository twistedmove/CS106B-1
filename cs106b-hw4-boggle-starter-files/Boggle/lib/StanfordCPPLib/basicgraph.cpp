/*
 * This file contains the implementation of some useful graph types,
 * specifically the Node and Arc structures used in the typical graph.
 * together in lecture.  We also implement BasicGraph, an instantiation of
 * Stanford's Graph class using Node and Arc as its type parameters.
 *
 * See BasicGraph.h for documentation of each member.
 *
 * Author: Marty Stepp
 * Version: 2014/01/23
 */

#include <sstream>
#include "basicgraph.h"

/*
 * Node member implementations
 */

Grid<double>* Node::s_world = NULL;
double (* Node::s_heuristicFunction)(Node* const from, Node* const to, const Grid<double>& world) = NULL;

Node::Node(string name, int row, int col, double gridValue) {
    this->name = name;
    this->m_row = row;
    this->m_col = col;
    this->m_gridValue = gridValue;
    this->resetData();
}

int Node::getColor() const {
    return this->m_color;
}

double Node::heuristic(Node* const other) const {
    if (s_world == NULL || s_heuristicFunction == NULL) {
        return 0.0;
    } else {
        return s_heuristicFunction(const_cast<Node*>(this), const_cast<Node*>(other), *s_world);
    }
}

void Node::resetData() {
    this->cost = 0.0;
    this->previous = NULL;
    this->visited = false;
    // this->m_color = WHITE;
}

void Node::setColor(int c) {
    this->m_color = c;
    // if (s_world != NULL && guiIsInitialized()) {
    //     colorCell(*s_world, makeLoc(this->m_row, this->m_col), c);
    // }
}

void Node::setHeuristicFunction(double (* func)(Node* const from, Node* const to, const Grid<double>& world)) {
    s_heuristicFunction = func;
}

void Node::setWorld(const Grid<double>& world) {
    s_world = const_cast<Grid<double>*>(&world);
}

string Node::toString() const {
    ostringstream out;
    out << *this;
    return out.str();
}

ostream& operator<<(ostream& out, const Node& node) {
    out << "Node{name=" << node.name;
    if (node.cost != 0.0) {
        out << ", cost=" << node.cost;
    }
    out << ", cost=" << node.cost;
    out << ", visited=" << (node.visited ? "true" : "false");
    out << ", previous=" << (node.previous == NULL ? string("NULL") : node.previous->name);

    out << ", neighbors={" << node.arcs;
    int i = 0;
    __foreach__ (Arc* arc __in__ node.arcs) {
        if (i > 0) {
            out << ", ";
        }
        if (arc->finish) {
            out << arc->finish->name;
        } else {
            out << "NULL";
        }
    }
    out << "}";
    out << "}";
    return out;
}


/*
 * Arc member implementations
 */

Arc::Arc(Node* start, Node* finish, double cost) {
    this->start = start;
    this->finish = finish;
    this->cost = cost;
    this->resetData();
}

void Arc::resetData() {
    this->visited = false;
}

string Arc::toString() const {
    ostringstream out;
    out << *this;
    return out.str();
}

ostream& operator<<(ostream& out, const Arc& arc) {
    out << "Arc{start=" << arc.start->name << ", finish=" << arc.finish->name;
    if (arc.cost != 0.0) {
        out << ", cost=" << arc.cost;
    }
    if (arc.visited) {
        out << ", visited=" << (arc.visited ? "true" : "false");
    }
    out << "}";
    return out;
}


/*
 * BasicGraph member implementations
 */

BasicGraph::BasicGraph() : Graph<Node, Arc>() {
    m_resetEnabled = true;
}

bool BasicGraph::containsArc(Node* v1, Node* v2) const {
    return this->getArc(v1, v2) != NULL;
}

bool BasicGraph::containsArc(string v1, string v2) const {
    return this->getArc(v1, v2) != NULL;
}

bool BasicGraph::containsArc(Arc* arc) const {
    if (arc == NULL) {
        return false;
    } else {
        return this->getArcSet().contains(arc);
    }
}

bool BasicGraph::containsEdge(Node* v1, Node* v2) const {
    return this->containsArc(v1, v2);
}

bool BasicGraph::containsEdge(string v1, string v2) const {
    return this->containsArc(v1, v2);
}

bool BasicGraph::containsEdge(Arc* arc) const {
    return this->containsArc(arc);
}

bool BasicGraph::containsNode(string name) const {
    return this->getNode(name) != NULL;
}

bool BasicGraph::containsNode(Node* v) const {
    if (v == NULL) {
        return false;
    } else {
        return this->getNodeSet().contains(v);
    }
}

bool BasicGraph::containsVertex(string name) const {
    return this->containsNode(name);
}

bool BasicGraph::containsVertex(Node* v) const {
    return this->containsNode(v);
}

Arc* BasicGraph::getArc(Node* v1, Node* v2) const {
    __foreach__ (Arc* arc __in__ this->getArcSet(v1)) {
        if (arc->finish == v2) {
            return arc;
        }
    }
    return NULL;
}

Arc* BasicGraph::getArc(string v1, string v2) const {
    return this->getArc(this->getVertex(v1), this->getVertex(v2));
}

Arc* BasicGraph::getEdge(Node* v1, Node* v2) const {
    return this->getArc(v1, v2);
}

Arc* BasicGraph::getEdge(string v1, string v2) const {
    return this->getArc(v1, v2);
}

Arc* BasicGraph::getInverseArc(Arc* arc) const {
    return this->getArc(arc->finish, arc->start);
}

Arc* BasicGraph::getInverseEdge(Arc* arc) const {
    return this->getInverseArc(arc);
}

bool BasicGraph::isNeighbor(string v1, string v2) const {
    return this->isNeighbor(this->getVertex(v1), this->getVertex(v2));
}

bool BasicGraph::isNeighbor(Node* v1, Node* v2) const {
    __foreach__ (Arc* edge __in__ this->getEdgeSet(v1)) {
        if (edge->finish == v2) {
            return true;
        }
    }
    return false;
}

void BasicGraph::resetData() {
    if (m_resetEnabled) {
        __foreach__ (Node* node __in__ getNodeSet()) {
            node->resetData();
        }
        __foreach__ (Arc* arc __in__ getArcSet()) {
            arc->resetData();
        }
    }
}

void BasicGraph::setResetEnabled(bool enabled) {
    m_resetEnabled = enabled;
}

// members below are just mirrors of ones from Graph

Arc* BasicGraph::addEdge(string v1, string v2, double cost, bool directed) {
    return this->addEdge(getVertex(v1), getVertex(v2), cost, directed);
}

Arc* BasicGraph::addEdge(Node* v1, Node* v2, double cost, bool directed) {
    Arc* e = new Arc(v1, v2, cost);
    return addEdge(e, directed);
}

Arc* BasicGraph::addEdge(Arc* e, bool directed) {
    Arc* result = this->addArc(e);
    if (!directed) {
        Arc* result2 = this->addArc(e->finish, e->start);
        result2->cost = e->cost;
    }
    return result;
}

Node* BasicGraph::addVertex(string name) {
    return this->addNode(name);
}

Node* BasicGraph::addVertex(Node* v) {
    return this->addNode(v);
}

const Set<Arc*>& BasicGraph::getEdgeSet() const {
    return this->getArcSet();
}

const Set<Arc*>& BasicGraph::getEdgeSet(Node* v) const {
    return this->getArcSet(v);
}

const Set<Arc*>& BasicGraph::getEdgeSet(string v) const {
    return this->getArcSet(v);
}

Node* BasicGraph::getVertex(string name) const {
    return this->getNode(name);
}

const Set<Node*>& BasicGraph::getVertexSet() const {
    return this->getNodeSet();
}

void BasicGraph::removeEdge(string v1, string v2, bool directed) {
    this->removeEdge(this->getVertex(v1), this->getVertex(v2), directed);
}

void BasicGraph::removeEdge(Node* v1, Node* v2, bool directed) {
    this->removeArc(v1, v2);
    if (!directed) {
        this->removeArc(v2, v1);
    }
}

void BasicGraph::removeEdge(Arc* e, bool directed) {
    this->removeArc(e);
    if (!directed) {
        this->removeArc(e->finish, e->start);
    }
}

void BasicGraph::removeVertex(string name) {
    this->removeNode(name);
}

void BasicGraph::removeVertex(Node* v) {
    this->removeNode(v);
}

void BasicGraph::scanArcData(TokenScanner& scanner, Edge* edge, Edge* inverse) {
    string colon = scanner.nextToken();   // ":", skip over
    if (colon == ":") {
        string costStr = scanner.nextToken();
        edge->cost = stringToReal(costStr);
        if (inverse != NULL) {
            inverse->cost = edge->cost;
        }
    } else {
        // no cost for this edge (cost 0); un-read the colon token because
        // it probably wasn't actually a colon
        scanner.saveToken(colon);
    }
}

void BasicGraph::writeArcData(ostream& out, Edge* edge) const {
    if (edge->cost != 0) {
        out << " : ";
        out << edge->cost;
    }
}
