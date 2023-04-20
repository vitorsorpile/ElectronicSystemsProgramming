#ifndef __CAR__
#define __CAR__

#include <iostream>
#include <thread>
#include <mutex>
#include <memory>
#include <condition_variable>
#include <chrono>
#include <vector>


enum class CarStatus {Loading, Unloading, Running};

class Car
{

private:
	const unsigned int MAX_PASSENGERS = 6;
	int _id;
	int numberOfRuns = 0;
	std::mutex _carMtx;
	std::mutex* _coutMtx;
	unsigned int _boardedPassengers = 0;
	CarStatus _status = CarStatus::Loading;

	void load(std::shared_ptr<std::condition_variable>);
	void run();
	void unload();

public:
	Car(int, std::mutex *);
	Car(const Car&);
	~Car() {/*delete car_cv;*/}
	std::condition_variable car_cv;
	std::condition_variable boardedPassengersCV;

	void operator()(std::shared_ptr<std::condition_variable> passengersCV);
	// void lock();
	// void unlock();
	unsigned int getBoardedPassengers();
	bool enterCar(int passengerID);
	bool leaveCar(int passengerID);
	bool isFull();
	int getID();
	CarStatus getStatus();
	

};

#endif