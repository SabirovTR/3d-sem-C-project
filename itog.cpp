#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Grid{
    struct Cell{
        bool state;
        int count;

        explicit
        Cell(bool state = false) : state(state), count(0) {};

        Cell(const Cell &c) = default;
        Cell& operator=(const Cell &c) = default;

        Cell(Cell &&c) {
            if (this != &c) {
                bool tmp_state = c.state;
                int tmp_count = c.count;
                c.~Cell();
                this->state = tmp_state;
                this->count = tmp_count;
            }
        };
        Cell& operator=(Cell &&c) {
            if (this == &c) {
                return *this;
            } else {
                bool tmp_state = c.state;
                int tmp_count = c.count;
                c.~Cell();
                state = tmp_state;
                count = tmp_count;
                return *this;
            }
        };

        ~Cell() = default;

        friend std::ostream& operator<<(std::ostream& out, const Cell& c) {
            out << c.state;
            return out;
        }
    };
private:
    int x_size;
    int y_size;
    Cell* field;
public:
    explicit
    Grid(int x_size, int y_size, bool state = false) : x_size(x_size), y_size(y_size) {
        field = new Cell[x_size*y_size];
        for (int i = 0; i < x_size*y_size; i++) {field[i] = Cell(state);}
    };

    Grid(const Grid &g) {
        for (int i = 0; i < x_size*y_size; i++) {
            field[i] = g.field[i];
        }
    }
    Grid& operator=(const Grid &g) {
        if (this != &g) {
            for (int i = 0; i < x_size*y_size; i++) {
                field[i] = g.field[i];
            }
        }
        return *this;
    }

    Grid(Grid &&g) {
        if (this != &g) {
            Cell* tmp = g.field;
            g.field = nullptr;
            field = tmp;
        }
    };
    Grid& operator=(Grid &&g) {
        if (this == &g) {
            return *this;
        } else {
            Cell* tmp = g.field;
            g.field = nullptr;
            field = tmp;
            return *this;
        }
    };

    ~Grid() {
        delete [] field;
    };

    friend std::ostream& operator<<(std::ostream& out, const Grid& g) {
        for (int i = 0; i < g.x_size*g.y_size; i++) {
            out << g.field[i].state << " ";
        }
        out << std::endl;

//        for (int i = 0; i < g.y_size; i++) {
//            for (int j = 0; j < g.x_size; j++) {
//                if (g.field[i*g.x_size + j].state) {out << "@" << " ";}
//                else {out << "." << " ";}
//                //out << g.field[i*g.x_size + j].state << " ";
//            }
//            out << std::endl;
//        }
//        out << std::endl;

//        for (int i = 0; i < y_size; i++) {
//            for (int j = 0; j < x_size; j++) {
//                out << g.field[i*x_size + j].count << " ";
//            }
//            out << std::endl;
//        }
//        out << std::endl;
        out << std::endl;
        return out;
    };

    void insert(int i, int j, bool state) {
        field[i*x_size + j] = Cell(state);
    };

    bool operator[](int i) const {
        return &this->field[i];
    };

    int recalculate_cell_count(int i, int j) {
        int count = 0;
        for (int p = -1; p < 2; p++) {
            for (int q = -1; q < 2; q++) {
                if ((0 <= (i + p) && (i + p) < y_size && 0 <= (j + q) && (j + q) < x_size && p+p + q*q != 0) &&
                    (field[(i + p)*x_size + (j + q)]).state) {
                    count++;
                }
            }
        }
        return count;
    };

    void recalculate_field_count() {
        for (int i = 0; i < y_size; i++) {
            for (int j = 0; j < x_size; j++) {
                field[i*x_size + j].count = recalculate_cell_count(i, j);
            }
        }
    }

    void recalculate_field_state() {
        for (int i = 0; i < y_size; i++) {
            for (int j = 0; j < x_size; j++) {
                bool tmp_state = field[i*x_size + j].state;
                int tmp_count = field[i*x_size + j].count;
                if (tmp_state) {
                    if (tmp_count != 2 && tmp_count != 3) {field[i*x_size + j].state = false;}
                } else {
                    if (tmp_count == 3) {field[i*x_size + j].state = true;}
                }
            }
        }
    }

    void life_step() {
        this->recalculate_field_count();
        this->recalculate_field_state();
    }
};

void file(Grid& g, string& name, int x, int y) {
    ofstream fout;
    fout.open(name);
    fout << "# vtk DataFile Version 3.0\n";
    fout << "vtk output\n";
    fout << "ASCII" << "\n";
    fout << "DATASET RECTILINEAR_GRID\n";
    fout << "DIMENSIONS " << x + 1 << " " << y + 1 << " 1\n";
    fout << "X_COORDINATES " << x + 1 << " float\n";
    for (int i = 0; i < x + 1; i++)
        fout << i << " ";
    fout << "\n";
    fout << "Y_COORDINATES " << y + 1 << " float\n";
    for (int i = 0; i < y + 1; i++)
        fout << i << " ";
    fout << "\n";
    fout << "Z_COORDINATES 1 float\n";
    fout << "0\n";
    fout << "CELL_DATA " << x * y << "\n";
    fout << "FIELD FieldData 1\n";
    fout << "cellscalar 1 " << x * y << " float\n";
        fout << g;
    fout << "\n";
    fout << "POINT_DATA " << (x + 1) * (y + 1) << "\n";
    fout.close();
};

int main() {
    int x, y;
    cin >> x >> y;

    Grid g(x, y, false);

    g.insert(3, 2, true);
    g.insert(3, 3, true);
    g.insert(3, 4, true);
    g.insert(2, 4, true);
    g.insert(1, 3, true);

    for (int t = 1; t < 25; t++) {
        string name = "file" + to_string(t) + ".vtk";
        file(g, name, x, y);
        g.life_step();
    }
    return 0;
}
