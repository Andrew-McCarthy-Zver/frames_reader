#ifndef EDGE_H
#define EDGE_H
#include <QString>
#include "vertex.h"

class Edge
{
public:
    Edge(Vertex,Vertex,int);
    Vertex getFrom() const;
    Vertex getTo() const;
    int getWeight() const;
    void counterweight();
private:
    Vertex From = Vertex(0,"");
    Vertex To = Vertex(0,"");
    int Weight;
};

#endif // EDGE_H
