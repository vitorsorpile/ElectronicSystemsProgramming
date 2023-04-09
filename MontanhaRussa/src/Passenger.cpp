#include "Passenger.h"


Passenger::Passenger(int id, std::shared_ptr<std::condition_variable> passengerCV) {
	this->_id = id;
	this->_passengerCV = passengerCV;
}


void Passenger::operator()(std::vector<Car> &cars) {
	std::mutex mtx;
	std::unique_lock<std::mutex> lck(mtx);

	while(true) {
		Car *enteredCar = nullptr;
		
		// search for a space

		for (auto &car : cars) {
			if (car.getStatus() == CarStatus::Loading && car.enterCar(this->_id)) {
				
				enteredCar = &car;
				// std::cout << _id << " entered Car " << car.getID() << "\n";
				// last one to join a car
				// std::cout << "[Passenger] Car " <<car.getID() << " - " << (&car) << "\n";
				if (car.isFull()) {
					std::cout << "Notifying car " << car.getID() << " to start the ride. " << &(car.car_cv)<< "\n";
					car.car_cv->notify_one();
				}

				break;
			}
		}
		// there was no space available
		if (!enteredCar) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			continue;
		}
		// Running on rollercoaster
		while(enteredCar->getStatus() != CarStatus::Unloading) enteredCar->boardedPassengersCV.wait(lck);

		if (enteredCar->leaveCar(_id) == 0) {
			enteredCar->car_cv->notify_one();
		}

		// while(true) {}
	}
	std::cout << "Passenger " << _id << " left the park" << "\n";

}