#include "graph.h"

Graph::Graph()
{

}

int** Graph::get(int **Matrix ) const {

    int** matrix = new int*[Vertexes.count()];
    for(int k = 0; k < Vertexes.count(); k++)
    {
        matrix[k] = new int[Vertexes.count()];
        for (int j = 0; j < Vertexes.count(); j++)
            matrix[k][j] = 0;
    }
    for (auto edge : Edges)
    {
        int row = edge.getFrom().Number-1;
        int column = edge.getTo().Number-1;
        matrix[row][column] = edge.getWeight();
    }
    Matrix = matrix;
    return Matrix;
}
void Graph:: addVertex(Vertex vertex) {
    Vertexes.append(vertex);
}
void Graph:: addEdge(Edge edge) {
    Edges.append(edge);
}
 QList<Vertex> Graph:: getVertexes() const {
    return Vertexes;
}
 int  Graph:: countVertex() const {
     return Vertexes.count();
 }
 QList<Edge> Graph:: getEdges() const {
    return Edges;
}
QString Graph::getNameVertex(int num) const{
    foreach (Vertex v, Vertexes)
    {
        if (v.Number == num) {return v.Address;}
    }
    return "No Address";
}

 void Graph:: countweightedge(Edge edge) {
     int m = 0;
     foreach(Edge e, Edges)
     {

         if (e.getFrom().Address == edge.getFrom().Address && e.getTo().Address == edge.getTo().Address  && e.getWeight() == edge.getWeight())
         {
             Edges.removeAt(m);
             e.counterweight();
             Edges.append(e);
         }
         m++;
     }
 }
