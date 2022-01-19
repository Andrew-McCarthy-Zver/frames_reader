#include "edge.h"
#include "vertex.h"
Edge::Edge(Vertex fr,Vertex to,int weight)
{
    From = fr;
    To = to;
    Weight = weight;
}
Vertex Edge::getFrom() const {

    return From;
}
Vertex Edge::getTo() const {

    return To;
}
int Edge::getWeight() const {

    return Weight;
}
void Edge::counterweight() {
    Weight++;
}
