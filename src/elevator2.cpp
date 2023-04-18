#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Passenger {
   public:
    enum Status {
        NOT_SET,
        WAITING_ELEVATOR_UP,
        WAITING_ELEVATOR_DOWN,
        RIDING_ELEVATOR,
        AT_DESTINATION
    };
    Passenger(int id, int start_time, int start_floor, int end_floor, Status status)
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
    int start_floor;
    int end_floor;
    Status status = NOT_SET;
    // Determines passenger status
};


class Floor {
   public:
    Floor(int floorNum) : floorNumber(floorNum) {}
    const int floorNumber;    
    vector<Passenger> passengersAwaitingPickup;
    bool isStopMovingUpRequested();    
    bool isStopMovingDownRequested();    
    void queuePassenger(Passenger passenger);

   private:    
};

bool Floor::isStopMovingUpRequested()
{
    //TODO: update this to check and see if passenger who wants to go up requested stop
    return false;
}

bool Floor::isStopMovingUpRequested()
{
    //TODO: update this to check and see if passenger who wants to go down requested stop
    return false;
}


void Floor::queuePassenger(Passenger passenger){
    passengersAwaitingPickup.push_back(passenger);
}


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

    Elevator(const int max_passengers, const int max_floors)
        : state(STOPPED_NO_PASSENGERS),          
          stop_time_left(0),
          num_passengers(0),
          MAX_PASSENGERS(max_passengers),
          MAX_FLOORS(max_floors) {}

    void add_passenger(Passenger passenger) {
        if (num_passengers < MAX_PASSENGERS) {
            passengers.push_back(passenger);
            num_passengers++;
        }
    }

    int getCurrentFloor(){
        return currentFloorNumber;
    }

    // TODO: should take some period of time to move between floors
    //  10 seconds in the original problem statement and then
    //  5 seconds for the modified version.
    // Doesn't seem like this is currently being handled anywhere yet
    void move_up() {
        currentFloorNumber++;
        // TODO: is someone getting off or on this floor?
        //       if so stop()
        //       else carry on
        bool stopHere = false;
        for (Passenger p : passengers)
        {
            if (p.get_end_floor() == currentFloorNumber)
            {
                stopHere = true;
            }
        }

        if (stopHere)
        {
            stop();
        } else if(num_passengers < MAX_PASSENGERS) //removing this for now && current_floor.isStopRequested()) //this needs to be to check for passengers on floor
        //also lol smart elevator birches.... no stops if no room
        {
            stop();
        } else {
            //continue on
            state = MOVING_UP;
        }
    }

    void move_down() {
        currentFloorNumber--;        
        // TODO: is someone getting off or on this floor?
        //      if so stop()
        //      else carry on
        bool stopHere = false;
        for (Passenger p : passengers)
        {
            if (p.get_end_floor() == currentFloorNumber)
            {
                stopHere = true;
            }
        }

        if (stopHere)
        {
            stop();
        } else if(num_passengers < MAX_PASSENGERS) // && current_floor.isStopRequested()) //this needs to be to check for passengers on floor
        //also lol smart elevator birches.... no stops if no room
        {
            stop();
        } else {
            //continue on
            state = MOVING_DOWN;
        }
    }

    void stop() {
        if (this->state == MOVING_UP) {
            state = STOPPING_GOING_UP;
        } else {
            state = STOPPING_GOING_DOWN;
        }
        stop_time_left = 2;
    }

    void update() {
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
    }

    ElevatorState get_state() const { return state; }    
    int get_num_passengers() const { return num_passengers; }

   private:
    ElevatorState state;
    int currentFloorNumber;
    int stop_time_left;
    int moving_time_left;
    vector<Passenger> passengers;
    int num_passengers;
    const int MAX_PASSENGERS;
    const int MAX_FLOORS;
    const int DEFAULT_START_FLOOR = 1;
};


class Building {
public:        
    Building();
    Building(const int num_elevators, const int num_floors);
    inline vector<Elevator> getElevators() {return elevators;}
    
private:
    int numFloors;
    vector<Floor> floors;    
    vector<Elevator> elevators;
};

/// @brief Default parameterless constructor
Building::Building(){}

/// @brief Constructor for a Building. Instantiates a number of floors 
///     and elevators for the object. 
/// @param num_elevators 
/// @param num_floors 
Building::Building(const int num_elevators, const int num_floors){
    numFloors = num_floors;
    for (int i = 0; i < num_floors; i++) {
        floors.push_back(Floor(i + 1));            
    }        
    for (int i = 0; i < num_elevators; i++) {
        elevators.push_back(Elevator(num_elevators, num_floors));
    }
}

class RunSimulation {
   public:
    const int MAX_ELEVATORS = 4;
    const int MAX_PASSENGERS = 8;
    const int MAX_FLOORS = 100;
    const int TIME_BETWEEN_FLOORS = 10;
    
    RunSimulation() {
        building = Building(MAX_ELEVATORS, MAX_FLOORS);    
    }

    void add_passenger(Passenger passenger) { passengers.push_back(passenger); }

    void add_bulk_passenger(vector<Passenger> p) { passengers = p; }

    void update() {
        for (auto& elevator : building.getElevators()) {
            if (elevator.get_state() ==
                    Elevator::ElevatorState::STOPPED_GOING_UP ||
                elevator.get_state() ==
                    Elevator::ElevatorState::STOPPED_GOING_DOWN) {
                int min_dist = MAX_FLOORS;
                int closest_passenger_index = -1;
                for (int i = 0; i < passengers.size(); i++) {
                    Passenger& passenger = passengers[i];
                    if (passenger.get_start_floor() ==
                        elevator.getCurrentFloor()) {
                        if (passenger.get_start_time() <= current_time) {
                            int dist = abs(elevator.getCurrentFloor() 
                            - passenger.get_start_floor());
                            if (dist < min_dist) {
                                min_dist = dist;
                                closest_passenger_index = i;
                            }
                        }
                    }
                }
                if (closest_passenger_index != -1) {
                    elevator.add_passenger(passengers[closest_passenger_index]);                    
                    passengers.erase(passengers.begin() +
                                     closest_passenger_index);
                    elevator.stop();
                }
            }
            elevator.update();
        }
        current_time++;
    }

    int get_current_time() const { return current_time; }

   private:
    vector<Elevator> elevators;
    vector<Passenger> passengers; 
    Building building;   
    int current_time = 0;
};

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
    for (vector<string> v : content) {
        for (auto& element : v) {
            Passenger::Status initialStatus;
            if (element[1] > element[2]) {
                initialStatus = Passenger::WAITING_ELEVATOR_DOWN;
            } else if (element[1] < element[2]) {
                initialStatus = Passenger::WAITING_ELEVATOR_UP;
            } else {
                initialStatus =
                    Passenger::AT_DESTINATION;  // End floor and start floor are
                                                // the same for this passenger
            }
            passengers.push_back(
                Passenger(id, element[0], element[1], element[2], initialStatus));
            // cout<<element<<" ";
        }
        id++;
        cout << "\n";
    }

    return passengers;
}

int main() {
    RunSimulation sim;
    vector<Passenger> passengers = getPassengerData();
    sim.add_bulk_passenger(passengers);

    while (true) {
        sim.update();
        // TODO: check if all passengers have reached their destination and exit
        // loop if so
    }

    // TODO: calculate and print average wait time and average travel time
    return 0;
}
