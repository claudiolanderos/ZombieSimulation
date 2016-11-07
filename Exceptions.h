#pragma once
#include <exception>

class InvalidOp : public std::exception
{
public:
	const char* what() const noexcept override { return "Invalid OP code or parameters."; }
};

class InvalidFile : public std::exception
{
public:
    const char* what() const noexcept override { return "Invalid zom file."; }
};

class SimulationException : public std::exception
{
public:
    const char* what() const noexcept override { return "Something went wrong in the simulation."; }
};
