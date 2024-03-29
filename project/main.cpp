#include <algorithm>
#include <fstream>
#include "common.cpp"

using namespace std;


int main() {
    ifstream inputFile("cha0_rev.in");
    if (!inputFile.is_open()) {
        cerr << "Nie można otworzyć pliku.";
        return 1;
    }

    int cols, rows, max_moves;
    inputFile >> cols >> rows >> max_moves;

    vector<string> grid(rows);
    for (int i = 0; i < rows; ++i) {
        inputFile >> grid[i];
    }

    inputFile.close();

    
    cout << "Wczytana siatka:" << endl;
    for (const string& row : grid) {
        cout << row << endl;
    }
    cout << endl;
    bool reversed = false;

    Car dean = findDean(grid, rows, cols);
    if (dean.length == -1) {
        cerr << "Nie znaleziono samochodu dziekana.";
        return 1;
    }

    else if (dean.row == 2){
        cout << dean.col << " " << dean.row << " R " <<  cols - dean.col <<  endl;
        return 0;
    }

    // cout << "Samochód dziekana jest na: " << dean.row << ", " << dean.col << endl;
    else if (dean.row == 1){
        reverse(grid.begin(), grid.end());
        dean = findDean(grid, rows, cols);
        reversed = true;
    }    

    set<Car> set;
    set = explore_tree(grid, rows, cols, dean, set);

    if (!reversed) {
        for (auto it = set.begin(); it != set.end(); ++it) {
            if (it->type == CarType::Horizontal)
                cout << it->col << " " << it->row << " L " <<  it->col - 1 <<  endl; // assumption that always car moves max(sol. is class field) to the LEFT TODO
            else if (it->type == CarType::Vertical)
                cout << it->col << " " << it->row << " U " <<  1  <<  endl;
            else
                cout << "Unknown type" << endl;
        }
        cout << dean.col << " " << dean.row << " R " <<  cols - dean.col <<  endl;

    } 

    else {
        cout << "Reversed" << endl;
        for (auto it = set.begin(); it != set.end(); ++it) {
            if (it->type == CarType::Horizontal)
                cout << it->col << " " << rows - it->row  - 1 << " L " <<  it->col - 1 <<  endl; // assumption that always car moves max(sol. is class field) to the LEFT TODO
            else if (it->type == CarType::Vertical)
                cout << it->col << " " << rows - it->row - 1 << " D " <<  1 <<  endl;
        }
        cout << dean.col << " " << rows - dean.row - 1 << " R " <<  cols - dean.col <<  endl;

    }

    return 0;
}
