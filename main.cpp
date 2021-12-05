#include <iostream>
#include "Config_Import.h"

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

    Grid(std::string path) {
        int* file = Config_Import::file(path);
        x_size = Config_Import::x_size(file);
        y_size = Config_Import::y_size(file);
        bool* data =  Config_Import::data(file);
        field = new Cell[x_size*y_size];
        for (int i = 0; i < x_size*y_size; i++) {
            field[i] = Cell(data[i]);
        }
    }

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
//        for (int i = 0; i < g.x_size*g.y_size; i++) {
//            out << g.field[i].state << " ";
//        }
//        out << std::endl;

        for (int i = 0; i < g.y_size; i++) {
            for (int j = 0; j < g.x_size; j++) {
                if (g.field[i*g.x_size + j].state) {out << "@" << " ";}
                else {out << "." << " ";}
                //out << g.field[i*g.x_size + j].state << " ";
            }
            out << std::endl;
        }
        out << std::endl;

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



int main() {


    std::string tmp = "example.txt";
    Grid g(tmp);

    for (int i = 0; i < 30; i++) {
        std::cout << g;
        g.life_step();
    }
    return 0;
}
