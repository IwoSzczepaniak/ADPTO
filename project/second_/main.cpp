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

    bool onTheEdge(const int &H, const int &W){
        return (x + size >= W) || (y + size >= H);
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
    Car currentCar = cars[carNumber];

    if (canMoveNSteps(carNumber, *prevState, n, way)){
        currentCar.x += (way == 'R') ? n : (way == 'L') ? -n : 0;
        currentCar.y += (way == 'D') ? n : (way == 'U') ? -n : 0;

        if (currentCar.special && currentCar.onTheEdge(prevState->map.size(), prevState->map[0].size())){
            n+=1;
        }
        moves.push(to_string(currentCar.x) + ' ' + to_string(currentCar.y) + ' ' + way + ' ' + to_string(n));
    } else {
        return NULL;
    }

    cars[carNumber] = currentCar;
    vector<string> newMap = createMap(prevMap, cars);
    
    Node* newState = new Node(cars, prevState, "", newMap);
    newState->moves = moves;
    return newState;
}

Node* search(Node* root, const int &maxMoves, int& currentMoves) {
    if (currentMoves > maxMoves) {
        return NULL;
    }

    for (Car car : root->cars) {
        if (car.special) {
            if ((car.direction == 'h' && car.x + car.size >= root->map[0].size()) || 
                (car.direction == 'v' && car.y + car.size >= root->map.size())) {
                return root;
            }
        }
    }

    int longestMove = max(root->map.size(), root->map[0].size());
    for (int i = 0; i < root->cars.size(); i++) {
        for (char direction : {'U', 'D', 'L', 'R'}) {
            for (int n = 1; n <= longestMove; n++) { 
                Node* newNode = move(root, i, n, direction);
                if (newNode != NULL) {
                    currentMoves++;
                    Node* result = search(newNode, maxMoves, currentMoves);
                    if (result != NULL) {
                        return result;
                    }
                    currentMoves--;
                } 
            }
        }
    }

    return NULL;
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

    int currentMoves = 0;
    Node* solution = search(root, N, currentMoves);

    stack<string> res;
    if (solution != NULL) {
        stack<string> moves = solution->moves;
        int numMoves = moves.size();
        cout << numMoves << endl;

        while (!moves.empty()) {
            res.push(moves.top());
            moves.pop();
        }
        while (!res.empty()) {
            cout << res.top() << endl;
            res.pop();
        }
    } else {
        cout << "Nie znaleziono rozwiązania w maksymalnej liczbie ruchów N." << endl;
    }

    delete root;
    return 0;
}