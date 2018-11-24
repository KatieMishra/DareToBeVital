// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: write comment header for this file; remove this comment

#include "trailblazer.h"
// TODO: include any other headers you need; remove this comment

using namespace std;

/**
 * @brief depthFirstSearch -- uses DFS to find first path from start to end vertex
 * @param graph -- world representing connections between vertices
 * @param start -- initial vertex
 * @param end -- ending vertex
 * @return -- path from start to end vertex
 */
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    Set<Vertex*> visited;
    depthFirstSearchHelper(graph, path, start, end, visited);
    return path;
}

bool depthFirstSearchHelper(BasicGraph& graph, Vector<Vertex*>& path, Vertex* v1, Vertex* v2, Set<Vertex*> visited) {
    visited.add(v1);
    v1->setColor(GREEN);
    path.add(v1);
    if (v1 == v2) {
        return true;
    } else {
        for (Edge* edge : v1->edges) {
            if (!visited.contains(edge->finish)) {
                return depthFirstSearchHelper(graph,path,edge->finish,v2,visited);
            }
        }
        v1->setColor(GRAY);
        path.remove(path.size()-1);
    }
    return false;
}

/**
 * @brief breadthFirstSearch
 * @param graph
 * @param start
 * @param end
 * @return
 */
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    // TODO: implement this function; remove these comments
    //       (The function body code provided below is just a stub that returns
    //        an empty vector so that the overall project will compile.
    //        You should remove that code and replace it with your implementation.)
    Vector<Vertex*> path;
    return path;
}

/**
 * @brief dijkstrasAlgorithm
 * @param graph
 * @param start
 * @param end
 * @return
 */
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    // TODO: implement this function; remove these comments
    //       (The function body code provided below is just a stub that returns
    //        an empty vector so that the overall project will compile.
    //        You should remove that code and replace it with your implementation.)
    Vector<Vertex*> path;
    return path;
}

/**
 * @brief aStar
 * @param graph
 * @param start
 * @param end
 * @return
 */
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    // TODO: implement this function; remove these comments
    //       (The function body code provided below is just a stub that returns
    //        an empty vector so that the overall project will compile.
    //        You should remove that code and replace it with your implementation.)
    Vector<Vertex*> path;
    return path;
}

/**
 * @brief alternatePath
 * @param graph
 * @param start
 * @param end
 * @param difference
 * @return
 */
Vector<Vertex*> alternatePath(BasicGraph& graph, Vertex* start, Vertex* end, double difference) {
    // TODO: implement this function; remove these comments
    //       (The function body code provided below is just a stub that returns
    //        an empty vector so that the overall project will compile.
    //        You should remove that code and replace it with your implementation.)
    Vector<Vertex*> path;
    return path;
}

/**
 * @brief kruskal
 * @param graph
 * @return
 */
Set<Edge*> kruskal(BasicGraph& graph) {
    // TODO: implement this function; remove these comments
    //       (The function body code provided below is just a stub that returns
    //        an empty set so that the overall project will compile.
    //        You should remove that code and replace it with your implementation.)
    Set<Edge*> mst;
    return mst;
}