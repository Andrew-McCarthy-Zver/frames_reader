#ifndef GRAPH_H
#define GRAPH_H

#include <QString>
#include "vertex.h"
#include "edge.h"
#include <QList>
class Graph
{
public:
    Graph();
    int** get(int **Matrix ) const;
    QList<Vertex> getVertexes() const;
    QList<Edge> getEdges() const;
    void addVertex(Vertex vertex);
    void addEdge(Edge edge);
    int countVertex()const;
    void countweightedge(Edge edge);
    QString getNameVertex(int num) const;
private:
    QList<Vertex> Vertexes;
    QList<Edge> Edges;
};

#endif // GRAPH_H
