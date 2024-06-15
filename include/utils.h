#ifndef UTILS_H
#define UTILS_H

#include "class_vector.h"
#include "class_polygon.h"

#include <vector>
#include <string>

// saves a static svg file. The polygon vertices are supposed to be in the range [0..1], and a canvas of size 1000x1000 is created
void save_svg(const std::vector<Polygon> &polygons, const std::vector<Vector> &vertices, const std::string &filename, const std::string &fillcol = "none");
void save_svg(const std::vector<Polygon> &polygons, const std::string &filename, const std::string &fillcol = "none");
 
// Adds one frame of an animated svg file. frameid is the frame number (between 0 and nbframes-1).
// polygons is a list of polygons, describing the current frame.
// The polygon vertices are supposed to be in the range [0..1], and a canvas of size 1000x1000 is created
void save_svg_animated(const std::vector<Polygon> &polygons, const std::string &filename, int frameid, int nbframes);
void save_frame(const std::vector<Polygon> &cells, std::string filename, int frameid = 0);

double urand();

#endif