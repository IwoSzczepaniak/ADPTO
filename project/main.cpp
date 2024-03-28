#include <fstream>
#include "common.cpp"

using namespace std;


int main() {
    ifstream inputFile("cha0.in");
    if (!inputFile.is_open()) {
        cerr << "Nie można otworzyć pliku.";
        return 1;
    }

    int rows, cols, num_players;
    inputFile >> rows >> cols >> num_players;

    vector<string> grid(rows);
    for (int i = 0; i < rows; ++i) {
        inputFile >> grid[i];
    }

    inputFile.close();

    // cout << "Wczytana siatka:";
    // for (const string& row : grid) {
    //     cout << row << endl;
    // }
    // cout << endl;

    Car dean = findDean(grid, rows, cols);
    if (dean.length == -1) {
        cerr << "Nie znaleziono samochodu dziekana.";
        return 1;
    }
    // cout << "Samochód dziekana jest na: " << dean.row << ", " << dean.col << endl;
        

    set<Car> set;
    set = explore_tree(grid, rows, cols, dean, set);

    for (auto it = set.begin(); it != set.end(); ++it) {
        if (it->type == CarType::Horizontal)
            cout << it->col << " " << it->row << " L " <<  it->col - 1 <<  endl; // assumption that always car moves max to the left TODO
        else if (it->type == CarType::Vertical)
            cout << it->col << " " << it->row << " U " <<  1  <<  endl; // assumption that always car moves UP TODO
        else
            cout << "Unknown type" << endl;
    }

    cout << dean.col << " " << dean.row << " R " <<  cols - dean.col <<  endl;

    return 0;
}
