#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <string>

using namespace std;


class Passenger {
public:
    enum Status
    {   
        NOT_SET, 
        WAITING_ELEVATOR_UP,
        WAITING_ELEVATOR_DOWN,
        RIDING_ELEVATOR,
        AT_DESTINATION
    };
    Passenger(int start_time, int start_floor, int end_floor, Status status) :
        start_time(start_time), start_floor(start_floor), end_floor(end_floor), status(status) {}

    int get_start_time() const { return start_time; }
    int get_start_floor() const { return start_floor; }
    int get_end_floor() const { return end_floor; }

private:
    int start_time;
    int start_floor;
    int end_floor;
    Status status = NOT_SET;
    // Determines passenger status  
};

class Floor{    
    public:
        Floor();
        inline void incrementFloorNumber();
        inline void decrementFloorNumber();
        inline int getFloorNumber() const;
        inline bool operator<(const Floor& floor);
        inline bool operator>(const Floor& floor);
        inline bool operator==(const Floor& floor);
        inline bool operator<(const int value);
        inline bool operator>(const int value);
        inline bool operator==(const int value);
        static const int STARTING_FLOOR_NUMBER = 1;
    private:
        int floorNumber;
};

Floor::Floor(){
    this->floorNumber = Floor::STARTING_FLOOR_NUMBER;
}

inline void Floor::incrementFloorNumber() { 
   this->floorNumber++; 
}

inline void Floor::decrementFloorNumber() { 
   this->floorNumber--; 
}



inline int Floor::getFloorNumber() const {
    return this->floorNumber;
}

inline bool Floor::operator<(const Floor& floor) 
{
   return floor.floorNumber < this->floorNumber;
}

inline bool Floor::operator>(const Floor& floor) 
{
   return floor.floorNumber > this->floorNumber;
}

inline bool Floor::operator==(const Floor& floor) 
{
   return floor.floorNumber == this->floorNumber;
}

inline bool Floor::operator<(const int value) 
{
   return value < this->floorNumber;
}

inline bool Floor::operator>(const int value) 
{
   return value > this->floorNumber;
}

inline bool Floor::operator==(const int value) 
{
   return value == this->floorNumber;
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

    
    Elevator(const int max_passengers, const int max_floors) : state(STOPPED_NO_PASSENGERS), current_floor(), stop_time_left(0), num_passengers(0),
     MAX_PASSENGERS(max_passengers), MAX_FLOORS(max_floors) {}

    void add_passenger(Passenger passenger) {
        if (num_passengers < MAX_PASSENGERS) {
            passengers.push_back(passenger);
            num_passengers++;
        }
    }

    //TODO: should take some period of time to move between floors
    // 10 seconds in the original problem statement and then 
    // 5 seconds for the modified version. 
    //Doesn't seem like this is currently being handled anywhere yet
    void move_up() {
        state = MOVING_UP;
        current_floor.incrementFloorNumber();
        for (auto it = passengers.begin(); it != passengers.end(); ) {
            if (it->get_end_floor() == current_floor.getFloorNumber()) {
                it = passengers.erase(it);
                num_passengers--;
                it->AT_DESTINATION;
            } else {
                ++it;
            }
        }
    }

    void move_down() {
        state = MOVING_DOWN;
        current_floor.decrementFloorNumber();
        for (auto it = passengers.begin(); it != passengers.end(); ) {
            if (it->get_end_floor() == current_floor.getFloorNumber()) {
                it = passengers.erase(it);
                num_passengers--;
                it->AT_DESTINATION;
            } else {
                ++it;
            }
        }
    }

    void stop() {
        if(this->MOVING_UP){
            state = STOPPING_GOING_UP;
        }
        else{
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
        } 
        else if(state == STOPPING_GOING_DOWN){
            stop_time_left--;
            if (stop_time_left == 0) {
                state = STOPPED_GOING_DOWN;
            }
        }
        else if (state == MOVING_UP) {
            if(current_floor.getFloorNumber() == MAX_FLOORS) {
                stop();
            } else {
                move_up();
            }
        } else if (state == MOVING_DOWN) {
            if (current_floor.getFloorNumber() == 1) {
                stop();
            } else {
                move_down();
            }
        }
    }

    ElevatorState get_state() const { return state; }
    Floor get_current_floor() const { return current_floor; }
    int get_num_passengers() const { return num_passengers; }

private:
    ElevatorState state;
    Floor current_floor;
    int stop_time_left;
    vector<Passenger> passengers;
    int num_passengers;
    const int MAX_PASSENGERS;
    const int MAX_FLOORS;
};



class RunSimulation {
public:
    const int MAX_ELEVATORS = 4;
    const int MAX_PASSENGERS = 8;
    const int MAX_FLOORS = 100;
    const int TIME_BETWEEN_FLOORS = 10;
    RunSimulation() {
        for (int i = 0; i < MAX_ELEVATORS; i++) {
            elevators.push_back(Elevator(MAX_PASSENGERS, MAX_FLOORS));
        }
    }

    void add_passenger(Passenger passenger) {
        passengers.push_back(passenger);
    }

    void add_bulk_passenger(vector<Passenger> p) {
        passengers = p;
    }

    void update() {
        for (auto& elevator : elevators) {
            if (elevator.get_state() == Elevator::ElevatorState::STOPPED_GOING_UP 
                || elevator.get_state() == Elevator::ElevatorState::STOPPED_GOING_DOWN) {
                int min_dist = MAX_FLOORS;
                int closest_passenger_index = -1;
                for (int i = 0; i < passengers.size(); i++) {
                    Passenger& passenger = passengers[i];
                    if (passenger.get_start_floor() == elevator.get_current_floor().getFloorNumber()) {
                        if (passenger.get_start_time() <= current_time) {
                            int dist = abs(elevator.get_current_floor().getFloorNumber() - passenger.get_start_floor());
                            if (dist < min_dist) {
                                min_dist = dist;
                                closest_passenger_index = i;
                            }
                        }
                    }
                }
                if (closest_passenger_index != -1) {
                    elevator.add_passenger(passengers[closest_passenger_index]);
                     //TODO/Reminder to revist this line. Should we be erasing passengers from this list? Isn't this kind of our master list of passengers?
                    passengers.erase(passengers.begin() + closest_passenger_index);  
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
    int current_time = 0;
};



vector<Passenger> getPassengerData() {

    vector<Passenger> passengers;

    string fname = "Mod10_Assignment_Elevators.csv";
 
	vector<vector<string>> content;
	vector<string> row;
	string line, word;
 
	fstream file (fname, ios::in);
	if(file.is_open())
	{
		while(getline(file, line))
		{
			row.clear();
 
			stringstream str(line);
 
			while(getline(str, word, ','))
				row.push_back(word);
			content.push_back(row);
		}
	}
	else
		cout<<"Could not open the file\n";
 
	for(vector<string> v : content)
	{
		for(auto & element: v )
		{
            Passenger::Status initialStatus;
            if(element[1] > element[2]){
                initialStatus = Passenger::WAITING_ELEVATOR_DOWN;
            }
            else if(element[1] < element[2]){
                initialStatus = Passenger::WAITING_ELEVATOR_UP;
            }
            else{
                initialStatus = Passenger::AT_DESTINATION;  //End floor and start floor are the same
            }
            passengers.push_back(Passenger(element[0],element[1],element[2], initialStatus));
			//cout<<element<<" ";
		}
		cout<<"\n";
	}

    return passengers;
}

int main() {
    RunSimulation sim;    
    vector<Passenger> passengers = getPassengerData();
    sim.add_bulk_passenger(passengers);

    while (true) {
        sim.update();
        // TODO: check if all passengers have reached their destination and exit loop if so        
    }

    // TODO: calculate and print average wait time and average travel time
    return 0;
}
