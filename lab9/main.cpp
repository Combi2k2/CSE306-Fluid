#include "include/class_edge.h"
#include "include/class_vector.h"
#include "include/class_mesh.h"

#include <unordered_map>
#include <unordered_set>

#include <vector>
#include <cmath>
#include <iostream>

#define PI          3.14159265359
#define NUM_ITER    5000


int main() {
    TriangleMesh goethe;
    goethe.readOBJ("model/goethe.obj");
    goethe.rescale();

    std::unordered_map<Edge, std::vector<int>> triangleE;
    std::unordered_map<int, std::vector<int>> triangleV;
    std::vector<bool> onBoundary(goethe.vertices.size(), false);

    for (int index = 0; index < goethe.indices.size(); ++index) {
        int vi = goethe.indices[index].vtxi;
        int vj = goethe.indices[index].vtxj;
        int vk = goethe.indices[index].vtxk;

        triangleE[Edge(vi, vj)].push_back(index);
        triangleE[Edge(vj, vk)].push_back(index);
        triangleE[Edge(vk, vi)].push_back(index);

        triangleV[vi].push_back(index);
        triangleV[vj].push_back(index);
        triangleV[vk].push_back(index);
    }

    std::vector<Edge> boundaryEdges;
    for (auto [edge, triangles] : triangleE)
        if (triangles.size() == 1) {
            boundaryEdges.push_back(edge);
            onBoundary[edge.u] = true;
            onBoundary[edge.v] = true;
        }

    std::vector<Edge> orderedEdges(boundaryEdges.size());
    orderedEdges[0] = boundaryEdges[0];

    for (int i = 1; i < boundaryEdges.size(); ++i)
    for (int j = 0; j < boundaryEdges.size(); ++j)
        if (orderedEdges[i - 1].v == boundaryEdges[j].u) {
            orderedEdges[i] = boundaryEdges[j];
            break;
        }

#pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0 ; i < orderedEdges.size() ; ++i) {
        double theta = i * 2 * M_PI / orderedEdges.size();
        goethe.vertices[orderedEdges[i].u] = Vector(cos(theta), sin(theta), 0);
    }

    for (int t = 0; t < NUM_ITER ; ++t) {
        std::vector<Vector> newVertices(goethe.vertices.size());
#pragma omp parallel for schedule(dynamic, 1)
        for (int i = 0 ; i < goethe.vertices.size() ; ++i) {
            if (onBoundary[i]) {
                newVertices[i] = goethe.vertices[i];
                continue;
            }
            Vector avg_neighbor(0, 0, 0);

            for (int j : triangleV[i]) {
                if (goethe.indices[j].vtxi != i)    avg_neighbor += goethe.vertices[goethe.indices[j].vtxi];
                if (goethe.indices[j].vtxj != i)    avg_neighbor += goethe.vertices[goethe.indices[j].vtxj];
                if (goethe.indices[j].vtxk != i)    avg_neighbor += goethe.vertices[goethe.indices[j].vtxk];
            }
            newVertices[i] = avg_neighbor / (2 * triangleV[i].size());
        }
        goethe.vertices.swap(newVertices);
    }
    goethe.writeOBJ("model/goethe_out.obj");
}