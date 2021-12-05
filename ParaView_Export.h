//
// Created by User on 05.12.2021.
//

#ifndef MAIN_CPP_PARAVIEW_EXPORT_H
#define MAIN_CPP_PARAVIEW_EXPORT_H

#endif //MAIN_CPP_PARAVIEW_EXPORT_H

#include <fstream>
#include <string>

class ParaView_Export {
private:
public:
    static void file(bool* g, int step, int x, int y) {
        std::string name = "file" + std::to_string(step) + ".vtk";
        std::ofstream fout;
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
        for (int i = 0 ; i < x*y; i++)
            fout << g[i] << " ";
        fout << "\n";
        fout << "POINT_DATA " << (x + 1) * (y + 1) << "\n";
        fout.close();
    };
};