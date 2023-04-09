#include "Car.h"

Car::Car(int id){
	this->_id = id;
	this->_boardedPassengers = 0;
	this->car_cv = new std::condition_variable;

}

Car::Car(const Car &c) {
	this->_id = c._id;
	this->_boardedPassengers = c._boardedPassengers;
	this->car_cv = c.car_cv;
	//this->_carMtx = c._carMtx;
}

void Car::operator()(std::shared_ptr<std::condition_variable> passengersCV) {
	std::mutex mtx;
	std::unique_lock<std::mutex> lck(mtx);
	
	std::cout << std::this_thread::get_id() << " - Car " << _id << " - " << &car_cv << "\n";
	// std::cout << "[functor] Car " << _id << " - " << this << "\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	while(true) {
		
		// Wait passengers board - load()
		_status = CarStatus::Loading;
		passengersCV->notify_all();
		std::cout << std::this_thread::get_id() << " - Car " << _id << " - " << _boardedPassengers << "\n";

		while(_status != CarStatus::Running) {
			// std::cout << std::this_thread::get_id() << " - " << this << " - " << _id << " -> " << _boardedPassengers << std::endl;
			car_cv->wait(lck);
		}
		// std::cout << "Starting ride for Car " << _id << "\n";

		// Ride - run()
		std::cout << "Car " << _id << "tried to get _carMtx" << "\n";
		_carMtx.lock();
		_status = CarStatus::Running;
		std::cout << "Starting ride for Car " << _id << " - " << _boardedPassengers << " passengers on board" << "\n";
		std::this_thread::sleep_for(std::chrono::seconds(5));
		_status = CarStatus::Unloading;
		std::cout << "Finished ride of Car " << _id << " - " << _boardedPassengers << " passengers on board" << "\n";
		_carMtx.unlock();
		boardedPassengersCV.notify_all();

		// Wait passengers unload - unload()
		while(_boardedPassengers != 0) {
			car_cv->wait(lck);
		}


	}
}

unsigned int Car::getBoardedPassengers() {
	return _boardedPassengers;
}
	
bool Car::enterCar(int passengerID) {
	
	_carMtx.lock();

	if (_boardedPassengers == MAX_PASSENGERS) {
		std::cout << std::this_thread::get_id() << " - " << this << " - There was no space for Passenger " << passengerID << " on Car " << _id << "\n";
		_carMtx.unlock();
		return false;
	}
	else {
		std::cout << std::this_thread::get_id() << " - " << this << " - Passenger " << passengerID << " entered Car " << _id << "\n";
		_boardedPassengers++;
		if (_boardedPassengers == MAX_PASSENGERS) {
			_status = CarStatus::Running;
		}
		_carMtx.unlock();
		return true;
	}
}

unsigned int Car::leaveCar(int passengerID) {
	_carMtx.lock();
	_boardedPassengers--;
	_carMtx.unlock();
	return _boardedPassengers;
}

bool Car::isFull() {
	return _boardedPassengers == MAX_PASSENGERS;
}

int Car::getID() {
	return _id;
}

CarStatus Car::getStatus() {
	return _status;
}