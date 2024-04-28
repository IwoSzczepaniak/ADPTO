#include <iostream>
#include <vector>
#include <deque>
#include <unordered_set>
#include <chrono>
#include <algorithm>
#include <stack>

using namespace std;

char HORIZONTAL_SYMBOLS[] = {'a', 'b', 'c', 'd'};
char VERTICAL_SYMBOLS[] = {'x', 'y', 'z', 'w'};

class Car {
public:
    int size;
    int x;
    int y;
    char direction;
    bool special = false;

    Car(int size, int x, int y, char direction)
        : size(size), x(x), y(y), direction(direction) {}

    void printCar() {
        cout << "size:" << size << " x:" << x << " y:" << y << " dir:" << direction << endl;
    }
};

class Node {
public:
    vector<Car> cars;
    Node* parent;
    string operation;
    vector<string> map;
    stack<string> moves;


    Node(vector<Car>& cars, Node* parent, string operation, vector<string>& map)
        : cars(cars), parent(parent), operation(operation), map(map) {}
};

vector<string> loadMap(int H, int W) {
    vector<string> map;
    string line;
    for(int i = 0; i < H; i++) {
        cin >> line;
        map.push_back(line);
    }
    return map;
}

vector<Car> findCars(vector<string> map) {
    vector<Car> cars;
    Car special_car(0, -1, -1, 'h');
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (map[y][x] == 'o') {
                if (special_car.x == -1) {
                    special_car.x = x;
                    special_car.y = y;
                    special_car.special = true;
                }
                special_car.size++;
                
            } else if (map[y][x] == 'a') {
                int size = 1;
                while (x + size < map[y].size() && map[y][x + size] == HORIZONTAL_SYMBOLS[size]) {
                    size++;
                }
                cars.push_back(Car(size, x, y, 'h'));
                
            }else if (map[y][x] == 'x') {
                int size = 1;
                while (y + size < map.size() && map[y + size][x] == VERTICAL_SYMBOLS[size]) {
                    size++;
                }
                cars.push_back(Car(size, x, y, 'v'));
            }
        }
    }
    cars.push_back(special_car);
    return cars;
}

vector<string> createMap(vector<string> previousMap, vector<Car> cars) {
    vector<string> map = previousMap;

    for(int y = 1; y < map.size() - 1; y++) {
        for(int x = 1; x < map[y].size() - 1; x++) {
            map[y][x] = '.';
        }
    }

    for(Car car : cars) {
        if (car.special && car.direction == 'h') {
            for(int i = 0; i < car.size; i++) {
                map[car.y][car.x + i] = 'o';
            }
        }
        else if (car.special && car.direction == 'v') {
            for(int i = 0; i < car.size; i++) {
                map[car.y + i][car.x] = 'o';
            }
        }
        else if (car.direction == 'h') {
            for(int i = 0; i < car.size; i++) {
                map[car.y][car.x + i] = HORIZONTAL_SYMBOLS[i];
            }
        } else {
            for(int i = 0; i < car.size; i++) {
                map[car.y + i][car.x] = VERTICAL_SYMBOLS[i];
            }
        }
    }
    return map;
}


bool canMoveNSteps(const int &carNumber, Node& state, const int n, const char way) {
    Car currentCar = state.cars[carNumber];

    if(currentCar.direction == 'v' && (way == 'L' || way == 'R'))
        return false;
    if(currentCar.direction == 'h' && (way == 'U' || way == 'D'))
        return false;

    if (way == 'L'){
        if (currentCar.x - n < 0){
            return false;
        }
        for (int i = 1; i <= n; i++){
            if (state.map[currentCar.y][currentCar.x - i] != '.'){
                return false;
            }
        }

    }

    else if (way == 'R'){
        if (currentCar.x + n >= state.map[0].size()){
            return false;
        }
        for (int i = 1; i <= n; i++){
            if (state.map[currentCar.y][currentCar.x + i + currentCar.size - 1] != '.'){
                return false;
            }
        }

    }

    else if (way == 'U'){
        if (currentCar.y - n < 0){
            return false;
        }
        for (int i = 1; i <= n; i++){
            if (state.map[currentCar.y - i][currentCar.x] != '.'){
                return false;
            }
        }

    }

    else if (way == 'D'){
        if (currentCar.y + n >= state.map.size()){
            return false;
        }
        for (int i = 1; i <= n; i++){
            if (state.map[currentCar.y + i + currentCar.size - 1][currentCar.x] != '.'){
                return false;
            }
        }

    }

    return true;
}

Node* move(Node* prevState, int carNumber, int n, char way) {
    vector<Car> cars = prevState->cars;
    vector<string> prevMap = prevState->map;
    stack<string> moves = prevState->moves;

    if (canMoveNSteps(carNumber, *prevState, n, way)){
        cars[carNumber].x += (way == 'R') ? n : (way == 'L') ? -n : 0;
        cars[carNumber].y += (way == 'D') ? n : (way == 'U') ? -n : 0;
        moves.push(to_string(cars[carNumber].x) + ' ' + to_string(cars[carNumber].y) + ' ' + way + ' ' + to_string(n));
    } else {
        return NULL;
    }

    vector<string> newMap = createMap(prevMap, cars);
    
    Node* newState = new Node(cars, prevState, "", newMap);
    newState->moves = moves;
    return newState;
}


Node* search(Node* root) {
    return root;
}




int main() {
    int W, H, N;
    cin >> W >> H >> N;
    
    vector<string> map = loadMap(H, W);

    for(int i = 0; i < map.size(); i++) {
        cout << map[i] << endl;
    }

    vector<Car> cars = findCars(map);
    Node* root = new Node(cars, NULL, "", map);

    Node* newNode = move(root, 1, 1, 'D');
    if (newNode != NULL) {
        cout << "I can move" << endl;
        for(int i = 0; i < newNode->map.size(); i++) {
            cout << newNode->map[i] << endl;
        }
        stack<string> moves = newNode->moves;
        while (!moves.empty()) {
            cout << moves.top() << endl;
            moves.pop();
        }
    } else {
        cout << "I can't move" << endl;
    }



    // Node* result = search(root);
    return 0;
}