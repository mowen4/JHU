#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

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

    int get_start_time() const { return start_time; }
    int get_start_floor() const { return start_floor; }
    int get_end_floor() const { return end_floor; }
    void set_status(Passenger::Status s) { this->status = s; }
    Status get_status() { return this->status; }

   private:
    int passenger_ID;
    int start_time;
    int end_time = -1;
    int start_floor;
    int end_floor;
    Status status = NOT_SET;
    // Determines passenger status
};

class Floor {
   public:
    Floor(int floorNum) : floorNumber(floorNum) {}
    enum CallState { NONE, UP, DOWN, BOTH };
    const int floorNumber;
    vector<Passenger> passengerGoingUp;
    vector<Passenger> passengerGoingDown;
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

    Elevator(const int max_passengers, const int max_floors, Building& bld)
        : building(bld),
          state(STOPPED_NO_PASSENGERS),
          stop_time_left(0),
          num_passengers(0),
          MAX_PASSENGERS(max_passengers),
          MAX_FLOORS(max_floors) {}
    
    int currentFloorNumber;
    int stop_time_left;
    int moving_time_left;
    int num_passengers;
    Building& building;
    vector<Passenger> passengers;    

    void add_passenger(Passenger passenger);    
    void move_up();
    void move_down();
    void stop();
    void update();
    void loadPassengers();
    void unloadPassengers();

    ElevatorState get_state() const;    

   private:
    ElevatorState state;    
    const int MAX_PASSENGERS;
    const int MAX_FLOORS;
    const int DEFAULT_START_FLOOR = 1;
};


class Building {
   public:
    Building();
    Building(int num_elevators, int num_floors);
    vector<Floor> floors;
    vector<int> floorsRequestingPickup;
    vector<Floor::CallState> floorsCallState;
    vector<Elevator> elevators;
    vector<Passenger> passengersNotOnFloorsYet;

    void setCurrentTime(int timeInSeconds);
    void updateFloorCallStatus();
    void getFloorsAndCallStatuses();
    void UpdateElevatorMovement();
    static void unloadPassengers(Elevator elevator);
    static void loadPassengers(Elevator elevator);
    int current_time;
};



class RunSimulation {
   public:
    int MAX_ELEVATORS = 4;
    int MAX_PASSENGERS = 8;
    int MAX_FLOORS = 100;
    int TIME_BETWEEN_FLOORS = 10;
    int current_time = 0;
    RunSimulation();
    void generateBuilding();
    void iterateOneSecond();
    void addPassengersToBuilding(vector<Passenger>);

    Building b;
};


void Floor::queuePassengerGoingUp(Passenger passenger) {
    passengerGoingUp.push_back(passenger);
    if (c == NONE) {
        c = UP;
    } else if (c == DOWN) {
        c = BOTH;
    }
}

void Floor::queuePassengerGoingDown(Passenger passenger) {
    passengerGoingDown.push_back(passenger);
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
        // TODO: is someone getting off or on this floor?
        //       if so stop()
        //       else carry on
        bool stopHere = false;
        for (Passenger p : passengers) {
            if (p.get_end_floor() == currentFloorNumber) {
                stopHere = true;
            }
        }

        if (stopHere) {
            stop();
        } else if (num_passengers <
                   MAX_PASSENGERS)  // removing this for now &&
                                    // current_floor.isStopRequested()) //this
                                    // needs to be to check for passengers on
                                    // floor
        // also lol smart elevator birches.... no stops if no room
        {
            stop();
        } else {
            // continue on
            state = MOVING_UP;
        }
    }

void Elevator::move_down() {
    currentFloorNumber--;
    // TODO: is someone getting off or on this floor?
    //      if so stop()
    //      else carry on
    bool stopHere = false;
    for (Passenger p : passengers) {
        if (p.get_end_floor() == currentFloorNumber) {
            stopHere = true;
        }
    }

    if (stopHere) {
        stop();
    } else if (num_passengers <
                MAX_PASSENGERS)  // && current_floor.isStopRequested())
                                // //this needs to be to check for
                                // passengers on floor
    // also lol smart elevator birches.... no stops if no room
    {
        stop();
    } else {
        // continue on
        state = MOVING_DOWN;
    }
}

void Elevator::stop() {
    if (this->state == MOVING_UP) {
        state = STOPPING_GOING_UP;
    } else {
        state = STOPPING_GOING_DOWN;
    }
    stop_time_left = 2;
}


void Elevator::update() {
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
            if (currentFloorNumber == MAX_FLOORS) {
                stop();
            }

            // added decrement counter to moving up moving time left = 10 or 5
            // when 0 actually decrement the floor, then the floor is changed
            moving_time_left--;
            if (moving_time_left == 0) {
                move_up();
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
            }
        }
        else if(state == STOPPED_NO_PASSENGERS){
            //call function to figure out where the closest passenger is and go that way
        }
        else if(state == STOPPED_GOING_DOWN || state == STOPPED_GOING_UP){
            unloadPassengers();
            loadPassengers();   
        }
    }

/// @brief Load passengers if there is room on the elevator
void Elevator::loadPassengers(){
    //Get the Floor object from the Building singleton for the current floor the elevator is on
    auto currFloor = building.floors.at(currentFloorNumber-1);
    if(state == STOPPED_GOING_DOWN){
        //while elevator is not full and Floor has passengers waiting to queue
        while(currFloor.passengerGoingDown.size() > 0 
            && this->num_passengers < MAX_PASSENGERS) 
        {
            Passenger passengerToAdd = currFloor.passengerGoingDown.front();
            currFloor.passengerGoingDown.erase(currFloor.passengerGoingDown.begin());
            passengers.push_back(passengerToAdd);
            num_passengers++;
        }        
    }
    else{ //else, elevator is going up, get the passengers going up
        while(currFloor.passengerGoingUp.size() > 0 
            && this->num_passengers < MAX_PASSENGERS) 
        {
            Passenger passengerToAdd = currFloor.passengerGoingUp.front();
            currFloor.passengerGoingUp.erase(currFloor.passengerGoingUp.begin());
            passengers.push_back(passengerToAdd);
            num_passengers++;
        }
    }
}

/// @brief Remove Passengers from the elevator if their end_floor value matches 
/// the current floor
void Elevator::unloadPassengers(){  
    passengers.erase(std::remove_if(
        passengers.begin(), passengers.end(), [this](const Passenger& passenger) 
        {return passenger.get_end_floor() == currentFloorNumber;}),
         passengers.end());   
    num_passengers--; 
}

Elevator::ElevatorState Elevator::get_state() const { return state; }


Building::Building() {}

Building::Building(int num_elevators, int num_floors) {
    for (int i = 0; i < num_floors; i++) {
        floors.push_back(Floor(i + 1));
    }
    for (int i = 0; i < num_elevators; i++) {
        elevators.push_back(Elevator(num_elevators, num_floors, *this));
    }
}

void Building::setCurrentTime(int t) { current_time = t; }

void Building::updateFloorCallStatus() {
    for (Passenger p : passengersNotOnFloorsYet) {
        if (p.get_start_time() <= current_time) {
            if (p.get_start_floor() < p.get_end_floor()) {
                // going up
                floors.at(p.get_start_floor()-1).queuePassengerGoingUp(p);
            } else if (p.get_start_floor() > p.get_end_floor()){
                // going down
                floors.at(p.get_start_floor()-1).queuePassengerGoingDown(p);
            }
        }
    }
}

void Building::getFloorsAndCallStatuses(){

    for (Floor f : floors){
        Floor::CallState c = f.isStopRequested();
        if (c != Floor::CallState::NONE){
            floorsRequestingPickup.push_back(f.floorNumber);
            floorsCallState.push_back(c);
        }
    }

}

RunSimulation::RunSimulation() {}

void RunSimulation::generateBuilding() {
    b = Building(MAX_ELEVATORS, MAX_FLOORS);
}

void RunSimulation::addPassengersToBuilding(vector<Passenger> p) {
    b.passengersNotOnFloorsYet = p;
}

void RunSimulation::iterateOneSecond() {
    b.setCurrentTime(current_time);
    b.updateFloorCallStatus();
    b.getFloorsAndCallStatuses();

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

    while (true) {
        s.iterateOneSecond();
        // TODO: check if all passengers have reached their destination and exit
        // loop if so
    }

    // TODO: calculate and print average wait time and average travel time
    return 0;
}
