#include "Passenger.h"

Passenger::Passenger(int id, std::shared_ptr<std::condition_variable> passengerCV, std::mutex *cout_mutex) {
	this->_id = id;
	this->_passengerCV = passengerCV;
	this->_coutMtx = cout_mutex;
}

void Passenger::operator()(std::vector<std::shared_ptr<Car>> &cars) {
	std::mutex mtx;
	std::unique_lock<std::mutex> lck(mtx);

	while(true) {
		std::shared_ptr<Car> enteredCar = nullptr;

		// Wait until a car calls load
		_passengerCV->wait(lck);

		// Try to find a space in a car
		for (auto &car : cars) {
			if (car->enterCar(this->_id)) {
				enteredCar = car;
				break;
			}
		}
		// There was no space available
		if (!enteredCar) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			continue;
		}
		// Running on rollercoaster and waiting until car calls unload
		while(enteredCar->getStatus() != CarStatus::Unloading) enteredCar->boardedPassengersCV.wait(lck);

		enteredCar->leaveCar(_id);

	}
	_coutMtx->lock();
	std::cout << "Passenger " << _id << " left the park" << "\n";
	_coutMtx->unlock();

}