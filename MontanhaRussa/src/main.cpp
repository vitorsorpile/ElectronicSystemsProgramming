#include "Car.h"
#include "Passenger.h"

int main(int argc, char* argv[]) {
	const unsigned int NUM_OF_CARS = 2;
	const unsigned int NUM_OF_PASSENGERS = 9;
	
	unsigned int boardedPassengers = 0;
	std::mutex cout_mutex;
	std::shared_ptr<std::mutex> mtx (new std::mutex);
	std::shared_ptr<std::condition_variable> passengerCV (new std::condition_variable);

	std::vector<std::shared_ptr<Car>> cars;

	std::vector<std::thread> threads;
	for (int i = 0; i < NUM_OF_CARS; i++) {

		
		cars.push_back(std::shared_ptr<Car> (new Car(i, &cout_mutex) ));
		// (*c)(passengerCV);
		// std::cout << "[Main] Car " << cars[i].getID() << " - " << &(cars[i]) <<"\n" ;

		threads.push_back(std::thread(std::ref(*cars[i]), std::ref(passengerCV)));

	}	

	for (int i = 0; i < NUM_OF_PASSENGERS; i++) {
		Passenger p(i, passengerCV, &cout_mutex);
		threads.push_back(std::thread(p, std::ref(cars)));
	}

	
	for (auto & th: threads) {
		if (th.joinable()) th.join();
	}
	// Passenger pass1(1, passengerCV);
	// pass1(cars);

	// for (auto & car: cars) {
	// 	delete car;
	// }
}