#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

/// @brief Passenger Class. Represents the passengers and contains a few more
/// additional attributes for analysis between the different move times of
/// the elevators
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
    inline int get_passenger_ID() { return this->passenger_ID; }
    inline int get_time_picked_up() { return this->time_picked_up; }
    inline int get_time_dropped_off() { return this->time_dropped_off; }
    inline void set_time_picked_up(int currentTime) {
        this->time_picked_up = currentTime;
    }
    inline void set_time_dropped_off(int currentTime) {
        this->time_dropped_off = currentTime;
    }

    /// @brief method to sort passenders in a vector
    /// @param p1 passenger1 Passenger class
    /// @param p2 passenger2 Passenger class
    /// @return Is P1 start < P2
    static bool comparePassengersByStartTime(Passenger p1, Passenger p2) {
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

/// @brief Floor class to represent the floor objects and associated methods
/// for getting and setting attributes and holding passengers
class Floor {
   public:
    Floor(int floorNum) : floorNumber(floorNum) {}
    enum CallState { NONE, UP, DOWN, BOTH };
    const int floorNumber;
    vector<Passenger> passengersGoingUp;
    vector<Passenger> passengersGoingDown;
    void queuePassengerGoingUp(Passenger passenger);
    void queuePassengerGoingDown(Passenger passenger);
    void SetCallStateForFloor();
    CallState isStopRequested();

   private:
    CallState callState = NONE;
};

// class declaration for compiler
class Building;

/// @brief Elevator Objecct. Can contain passengers and will update its own
/// movement with the update() function
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

    Elevator(const int max_passengers, const int max_floors,
             const int time_between_floors)
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
/// @brief Building class is the shell encompassing the floor, elevator and 
/// passenger classes. it will act as the controller for the simulation
/// called by the runsimulation class
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
    int totalTimeTravelled;
    int totalWaitTime;

    void setCurrentTime(int timeInSeconds);
    void updateFloorCallStatus();
    void getFloorsAndCallStatuses();
    void updateElevatorMovement();
    bool doesFloorHavePassengersGoingUp(int floorNum);
    bool doesFloorHavePassengersGoingDown(int floorNum);
    void markPassengersAtDestination(Passenger passenger);
    void addTimeTravelled(Passenger passenger);
    void addWaitTime(Passenger passenger);

    // this function is called for debugging purposes and can be deleted
    void debugGetNumberOfFloorsWithQueuedPassengers();
};

/// @brief Driver Shell code for the simulation
class RunSimulation {
   public:
    int MAX_ELEVATORS = 4;
    int MAX_PASSENGERS = 8;
    int MAX_FLOORS = 100;
    int TIME_BETWEEN_FLOORS;
    int current_time = 0;
    Building building;

    RunSimulation(int timeBetweenFloors);
    void generateBuilding();
    void iterateOneSecond();
    void addPassengersToBuilding(vector<Passenger>);
};

/// @brief queue passenger into the floor queue to go up
/// @param passenger passenger1 Passenger class
void Floor::queuePassengerGoingUp(Passenger passenger) {
    passengersGoingUp.push_back(passenger);
    if (callState == NONE) {
        callState = UP;
    } else if (callState == DOWN) {
        callState = BOTH;
    }
}
/// @brief queue passenger into the floor queue to go down
/// @param passenger passenger1 Passenger class
void Floor::queuePassengerGoingDown(Passenger passenger) {
    passengersGoingDown.push_back(passenger);
    if (callState == NONE) {
        callState = DOWN;
    } else if (callState == UP) {
        callState = BOTH;
    }
}
/// @brief method to return the floor callstate
Floor::CallState Floor::isStopRequested() { return callState; };

/// @brief method for the floor to set its callstate flag
void Floor::SetCallStateForFloor() {
    if (passengersGoingUp.size() == 0 && passengersGoingDown.size() == 0) {
        callState = NONE;
    } else if (passengersGoingUp.size() > 0 &&
               passengersGoingDown.size() == 0) {
        callState = UP;
    } else if (passengersGoingUp.size() == 0 &&
               passengersGoingDown.size() > 0) {
        callState = DOWN;
    } else {
        callState = BOTH;
    }
}

/// @brief add passsenger to elevator object
/// @param passenger passenger object
void Elevator::add_passenger(Passenger passenger) {
    if (num_passengers < MAX_PASSENGERS) {
        passengers.push_back(passenger);
        num_passengers++;
    }
}

/// @brief initiate elevator ascending one floor
void Elevator::move_up() {
    currentFloorNumber++;
    moving_time_left = TIME_BETWEEN_FLOORS;
}

/// @brief initiate elevator descending one floor
void Elevator::move_down() {
    currentFloorNumber--;
    moving_time_left = TIME_BETWEEN_FLOORS;
}

/// @brief initiate elevator stop procedure. Takes 2 seconds or two sim cycles
void Elevator::stop() {
    // TODO: Handling for if they are at the max or min floor? Move to
    // STOPPED_NO_PASSENGERS?
    if ((state == MOVING_UP && currentFloorNumber != MAX_FLOORS) ||
        currentFloorNumber == 1) {
        state = STOPPING_GOING_UP;
    } else {
        state = STOPPING_GOING_DOWN;
    }
    stop_time_left = 2;
    moving_time_left =
        TIME_BETWEEN_FLOORS;  // reset moving_time_left for the next move
}

/// @brief Elevator to update its own actions and update coordinating states
/// @param building building object
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
            // you just incremented the floor number, check if you should stop
            // here:
            // 1. You have a passenger that wants to get off here
            // 2. The current floor has a passenger waiting pickup to go up
            // 3. you're at the max floor
            // if yes to any of the above, stop
            if (hasPassengerOnCurrentFloor() ||
                (building.doesFloorHavePassengersGoingUp(currentFloorNumber) &&
                 passengers.size() < MAX_PASSENGERS) ||
                currentFloorNumber == MAX_FLOORS) {
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
            // you just decremented the floor number check if you should stop
            // here:
            // 1. You have a passenger that wants to get off here
            // 2. The current floor has a passenger waiting pickup to go down
            // 3. you're at the minimum floor
            // if yes to any of the above move, stop
            if (hasPassengerOnCurrentFloor() ||
                (building.doesFloorHavePassengersGoingDown(
                     currentFloorNumber) &&
                 passengers.size() < MAX_PASSENGERS) ||
                currentFloorNumber == MAX_FLOORS) {
                stop();
            }
        }
    } else if (state == STOPPED_NO_PASSENGERS) {
        // call function to figure out where the closest passenger is and go
        // that way
        if (building.floorsRequestingPickup.size() == 0) {
            // Do nothing, take a nap. There are no passengers to fetch.
        } else {
            int closestDistance = 999;  // arbitrarily large
            int closestFloor = -1;
            for (int floor : building.floorsRequestingPickup) {
                auto diff = std::abs(currentFloorNumber - floor);
                if (diff < closestDistance) {
                    closestDistance = diff;
                    closestFloor = floor;
                }
            }
            // You're already there somehow, pick them up immediately
            if (closestFloor == currentFloorNumber) {
                loadPassengers(building);
                // this is a weird case, figure out which direction to go now:
                if (passengers.size() > 0 &&
                    passengers.at(0).get_start_floor() <
                        passengers.at(0).get_end_floor()) {
                    state = MOVING_DOWN;
                } else {
                    state = MOVING_UP;
                }
            }
            if (closestFloor > currentFloorNumber) {
                state = MOVING_UP;
            } else {
                state = MOVING_DOWN;
            }
        }
    } else if (state == STOPPED_GOING_DOWN) {
        unloadPassengers(building);
        loadPassengers(building);
        // resume moving down if you still have passengers
        if (passengers.size() > 0) {
            state = MOVING_DOWN;
        }
        // otherwise move to STOPPED_NO_PASSENGERS state
        else {
            state = STOPPED_NO_PASSENGERS;
        }
    } else if (state == STOPPED_GOING_UP) {
        unloadPassengers(building);
        loadPassengers(building);
        // resume moving up if you still have passengers
        if (passengers.size() > 0) {
            state = MOVING_UP;
        }
        // otherwise move to STOPPED_NO_PASSENGERS state
        else {
            state = STOPPED_NO_PASSENGERS;
        }
    }
}

/// @brief Load passengers if there is room on the elevator
/// @param building building object
void Elevator::loadPassengers(Building &building) {
    // Get the Floor object from the Building singleton for the current floor
    // the elevator is on
    auto &currFloor = building.floors.at(currentFloorNumber - 1);
    if (state == STOPPED_GOING_DOWN ||
        (state == STOPPED_NO_PASSENGERS &&
         currFloor.passengersGoingDown.size() > 0)) {
        // while elevator is not full and Floor has passengers waiting to queue
        while (currFloor.passengersGoingDown.size() > 0 &&
               this->num_passengers < MAX_PASSENGERS) {
            Passenger passengerToAdd = currFloor.passengersGoingDown.front();
            currFloor.passengersGoingDown.erase(
                currFloor.passengersGoingDown.begin());
            passengerToAdd.set_time_picked_up(building.current_time);
            passengers.push_back(passengerToAdd);
            num_passengers++;
        }
    } else {  // else, elevator is going up, get the passengers going up
        while (currFloor.passengersGoingUp.size() > 0 &&
               this->num_passengers < MAX_PASSENGERS) {
            Passenger passengerToAdd = currFloor.passengersGoingUp.front();
            currFloor.passengersGoingUp.erase(
                currFloor.passengersGoingUp.begin());
            passengerToAdd.set_time_picked_up(building.current_time);
            passengers.push_back(passengerToAdd);
            num_passengers++;
        }
    }

    if (currFloor.passengersGoingDown.size() == 0 &&
        currFloor.passengersGoingUp.size() == 0) {
        // if no passengers are awaiting pickup on this floor, remove floor
        // number from floorsRequestingPickup
        building.floorsRequestingPickup.erase(currentFloorNumber);
    }
    currFloor.SetCallStateForFloor();
}

/// @brief elevator unloads passengers that are at their desired floor
/// @param building building object
void Elevator::unloadPassengers(Building &building) {
    int passengersBeforeUnloading = passengers.size();
    passengers.erase(
        std::remove_if(passengers.begin(), passengers.end(),
                       [this, &building](Passenger &passenger) {
                           if (passenger.get_end_floor() ==
                               currentFloorNumber) {
                               building.markPassengersAtDestination(passenger);

                               return true;
                           }
                           return false;
                       }),
        passengers.end());

    num_passengers = passengers.size();
    // TODO update some master list that says which passengers made it to their
    // destination
    // TODO update the passenger time when they got off the elevator for metrics
}

/// @brief elevator compares current floor to its passengers to determine
/// if there is a passenger who wants to get off on current floor
bool Elevator::hasPassengerOnCurrentFloor() {
    for (const auto &passenger : passengers) {
        if (passenger.get_end_floor() == currentFloorNumber) {
            return true;
        }
    }
    return false;
}

/// @brief Elevator return its state
Elevator::ElevatorState Elevator::get_state() const { return state; }

/// @brief empty building constructor
Building::Building() {}

/// @brief Building constructor called by RunSimulation, customizable for
/// various simulation parameters
/// @param num_elevators number of elevators for simulation
/// @param num_floors num floors for simulation
/// @param time_between_floors movement time of elevators between floors
Building::Building(int num_elevators, int num_floors, int time_between_floors) {
    for (int i = 0; i < num_floors; i++) {
        floors.push_back(Floor(i + 1));
    }
    for (int i = 0; i < num_elevators; i++) {
        elevators.push_back(
            Elevator(num_elevators, num_floors, time_between_floors));
    }
    totalTimeTravelled = 0;
    totalWaitTime = 0;
}

/// @brief set building time to sync with simulation time
/// @param t time in seconds
void Building::setCurrentTime(int t) { current_time = t; }


/// @brief Update the floor call status for all floors in building
/// this will move passengers from the notOnFloorYet queue to their
/// corresponding floors and then when the floors are checked for status
/// a status will return
void Building::updateFloorCallStatus() {
    auto it = passengersNotOnFloorsYet.begin();
    while (it != passengersNotOnFloorsYet.end()) {
        if (it->get_start_time() <= current_time) {
            if (it->get_start_floor() < it->get_end_floor()) {
                // going up
                floors.at(it->get_start_floor() - 1).queuePassengerGoingUp(*it);
                it = passengersNotOnFloorsYet.erase(it);
            } else if (it->get_start_floor() > it->get_end_floor()) {
                // going down
                floors.at(it->get_start_floor() - 1)
                    .queuePassengerGoingDown(*it);
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

/// @brief building method to scan floors and update attributes with
/// data for which floor and associated call status (up/down)
void Building::getFloorsAndCallStatuses() {
    for (Floor f : floors) {
        Floor::CallState c = f.isStopRequested();
        if (c != Floor::CallState::NONE) {
            floorsRequestingPickup.insert(f.floorNumber);
            floorsCallState.push_back(c);
        }
    }
}

/// @brief building method to update elevator movement for all elevators
void Building::updateElevatorMovement() {
    for (Elevator &e : elevators) {
        e.update(*this);
    }
}

/// @brief building method to check if floor has a passender going up
/// used in conjunction with elevator movement algorithm. Will pick up
/// passenger if going the same way
/// @param floorNum the floor number the elevator is currently at
bool Building::doesFloorHavePassengersGoingUp(int floorNum) {
    Floor relevantFloor = floors.at(floorNum - 1);
    if (relevantFloor.passengersGoingUp.size() > 0) {
        return true;
    }
    return false;
}

/// @brief building method to check if floor has a passender going down
/// used in conjunction with elevator movement algorithm. Will pick up
/// passenger if going the same way
/// @param floorNum the floor number the elevator is currently at
bool Building::doesFloorHavePassengersGoingDown(int floorNum) {
    for (int floor : floorsRequestingPickup) {
        if (floor == floorNum) {
            return true;
        }
    }
    return false;
}

/// @brief set passenger flag arrived
/// @param passenger passenger object
void Building::markPassengersAtDestination(Passenger passenger) {
    passengersAtDestination += 1;
    passenger.set_time_dropped_off(current_time);
    addTimeTravelled(passenger);
    addWaitTime(passenger);
}

/// @brief get passenger amount of time travelled and add it to total time
/// @param passenger passenger object
void Building::addTimeTravelled(Passenger passenger) {
    int time =
        passenger.get_time_dropped_off() - passenger.get_time_picked_up();
    totalTimeTravelled += time;
}

/// @brief get passenger amount of wait time and add it to total wait time
/// @param passenger passenger object
void Building::addWaitTime(Passenger passenger) {
    int time = passenger.get_time_picked_up() - passenger.get_start_time();
    totalWaitTime += time;
}

/// @brief method used for final debug stages. not for prduction use
void Building::debugGetNumberOfFloorsWithQueuedPassengers() {
    int count = 0;
    for (Floor floor : floors) {
        if (floor.passengersGoingDown.size() > 0 ||
            floor.passengersGoingUp.size() > 0) {
            count++;
        }
    }
    cout << "Actual number of floors with passengers waiting: " << count
         << endl;
}

/// @brief method to run the simulation
/// @param timeBetweenFloors used to update the travel time and allow for 
/// analysis on elevator speed
RunSimulation::RunSimulation(int timeBetweenFloors) {
    TIME_BETWEEN_FLOORS = timeBetweenFloors;
}

/// @brief method for a more descriptive way to generate a building for use
/// in the simulation
void RunSimulation::generateBuilding() {
    building = Building(MAX_ELEVATORS, MAX_FLOORS, TIME_BETWEEN_FLOORS);
}

/// @brief method to add the passengers from input into the simulation
/// @param p a vector of passenger objects returned from getPassengerData()
void RunSimulation::addPassengersToBuilding(vector<Passenger> p) {
    building.totalPassengers = p.size();
    building.passengersAtDestination = 0;
    building.fullPassengerList = p;
    building.passengersNotOnFloorsYet = p;
    std::sort(building.passengersNotOnFloorsYet.begin(),
              building.passengersNotOnFloorsYet.end(),
              Passenger::comparePassengersByStartTime);

    int countAlreadyAtDestination = std::count_if(
        building.fullPassengerList.begin(), building.fullPassengerList.end(),
        [](Passenger &passenger) {
            return passenger.get_status() == Passenger::AT_DESTINATION;
        });
    building.passengersAtDestination += countAlreadyAtDestination;
}

/// @brief method to control the flow of the simulation
void RunSimulation::iterateOneSecond() {
    building.setCurrentTime(current_time);
    building.updateFloorCallStatus();
    building.getFloorsAndCallStatuses();
    building.updateElevatorMovement();

    current_time++;
}

/// @brief method to get input from the associate input file (hard coded)
/// method will read in characters for each passenger, convert to type int
/// @return vector of passenger objects
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
                       [&](std::string s) { return stoi(s); });
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
        passengers.push_back(
            Passenger(id, ints[0], ints[1], ints[2], initialStatus));
        // std::cout << ints <<" ";
        id++;

        cout << "\n";
    }

    return passengers;
}

/// @brief main
int main() {
    // FIRST simulation ----- 10 seconds between each floor

    RunSimulation s(10);
    s.generateBuilding();
    vector<Passenger> passengers = getPassengerData();
    s.addPassengersToBuilding(passengers);
    cout << "Total number of passengers: " << s.building.totalPassengers
         << endl;
    cout << "Beginning simulation with 10 second travel times." << endl;
    while (s.building.passengersAtDestination < s.building.totalPassengers) {
        s.iterateOneSecond();
    }
    cout << "Number of passengers that reached their destination floor: "
         << s.building.passengersAtDestination << endl;
    cout << "Total number of seconds elapsed: " << s.building.current_time
         << endl;
    cout << "Total wait time for all passengers combined: "
         << s.building.totalWaitTime << endl;
    cout << "Total time spent travelling on an elevator for all passengers "
            "combined: "
         << s.building.totalTimeTravelled << endl;
    cout << "Average wait time for all passengers: "
         << s.building.totalWaitTime / s.building.totalPassengers << endl;
    cout << "Average time spent travelling on an elevator for all passengers "
            "combined: "
         << s.building.totalTimeTravelled / s.building.totalPassengers << endl;

    // SECOND simulation ----- 10 seconds between each floor

    RunSimulation s2(5);
    s2.generateBuilding();
    s2.addPassengersToBuilding(passengers);
    cout << "\n\n\n";
    cout << "Total number of passengers: " << s2.building.totalPassengers
         << endl;
    cout << "Beginning simulation with 5 second travel times." << endl;
    while (s2.building.passengersAtDestination < s2.building.totalPassengers) {
        s2.iterateOneSecond();
    }
    cout << "Number of passengers that reached their destination floor: "
         << s2.building.passengersAtDestination << endl;
    cout << "Total number of seconds elapsed: " << s2.building.current_time
         << endl;
    cout << "Total wait time for all passengers combined: "
         << s2.building.totalWaitTime << endl;
    cout << "Total time spent travelling on an elevator for all passengers "
            "combined: "
         << s2.building.totalTimeTravelled << endl;
    cout << "Average wait time for all passengers: "
         << s2.building.totalWaitTime / s.building.totalPassengers << endl;
    cout << "Average time spent travelling on an elevator for all passengers "
            "combined: "
         << s2.building.totalTimeTravelled / s.building.totalPassengers << endl;

    return 0;
}
