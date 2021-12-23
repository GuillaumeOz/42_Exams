#include "Warlock.hpp"


Warlock::Warlock(std::string name, std::string title) {

	this->name = name;
	this->title = title;
	std::cout << this->name << ": This looks like another boring day.\n";
}

Warlock::~Warlock() {

	std::cout << this->name << ": My job here is done!\n";

}

std::string Warlock::getName() const {

	return (this->name);
}


std::string Warlock::getTitle() const {

	return (this->title);
}

void	Warlock::setTitle(std::string title) {

	this->title = title;
}

void	Warlock::introduce() const {

	
	std::cout << this->name << ": I am " << this->name << ", " << this->title << "!\n";
}

