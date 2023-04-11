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
		
		// search for a space

		for (auto &car : cars) {
			if (car->getStatus() == CarStatus::Loading && car->enterCar(this->_id)) {
				
				enteredCar = car;
				// std::cout << _id << " entered Car " << car->getID() << "\n";
				// last one to join a car
				// std::cout << "[Passenger] Car " <<car->getID() << " - " << (&car) << "\n";
				if (car->isFull()) {
					_coutMtx->lock();
					std::cout << "Notifying car " << car->getID() << " to start the ride. " << &(car)<< "\n";
					_coutMtx->unlock();
					car->car_cv.notify_one();
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
			enteredCar->car_cv.notify_one();
		}

		// while(true) {}
	}
	_coutMtx->lock();
	std::cout << "Passenger " << _id << " left the park" << "\n";
	_coutMtx->unlock();

}