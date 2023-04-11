#include "Car.h"

Car::Car(int id, std::mutex *cout_mutex){
	this->_id = id;
	this->_boardedPassengers = 0;
	// this->car_cv = new std::condition_variable;
	this->_coutMtx = cout_mutex;

	_coutMtx->lock();
	std::cout << " Criei um carrito " << _id << "\n";
	_coutMtx->unlock();

}

Car::Car(const Car &c) {
	this->_id = c._id;
	this->_boardedPassengers = c._boardedPassengers;
	this->_coutMtx = c._coutMtx;

	_coutMtx->lock();
	std::cout << " Criei uma copia de um carrito " << _id << "\n";
	_coutMtx->unlock();
}

void Car::operator()(std::shared_ptr<std::condition_variable> passengersCV) {
	std::mutex mtx;
	std::unique_lock<std::mutex> lck(mtx, std::defer_lock);
	
	// _coutMtx->lock();
	// std::cout << std::this_thread::get_id() << " - Car " << _id << " - " << &car_cv << "\n";
	// std::cout << "[functor] Car " << _id << " - " << this << "\n";
	// _coutMtx->unlock();
	// std::this_thread::sleep_for(std::chrono::milliseconds(200));
	while(true) {
		
		// Wait passengers board - load()
		_status = CarStatus::Loading;
		passengersCV->notify_all();
		// _coutMtx->lock();
		// std::cout << std::this_thread::get_id() << " - Car " << _id << " - " << _boardedPassengers << "\n";
		// _coutMtx->unlock();

		while(_status != CarStatus::Running) {
			// std::cout << std::this_thread::get_id() << " - " << this << " - " << _id << " -> " << _boardedPassengers << std::endl;
			car_cv.wait(lck);
		}
		// std::cout << "Starting ride for Car " << _id << "\n";

		// Ride - run()
		// _coutMtx->lock();
		// std::cout << "Car " << _id << " tried to get _carMtx" << "\n";
		// _coutMtx->unlock();

		_carMtx.lock();
		_status = CarStatus::Running;

		_coutMtx->lock();
		std::cout << "Starting ride for Car " << _id << " - " << _boardedPassengers << " passengers on board" << "\n";
		_coutMtx->unlock();

		std::this_thread::sleep_for(std::chrono::seconds(5));
		_status = CarStatus::Unloading;

		_coutMtx->lock();
		std::cout << "Finished ride of Car " << _id << " - " << _boardedPassengers << " passengers on board" << "\n";
		_coutMtx->unlock();

		_carMtx.unlock();
		boardedPassengersCV.notify_all();

		// Wait passengers unload - unload()
		while(_boardedPassengers != 0) {
			car_cv.wait(lck);
		}


	}
}

unsigned int Car::getBoardedPassengers() {
	return _boardedPassengers;
}
	
bool Car::enterCar(int passengerID) {
	
	_carMtx.lock();

	if (_boardedPassengers == MAX_PASSENGERS) {
		_coutMtx->lock();
		std::cout << std::this_thread::get_id() << " - " << this << " - There was no space for Passenger " << passengerID << " on Car " << _id << "\n";
		_coutMtx->unlock();

		_carMtx.unlock();
		return false;
	}
	else {
		_coutMtx->lock();
		std::cout << std::this_thread::get_id() << " - " << this << " - Passenger " << passengerID << " entered Car " << _id << "\n";
		_coutMtx->unlock();

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