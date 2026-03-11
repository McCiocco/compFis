#ifndef BARNES_HUT_TREE_H
#define BARNES_HUT_TREE_H

#include <vector>

struct Quad {

    double minx, miny, maxx, maxy;
    double side;
    double mass;
    double cx, cy;
    unsigned nBodies;

    Quad(double minx_, double miny_, double side_);
    
    Quad(double side_) : Quad(-side_ / 2.0, -side_ / 2.0, side_) {}
    Quad() : Quad(0.0) {}

    bool contains(double x, double y) const;
    void addBody(double x, double y, double m);

};

struct BarnesHutTree {

    std::vector<Quad> tree;
    std::vector<unsigned> children;

    BarnesHutTree(double W, double H);

    void addBody(double x, double y, double m);
    void spawnChildren(unsigned index, Quad father);
    unsigned getContainingChildIndex(double x, double y, unsigned fatherIndex);

};

#endif