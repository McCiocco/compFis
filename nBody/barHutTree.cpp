#include "barHutTree.h"

//IMPLEMENTAZIONE QUAD

Quad::Quad(const double minx_, const double miny_, const double side_) : 
    minx(minx_), miny(miny_), side(side_),
    maxx(minx_ + side_), maxy(miny_ + side_),
    mass(0), cx(0), cy(0), nBodies(0) {}

bool Quad::contains(const double x, const double y) const {
    
    return (x >= minx) && (x <= maxx) && (y >= miny) && (y <= maxy);

}

void Quad::addBody(const double x, const double y, const double m) {

    cx = (cx * nBodies + m * x) / (nBodies + 1);
    cy = (cy * nBodies + m * y) / (nBodies + 1);
    mass += m;
    nBodies++;

}

//IMPLEMENTAZIONE BHTREE

BarnesHutTree::BarnesHutTree(const double W, const double H) {
    
    const double side = std::max(W, H);
    tree.push_back(Quad(side));
    for (int i = 0; i < 4; i++) children.push_back(0);

}

void BarnesHutTree::addBody(const double x, const double y, const double m) {
    
    unsigned currIndex = 0;
    bool over = false;
    Quad currQuad;

    while (!over) {
    
        currQuad = tree[currIndex];
        if (currQuad.nBodies == 0) {

            over = true;
        
        } else if (currQuad.nBodies == 1) {
          
            spawnChildren(currIndex, currQuad);
            
            unsigned quadIdOldBody = getContainingChildIndex(currQuad.cx, currQuad.cy, currIndex);
            tree[quadIdOldBody].addBody(currQuad.cx, currQuad.cy, currQuad.mass);

            currIndex = getContainingChildIndex(x, y, currIndex);

        } else {

            currIndex = getContainingChildIndex(x, y, currIndex);

        }

        currQuad.addBody(x, y, m);

    }

}

void BarnesHutTree::spawnChildren(const unsigned index, const Quad father) {

    //adds the children in the order: top left, top right, bot left, bot right

    for (int i = 0; i < 4; i++) {

        double newMinX = father.minx + (i % 2) * father.side;
        double newMinY = father.miny + (1 - i / 2) * father.side;

        tree.push_back(
            Quad(newMinX, newMinY, father.side / 2));
        children[4*index] = tree.size() - 1;

    }

    for (int i = 0; i < 16; i++) children.push_back(0);    

}

unsigned BarnesHutTree::getContainingChildIndex(const double x, const double y, const unsigned fatherIndex) {

    for (unsigned i = 0; i < 4; i++) {

        if (tree[children[4*fatherIndex + i]].contains(x, y)) return 4 * fatherIndex + i;

    }

}