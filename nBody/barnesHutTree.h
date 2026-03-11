#include <vector>

struct Quad {

    double minx, miny, maxx, maxy;
    double side;
    double mass;
    double cx, cy;
    unsigned nBodies;

    Quad(const double minx_, const double miny_, const double side_): 
        minx(minx_), miny(miny_), side(side_),
        maxx(minx_ + side), maxy(miny_ + side),
        mass(0), cx(0), cy(0), nBodies(0) {}

    Quad(const double side_) {

        Quad(-side/2, -side/2, side);

    }

    Quad() {

        Quad(0);

    }
        
    bool contains(const double x, const double y) const {

        return (x >= minx) && (x <= maxx) && (y >= miny) && (y <= maxy);

    }

    void addBody(const double x, const double y, const double m) {

        mass += m;
        cx = (cx * nBodies + m * x) / (nBodies + 1);
        cy = (cy * nBodies + m * y) / (nBodies + 1);
        nBodies++;

    }

};

struct BarnesHutTree {

    std::vector<Quad> tree;
    std::vector<unsigned> children;

    //tree stores the quads composing the tree
    //children stores the 4 children of tree at index i at the indices 4*i, 4*i+1, ecc

    BarnesHutTree(const double W, const double H) {

        const double side = std::max(W, H);
        tree.push_back(Quad(side));
        for (int i = 0; i < 4; i++) children.push_back(0);

    }

    void addBody(const double x, const double y, const double m) {

        unsigned currIndex = 0;
        Quad currQuad;
        bool over = false;
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

    void spawnChildren(const unsigned index, const Quad father) {

        //adds the children in the order: top left, top right, bot left, bot right

        for (int i = 0; i < 4; i++) {

            tree.push_back(
                Quad(father.minx + (i % 2) * father.side, father.miny + (1 - i / 2) * father.side, father.side / 2));
            children[4*index] = tree.size() - 1;

        }

        for (int i = 0; i < 16; i++) children.push_back(0);

    }

    unsigned getContainingChildIndex(const double x, const double y, const unsigned fatherIndex) {

        for (unsigned i = 0; i < 4; i++) {

            if (tree[children[4*fatherIndex + i]].contains(x, y)) return 4 * fatherIndex + i;

        }

    }


};