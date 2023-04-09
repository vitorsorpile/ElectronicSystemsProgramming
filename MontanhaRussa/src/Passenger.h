#ifndef __PASSENGER__
#define __PASSENGER__

#include <iostream>
#include <thread>
#include <mutex>
#include <memory>
#include <condition_variable>
#include <vector>
#include "Car.h"

class Car;

class Passenger
{
private:
	int _id;
	// std::shared_ptr<std::mutex> _carMtx;
	// unsigned int *_boardedPassengers = nullptr;
	std::shared_ptr<std::condition_variable> _passengerCV;

public:
	Passenger(int, std::shared_ptr<std::condition_variable>);

	void operator()(std::vector<Car> &cars);
};

#endif