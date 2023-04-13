#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

const int MAX_ELEVATORS = 4;
const int MAX_PASSENGERS = 8;
const int MAX_FLOORS = 100;

enum ElevatorState {
    STOPPED,
    STOPPING,
    MOVING_UP,
    MOVING_DOWN
};

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

class Elevator {
public:
    Elevator() : state(STOPPED), current_floor(1), num_passengers(0) {}

    void add_passenger(Passenger passenger) {
        if (num_passengers < MAX_PASSENGERS) {
            passengers.push_back(passenger);
            num_passengers++;
        }
    }

    void move_up() {
        state = MOVING_UP;
        current_floor++;
        for (auto it = passengers.begin(); it != passengers.end(); ) {
            if (it->get_end_floor() == current_floor) {
                it = passengers.erase(it);
                num_passengers--;
            } else {
                ++it;
            }
        }
    }

    void move_down() {
        state = MOVING_DOWN;
        current_floor--;
        for (auto it = passengers.begin(); it != passengers.end(); ) {
            if (it->get_end_floor() == current_floor) {
                it = passengers.erase(it);
                num_passengers--;
            } else {
                ++it;
            }
        }
    }

    void stop() {
        state = STOPPING;
        stop_time_left = 2;
    }

    void update() {
        if (state == STOPPING) {
            stop_time_left--;
            if (stop_time_left == 0) {
                state = STOPPED;
            }
        } else if (state == MOVING_UP) {
            if (current_floor == MAX_FLOORS) {
                stop();
            } else {
                move_up();
            }
        } else if (state == MOVING_DOWN) {
            if (current_floor == 1) {
                stop();
            } else {
                move_down();
            }
        }
    }

    ElevatorState get_state() const { return state; }
    int get_current_floor() const { return current_floor; }
    int get_num_passengers() const { return num_passengers; }

private:
    ElevatorState state;
    int current_floor;
    int stop_time_left;
    vector<Passenger> passengers;
    int num_passengers;
};

class Floors {
public:
    Floors() {
        for (int i = 0; i < MAX_ELEVATORS; i++) {
            elevators.push_back(Elevator());
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
            if (elevator.get_state() == STOPPED) {
                int min_dist = MAX_FLOORS;
                int closest_passenger_index = -1;
                for (int i = 0; i < passengers.size(); i++) {
                    Passenger& passenger = passengers[i];
                    if (passenger.get_start_floor() == elevator.get_current_floor()) {
                        if (passenger.get_start_time() <= current_time) {
                            int dist = abs(elevator.get_current_floor() - passenger.get_start_floor());
                            if (dist < min_dist) {
                                min_dist = dist;
                                closest_passenger_index = i;
                            }
                        }
                    }
                }
                if (closest_passenger_index != -1) {
                    elevator.add_passenger(passengers[closest_passenger_index]);
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
    Floors f;    
    vector<Passenger> passengers = getPassengerData();
    f.add_bulk_passenger(passengers);

    while (true) {
        f.update();
        // TODO: check if all passengers have reached their destination and exit loop if so        
    }

    // TODO: calculate and print average wait time and average travel time
    return 0;
}
