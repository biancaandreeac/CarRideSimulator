// Copyright 2019 Bianca Ciuche
#ifndef DRIVER_H_
#define DRIVER_H_

#include <list>
#include <iterator>
#include <vector>
#include <string>

class Driver {
 public:
	std::string name;
	std::string location;
	float rating;
	int rides;
	int dist;
	bool on;

	Driver();
	Driver(std::string name, std::string location);
	void offline();
	void online();
	void add_rating(int rate);
};

Driver::Driver() {
	name = "\0";
	location = "\0";
	rating = 0;
	rides = 0;
	dist = 0;
	on = false;;
}

Driver::Driver(std::string name, std::string location) {
	this->name = name;
	this->location = location;
	rating = 0;
	rides = 0;
	dist = 0;
	on = true;
}

void Driver::offline() {
	on = false;
}

void Driver::online() {
	on = true;
}

void Driver::add_rating(int rate) {
	rating *= rides;
	rating += rate;
	rating /= (++rides);
}

#endif  // DRIVER_H_
