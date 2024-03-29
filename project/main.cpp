#include <algorithm>
#include <fstream>
#include "common.cpp"

using namespace std;


int main() {
    // for (const string& row : grid) {
    //     cout << row << endl;
    // }
    // cout << endl;

    int cols, rows, max_moves;
    cin >> cols >> rows >> max_moves;

    vector<string> grid(rows);
    for (int i = 0; i < rows; ++i) {
        cin >> grid[i];
    }


    bool reversed = false;
    auto trueRow = [rows, reversed](const Car car){
        if (reversed)
            return rows - car.row  - 1;
        else
            return car.row;
    };

    auto moveVertical = [reversed](auto &it){
        if (it->row == 1 && !reversed || it->row == 3 && reversed)
            return " D ";
        else
            return " U ";
    };  

    Car dean = findDean(grid, rows, cols);
    if (dean.length == -1) {
        cerr << "Nie znaleziono samochodu dziekana.";
        return 1;
    }

    else if (dean.row == 2){
        cout << dean.col << " " << dean.row << " R " <<  cols - dean.col <<  endl;
        return 0;
    }

    else if (dean.row == 1){
        reverse(grid.begin(), grid.end());
        dean = findDean(grid, rows, cols);
        reversed = true;
    }    

    set<Car> set;
    set = explore_tree(grid, rows, cols, dean, set);

    cout << set.size() + 1 << endl;

    for (auto it = set.begin(); it != set.end(); ++it) {

        if (it->type == CarType::Horizontal)
            cout << it->col << " " << trueRow(*it) << " L " <<  it->move <<  endl; // problems with move for many cars in a row

        else if (it->type == CarType::Vertical)
            cout << it->col << " " << trueRow(*it) << moveVertical(it) <<  it->move  <<  endl;
            
        else
            cout << "Unknown type" << endl;
    }
    cout << dean.col << " " << trueRow(dean) << " R " <<  cols - dean.col <<  endl;

    return 0;
}
