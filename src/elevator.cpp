#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

const int MAX_ELEVATORS = 4;
const int MAX_PASSENGERS = 8;
const int MAX_FLOORS = 100;

class Passenger {
   public:
    enum Status {
        NOT_SET,
        WAITING_ELEVATOR_UP,
        WAITING_ELEVATOR_DOWN,
        RIDING_ELEVATOR,
        AT_DESTINATION
    };
    Passenger(int id, int start_time, int start_floor, int end_floor,
              Status status)
        : passenger_ID(id),
          start_time(start_time),
          start_floor(start_floor),
          end_floor(end_floor),
          status(status) {}

    inline int get_start_time() const { return start_time; }
    inline int get_start_floor() const { return start_floor; }
    inline int get_end_floor() const { return end_floor; }
    inline void set_status(Passenger::Status s) { this->status = s; }
    inline Status get_status() { return this->status; }
    inline void set_time_picked_up(int currentTime) {this->time_picked_up = currentTime;}
    inline void set_time_dropped_off(int currentTime) {this->time_dropped_off = currentTime;}
    
    // Define a comparison function to compare passengers by start_time
    static bool comparePassengersByStartTime(Passenger p1, Passenger p2){
        return p1.start_time < p2.start_time;
    }

   private:
    int passenger_ID;
    int start_time;
    int end_time = -1;
    int start_floor;
    int end_floor;
    int time_picked_up;
    int time_dropped_off;
    Status status = NOT_SET;
    // Determines passenger status
};

class Floor {
   public:
    Floor(int floorNum) : floorNumber(floorNum) {}
    enum CallState { NONE, UP, DOWN, BOTH };
    const int floorNumber;
    vector<Passenger> passengersGoingUp;
    vector<Passenger> passengersGoingDown;
    void queuePassengerGoingUp(Passenger passenger);
    void queuePassengerGoingDown(Passenger passenger);
    CallState isStopRequested();

   private:
    CallState c = NONE;
};

class Building;

class Elevator {
   public:
    enum ElevatorState {
        NOT_SET,
        MOVING_UP,
        MOVING_DOWN,
        STOPPING_GOING_UP,
        STOPPING_GOING_DOWN,
        STOPPED_NO_PASSENGERS,
        STOPPED_GOING_UP,
        STOPPED_GOING_DOWN
    };

    Elevator(const int max_passengers, const int max_floors, const int time_between_floors)
        : state(STOPPED_NO_PASSENGERS),
          stop_time_left(0),
          num_passengers(0),
          MAX_PASSENGERS(max_passengers),
          MAX_FLOORS(max_floors),
          currentFloorNumber(1),
          moving_time_left(time_between_floors),
          TIME_BETWEEN_FLOORS(time_between_floors) {}
    
    int currentFloorNumber;
    int stop_time_left;
    int moving_time_left;
    int num_passengers;    
    vector<Passenger> passengers;    

    void add_passenger(Passenger passenger);    
    void move_up();
    void move_down();
    void stop();
    void update(Building &building);
    void loadPassengers(Building &building);
    void unloadPassengers(Building &building);
    bool hasPassengerOnCurrentFloor();

    ElevatorState get_state() const;    

   private:
    ElevatorState state;    
    const int MAX_PASSENGERS;
    const int MAX_FLOORS;
    const int DEFAULT_START_FLOOR = 1;
    const int TIME_BETWEEN_FLOORS;
};


class Building {
   public:
    Building();
    Building(int num_elevators, int num_floors, int time_between_floors);
    vector<Floor> floors;
    set<int> floorsRequestingPickup;
    vector<Floor::CallState> floorsCallState;
    vector<Elevator> elevators;
    vector<Passenger> passengersNotOnFloorsYet;
    vector<Passenger> fullPassengerList;
    int totalPassengers;
    int passengersAtDestination;
    int current_time;

    void setCurrentTime(int timeInSeconds);
    void updateFloorCallStatus();
    void getFloorsAndCallStatuses();
    void updateElevatorMovement();   
    bool doesFloorHavePassengersGoingUp(int floorNum);
    bool doesFloorHavePassengersGoingDown(int floorNum);
    void markPassengersAtDestination(int numPassengers);    
};



class RunSimulation {
   public:
    int MAX_ELEVATORS = 4;
    int MAX_PASSENGERS = 8;
    int MAX_FLOORS = 100;
    int TIME_BETWEEN_FLOORS = 10;
    int current_time = 0;
    Building building;
    
    RunSimulation();
    void generateBuilding();
    void iterateOneSecond();
    void addPassengersToBuilding(vector<Passenger>);        
};


void Floor::queuePassengerGoingUp(Passenger passenger) {
    passengersGoingUp.push_back(passenger);
    if (c == NONE) {
        c = UP;
    } else if (c == DOWN) {
        c = BOTH;
    }
}

void Floor::queuePassengerGoingDown(Passenger passenger) {
    passengersGoingDown.push_back(passenger);
    if (c == NONE) {
        c = DOWN;
    } else if (c == UP) {
        c = BOTH;
    }
}

Floor::CallState Floor::isStopRequested(){
    return c;
};


void Elevator::add_passenger(Passenger passenger) {
        if (num_passengers < MAX_PASSENGERS) {
            passengers.push_back(passenger);
            num_passengers++;
        }
    }

void Elevator::move_up() {
    currentFloorNumber++;
    moving_time_left = TIME_BETWEEN_FLOORS;     
}

void Elevator::move_down() {
    currentFloorNumber--;
    moving_time_left = TIME_BETWEEN_FLOORS;     
}

void Elevator::stop() {
    //TODO: Handling for if they are at the max or min floor? Move to STOPPED_NO_PASSENGERS?     
    if ((state == MOVING_UP && currentFloorNumber != MAX_FLOORS) || currentFloorNumber == 1) {
        state = STOPPING_GOING_UP;
    } else {
        state = STOPPING_GOING_DOWN;
    }
    stop_time_left = 2;
    moving_time_left = TIME_BETWEEN_FLOORS; //reset moving_time_left for the next move
}


void Elevator::update(Building &building) {
    if (state == STOPPING_GOING_UP) {
        stop_time_left--;
        if (stop_time_left == 0) {
            state = STOPPED_GOING_UP;
        }
    } else if (state == STOPPING_GOING_DOWN) {
        stop_time_left--;
        if (stop_time_left == 0) {
            state = STOPPED_GOING_DOWN;
        }
    } else if (state == MOVING_UP) {
        // added decrement counter to moving up moving time left = 10 or 5
        // when 0 actually decrement the floor, then the floor is changed
        moving_time_left--;
        if (moving_time_left == 0) {
            move_up();
            //you just incremented the floor number, check if you should stop here:
              //1. You have a passenger that wants to get off here
              //2. The current floor has a passenger waiting pickup to go up
              //3. you're at the max floor
            //if yes to any of the above, stop
            if(hasPassengerOnCurrentFloor() 
                || (building.doesFloorHavePassengersGoingUp(currentFloorNumber) && passengers.size() < MAX_PASSENGERS)
                || currentFloorNumber == MAX_FLOORS){
                    stop();
                }
            }
            
    } else if (state == MOVING_DOWN) {
        if (currentFloorNumber == 1) {
            stop();
        }
        // added decrement counter to moving down 10 or 5
        // when 0 actually decrement the floor, then the floor is changed
        moving_time_left--;
        if (moving_time_left == 0) {
            move_down();
            //you just decremented the floor number check if you should stop here:
              //1. You have a passenger that wants to get off here
              //2. The current floor has a passenger waiting pickup to go down
              //3. you're at the minimum floor
            //if yes to any of the above move, stop
            if(hasPassengerOnCurrentFloor() 
                || (building.doesFloorHavePassengersGoingDown(currentFloorNumber) && passengers.size() < MAX_PASSENGERS)
                || currentFloorNumber == MAX_FLOORS){
                    stop();
                }
        }
    } else if(state == STOPPED_NO_PASSENGERS){
        //call function to figure out where the closest passenger is and go that way
        if(building.floorsRequestingPickup.size() == 0){
            //Do nothing, take a nap. There are no passengers to fetch. 
        }
        else{
            int closestDistance = 999; //arbitrarily large
            int closestFloor = -1;
            for(int floor : building.floorsRequestingPickup){
                auto diff = std::abs(currentFloorNumber - floor);
                if(diff < closestDistance){
                    closestDistance = diff;
                    closestFloor = floor;
                }
            }
            // //You're already there somehow, pick them up immediately
            // if(closestFloor == currentFloorNumber){
            //     loadPassengers(building);
            //     //this is a weird case, figure out which direction to go now:
            //     if(passengers.size() > 0 && passengers.at(0).get_start_floor() < passengers.at(0).get_end_floor()) {
            //         state = MOVING_DOWN;
            //     } 
            //     else{
            //         state = MOVING_UP;
            //     }                                
            // }
            
            if(closestFloor > currentFloorNumber){
                state = MOVING_UP;                }
            else{
                state = MOVING_DOWN;
            }
        }
    }
    else if(state == STOPPED_GOING_DOWN){
        unloadPassengers(building);
        loadPassengers(building);  
        //resume moving down if you still have passengers        
        if(passengers.size() > 0){
            state = MOVING_DOWN;
        }
        //otherwise move to STOPPED_NO_PASSENGERS state
        else{
            state = STOPPED_NO_PASSENGERS;
        }
    }
    else if(state == STOPPED_GOING_UP){
        unloadPassengers(building);
        loadPassengers(building); 
        //resume moving up if you still have passengers
        if(passengers.size() > 0){
            state = MOVING_UP;
        }
        //otherwise move to STOPPED_NO_PASSENGERS state
        else{
            state = STOPPED_NO_PASSENGERS;
        }
    }
}

/// @brief Load passengers if there is room on the elevator
void Elevator::loadPassengers(Building &building){
    //Get the Floor object from the Building singleton for the current floor the elevator is on
    auto& currFloor = building.floors.at(currentFloorNumber-1);
    if(state == STOPPED_GOING_DOWN 
        || (state == STOPPED_NO_PASSENGERS && currFloor.passengersGoingDown.size() > 0) ){
        //while elevator is not full and Floor has passengers waiting to queue
        while(currFloor.passengersGoingDown.size() > 0 
            && this->num_passengers < MAX_PASSENGERS) 
        {
            Passenger passengerToAdd = currFloor.passengersGoingDown.front();
            currFloor.passengersGoingDown.erase(currFloor.passengersGoingDown.begin());
            passengers.push_back(passengerToAdd);
            num_passengers++;
            passengerToAdd.set_time_picked_up(building.current_time);
        }        
    }
    else{ //else, elevator is going up, get the passengers going up
        while(currFloor.passengersGoingUp.size() > 0 
            && this->num_passengers < MAX_PASSENGERS) 
        {
            Passenger passengerToAdd = currFloor.passengersGoingUp.front();
            currFloor.passengersGoingUp.erase(currFloor.passengersGoingUp.begin());
            passengers.push_back(passengerToAdd);
            num_passengers++;
        }
    }
    
    if(currFloor.passengersGoingDown.size() == 0 && currFloor.passengersGoingUp.size() == 0){
        //if no passengers are awaiting pickup on this floor, remove floor number from floorsRequestingPickup
        building.floorsRequestingPickup.erase(currentFloorNumber);
    }
}

void Elevator::unloadPassengers(Building &building) {
    passengers.erase(std::remove_if(passengers.begin(), passengers.end(),
        [this, &building](const Passenger& passenger) {
            if (passenger.get_end_floor() == currentFloorNumber) {  
                //TODO set value of 'time picked up' and 'time dropped off' in master list before erasing?
                building.markPassengersAtDestination(1);                             
                return true;
            }
            return false;
        }), passengers.end());

    num_passengers = passengers.size();
    //TODO update some master list that says which passengers made it to their destination
    //TODO update the passenger time when they got off the elevator for metrics
}

bool Elevator::hasPassengerOnCurrentFloor() {
    for (const auto& passenger : passengers) {
        if (passenger.get_end_floor() == currentFloorNumber) {
            return true;
        }
    }
    return false;
}

Elevator::ElevatorState Elevator::get_state() const { return state; }


Building::Building() {}

Building::Building(int num_elevators, int num_floors, int time_between_floors) {
    for (int i = 0; i < num_floors; i++) {
        floors.push_back(Floor(i + 1));
    }
    for (int i = 0; i < num_elevators; i++) {
        elevators.push_back(Elevator(num_elevators, num_floors, time_between_floors));
    }
}

void Building::setCurrentTime(int t) { current_time = t; }

void Building::updateFloorCallStatus() {
    auto it = passengersNotOnFloorsYet.begin();
    while (it != passengersNotOnFloorsYet.end()) {
        if (it->get_start_time() <= current_time) {
            if (it->get_start_floor() < it->get_end_floor()) {
                // going up
                floors.at(it->get_start_floor()-1).queuePassengerGoingUp(*it);
                it = passengersNotOnFloorsYet.erase(it);
            } else if (it->get_start_floor() > it->get_end_floor()) {
                // going down
                floors.at(it->get_start_floor()-1).queuePassengerGoingDown(*it);
                it = passengersNotOnFloorsYet.erase(it);
            } else {
                // passenger is already on the destination floor
                it = passengersNotOnFloorsYet.erase(it);
            }
        } else {
            // passengersNotOnFloorsYet is sorted by start_time.
            // When you reach a passenger whose start_time is in the future,
            // you know all those that follow will also be in the future and
            // you can stop looking
            break;
        }
    }    
}

void Building::getFloorsAndCallStatuses(){
    for (Floor f : floors){
        Floor::CallState c = f.isStopRequested();
        if (c != Floor::CallState::NONE){
            floorsRequestingPickup.insert(f.floorNumber);
            floorsCallState.push_back(c);
        }
    }
}
    
void Building::updateElevatorMovement(){
    for (Elevator& e : elevators){
        e.update(*this);
    }
}

bool Building::doesFloorHavePassengersGoingUp(int floorNum){
    Floor relevantFloor = floors.at(floorNum-1);
    if(relevantFloor.passengersGoingUp.size() > 0){
        return true;
    }
    return false;
}

bool Building::doesFloorHavePassengersGoingDown(int floorNum){
    for(int floor : floorsRequestingPickup){
        if(floor == floorNum){
            return true;
        }
    }
    return false;
}

void Building::markPassengersAtDestination(int numPassengers){
    passengersAtDestination += numPassengers;
    cout << "Passengers at destination: " << passengersAtDestination << endl;

    if(passengersAtDestination % 5 == 0){
        cout << "Floors awaiting pickup " << this->floorsRequestingPickup.size() << endl;
    }

    if(passengersAtDestination == 55 || passengersAtDestination == 77 || passengersAtDestination == 358)
    {
        for(int i = 0; i < this->elevators.size(); i++){
            cout << "state number " << this->elevators.at(i).get_state() << endl;
            cout << "number of passengers " << this->elevators.at(i).num_passengers << endl;
            cout << "current floor " << this->elevators.at(i).currentFloorNumber << endl;

        }
    }
}

RunSimulation::RunSimulation() {}

void RunSimulation::generateBuilding() {
    building = Building(MAX_ELEVATORS, MAX_FLOORS, TIME_BETWEEN_FLOORS);
}

void RunSimulation::addPassengersToBuilding(vector<Passenger> p) {    
    building.totalPassengers = p.size();
    building.passengersAtDestination = 0;    
    building.fullPassengerList = p;
    building.passengersNotOnFloorsYet = p;
    std::sort(building.passengersNotOnFloorsYet.begin(),
        building.passengersNotOnFloorsYet.end(),
        Passenger::comparePassengersByStartTime);
}

void RunSimulation::iterateOneSecond() {
    building.setCurrentTime(current_time);
    building.updateFloorCallStatus();
    building.getFloorsAndCallStatuses();
    building.updateElevatorMovement();

    current_time++;
}


vector<Passenger> getPassengerData() {
    vector<Passenger> passengers;

    string fname = "Mod10_Assignment_Elevators.csv";

    vector<vector<string>> content;
    vector<string> row;
    string line, word;

    fstream file(fname, ios::in);
    if (file.is_open()) {
        while (getline(file, line)) {
            row.clear();

            stringstream str(line);

            while (getline(str, word, ',')) row.push_back(word);
            content.push_back(row);
        }
    } else
        cout << "Could not open the file\n";

    int id = 0;
    content.erase(content.begin());
    for (vector<string> v : content) {

        vector<int> ints;
        std::transform(v.begin(), v.end(), std::back_inserter(ints),
        [&](std::string s) {
            return stoi(s);
        });
        Passenger::Status initialStatus;
        if (ints[1] > ints[2]) {
            initialStatus = Passenger::WAITING_ELEVATOR_DOWN;
        } else if (ints[1] < ints[2]) {
            initialStatus = Passenger::WAITING_ELEVATOR_UP;
        } else {
            initialStatus =
                Passenger::AT_DESTINATION;  // End floor and start floor are
                                            // the same for this passenger
        }
        passengers.push_back(Passenger(id, ints[0], ints[1],
                                        ints[2], initialStatus));
        //std::cout << ints <<" ";
        id++;

        cout << "\n";
    }

    return passengers;
}

int main() {
    RunSimulation s;
    s.generateBuilding();
    vector<Passenger> passengers = getPassengerData();
    s.addPassengersToBuilding(passengers);

    cout << "Number of passengers needing an elevator ride: " << s.building.totalPassengers << endl;
    while (s.building.passengersAtDestination < s.building.totalPassengers) {
        s.iterateOneSecond();        
    }    
    cout << "Number of passengers at their destination floor: " << s.building.passengersAtDestination << endl;
    cout << "Number of seconds elapsed: " << s.building.current_time;

    // TODO: calculate and print average wait time and average travel time
    return 0;
}
