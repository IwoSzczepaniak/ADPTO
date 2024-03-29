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

void detectHorizontalColliding(vector<string>& grid, const int& rows, const int& cols, Car &parent, char current, const int& i, const int& j){
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

int detectHorizontalOnLine(vector<string>& grid, const int& rows, const int& cols, Car &parent, char current, int i, int j){
    int carLength = current - 'a' + 1;
    parent.children.emplace_back(CarType::Horizontal, carLength, i, j-carLength + 1);
    return carLength;
}

void findCarsOnLine(vector<string>& grid, const int& rows, const int& cols, Car &parent) {
    
    int i = parent.row;
    int j = parent.col;
    if (CarType::Dean == parent.type){
        j++;
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
        --j;
        for(;j>0;--j){
            char current = grid[i][j];
            if (current == '.' || current == '#')
                continue;
            else if(current >= 'a' and current <= 'd'){
                j-=detectHorizontalOnLine(grid, rows, cols, parent, current, i, j);
            }
            else if ((current >= 'x' and current <= 'z') || current == 'w' ) { 
                detectVerticalColliding(grid, rows, cols, parent, current, i, j);
            }
        }
    }

    else{
        --i;
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

void insertCar(Car &car, set<Car> &set, int parent_move){
    // if car is horizontal every new collision adds one more move
    if (car.type == CarType::Horizontal && set.find(car) != set.end()){
            car.move = set.find(car)->move + 1;
            if (parent_move > 0)
                car.move == parent_move;
            set.erase(car);
    }
    set.insert(car);
}

void putCollidingOnSet(Car &parent, set<Car> &set){
    vector<Car> carsOnLine = parent.children;
    for (Car& car : carsOnLine) {
        insertCar(car, set, parent.move);
    }
}

set<Car> explore_tree(vector<string>& grid, const int& rows, const int& cols, Car &current, set<Car> &set){
    
    findCarsOnLine(grid, rows, cols, current);
    if (current.children.empty()){
        insertCar(current, set, 0);
        return set; // only to speed up
    }

    putCollidingOnSet(current, set);

    for (Car& car : current.children) {
        explore_tree(grid, rows, cols, car, set);
    }
    return set;
}