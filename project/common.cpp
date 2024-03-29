#include <vector>
#include <set>
#include <iostream>
#include "Car.cpp"

Car findDean(const vector<string>& grid, const int& rows, const int& cols) {

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            char current = grid[i][j];
            if (grid[i][j] == 'o') {
                int k = j;
                while(k < cols && grid[i][k] == 'o'){
                    k++;
                }
                int carLength = k - j;
                return Car(CarType::Dean, carLength, i, j);
            }
        }
    }

    return Car(CarType::Dean, -1, -1, -1);
}

void detectHorizontalColliding(vector<string>& grid, const int& rows, const int& cols, Car &parent, char current, int i, int j){
    // assume we look for cars on the right
    int k = j+1;
    while(k < cols && grid[i][k] == current + 1){
        current = grid[i][k];
        k++;
    }
    k -= 1;

    int carLength = current - 'a' + 1;
    parent.children.emplace_back(CarType::Horizontal, carLength, i, k-carLength + 1);
}

void detectVerticalColliding(vector<string>& grid, const int& rows, const int& cols, Car &parent, char current, int i, int j){

    // assume we look for cars above
    int k = i+1;
    while(k > 0 && (grid[k][j] == current + 1 || grid[k][j] == 'w')){
        current = grid[k][j];
        k++;
    }
    k -= 1; 

    int carLength = current - 'x' + 1;
    if (current == 'w')
        carLength = 4;
    
    parent.children.emplace_back(CarType::Vertical, carLength, k-carLength + 1, j);
}

void findCarsOnLine(vector<string>& grid, const int& rows, const int& cols, Car &parent) {
    
    int i = parent.row;
    int j = parent.col;
    if (CarType::Dean == parent.type){
        // TODO INDEX PROBLEM
        for(;j<cols;++j){
            char current = grid[i][j];

            if (current == '.' || current == '#')
                continue;

            else if ((current >= 'x' and current <= 'z') || current == 'w' ) { 
                detectVerticalColliding(grid, rows, cols, parent, current, i, j);
            }
        }
    }

    else if(CarType::Horizontal == parent.type){
        // TODO INDEX PROBLEM
        for(;j>0;--j){
            char current = grid[i][j];
            if (current == '.' || current == '#')
                continue;
            else if ((current >= 'x' and current <= 'z') || current == 'w' ) { 
                detectVerticalColliding(grid, rows, cols, parent, current, i, j);
            }
            else if(current >= 'a' and current <= 'd'){
                // TODO
            }
        }
    }

    else{
        // TODO INDEX PROBLEM
        for(;i>0;--i){
            char current = grid[i][j];
            if (current == '.' || current == '#')
                continue;
            else if (current >= 'a' and current <= 'd') {
                detectHorizontalColliding(grid, rows, cols, parent, current, i, j);
            }
        }
    }
}

void putCollidingOnset(Car &parent, set<Car> &set){
    vector<Car> carsOnLine = parent.children;
    for (const Car& car : carsOnLine) {
        set.insert(car);
    }
}

set<Car> explore_tree(vector<string>& grid, const int& rows, const int& cols, Car &current, set<Car> &set){
    
    findCarsOnLine(grid, rows, cols, current);
    if (current.children.empty()){
        set.insert(current);

        return set; // only to speed up
    }

    putCollidingOnset(current, set);

    for (Car& car : current.children) {
        explore_tree(grid, rows, cols, car, set);
    }
    return set;
}