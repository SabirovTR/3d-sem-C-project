#include <iostream>
#include "ParaView_Export.h"
#include "Grid.h"

class Life {
private:
public:
    static void create(Grid& g, int steps) {
        for (int step = 0; step < steps; step++) {
            ParaView_Export::file(g.Grid_Import(), step, g.x(), g.y());
            g.step();
        }
    }
};

int main() {
    Grid g("gun.txt");
    Life::create(g, 100);
    return 0;
}
