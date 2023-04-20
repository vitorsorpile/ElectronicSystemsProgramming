#include "Car.h"

Car::Car(int id, std::mutex *cout_mutex){
	this->_id = id;
	this->_boardedPassengers = 0;
	this->_coutMtx = cout_mutex;
}

Car::Car(const Car &c) {
	this->_id = c._id;
	this->_boardedPassengers = c._boardedPassengers;
	this->_coutMtx = c._coutMtx;
}

void Car::operator()(std::shared_ptr<std::condition_variable> passengersCV) {
	while(true) {
		
		// Wait passengers board - load()
		this->load(passengersCV);

		/* Ride - run() */
		this->run();

		/* Wait passenger unboard - unload()*/
		_status = CarStatus::Unloading;
		this->unload();
	}
}


void Car::load(std::shared_ptr<std::condition_variable> passengersCV) {
	std::mutex mtx;
	std::unique_lock<std::mutex> lck(mtx, std::defer_lock);

	_status = CarStatus::Loading;

	while(_status != CarStatus::Running) {
		passengersCV->notify_all();
		car_cv.wait_for(lck, std::chrono::milliseconds(500));
	}
}

void Car::run() {
	_coutMtx->lock();
	std::cout << "Starting ride " << this->numberOfRuns <<" for Car " << _id << " - " << _boardedPassengers << " passengers on board" << "\n";
	_coutMtx->unlock();
	std::this_thread::sleep_for(std::chrono::seconds(5));
	this->numberOfRuns++;
}

void Car::unload() {
	std::mutex mtx;
	std::unique_lock<std::mutex> lck(mtx, std::defer_lock);

	_coutMtx->lock();
	std::cout << "Finished ride of Car " << _id << " - " << _boardedPassengers << " passengers on board" << "\n";
	_coutMtx->unlock();

	boardedPassengersCV.notify_all();

	while(_boardedPassengers != 0) {
		car_cv.wait(lck);
	}
}

unsigned int Car::getBoardedPassengers() {
	return _boardedPassengers;
}
	
bool Car::enterCar(int passengerID) {
	
	_carMtx.lock();

	if (_status != CarStatus::Loading) {
		_carMtx.unlock();
		return false;
	}
	else {
		_coutMtx->lock();
		std::cout << /*std::this_thread::get_id() << " - " << this << */" - Passenger " << passengerID << " entered Car " << _id << "\n";
		_coutMtx->unlock();

		_boardedPassengers++;
		if (_boardedPassengers == MAX_PASSENGERS) {
			_status = CarStatus::Running;
			_coutMtx->lock();
			std::cout << "Notifying car " << this->getID() << " to start the ride. " /*<< &(car)*/ << "\n";
			_coutMtx->unlock();
			this->car_cv.notify_one();
		}
		_carMtx.unlock();
		return true;
	}
}

bool Car::leaveCar(int passengerID) {

	if (_status != CarStatus::Unloading) return false;
	_carMtx.lock();
	_boardedPassengers--;
	_carMtx.unlock();

	if (_boardedPassengers == 0) {
		this->car_cv.notify_one();
	}
	return true;
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