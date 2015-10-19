#ifndef GENERATOR_H_
#define GENERATOR_H_
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version. (See cxx source for full Copyright notice)
 */

#include <Pythia8/Pythia.h>
#include <array>
#include <random>

class Generator {
public:
	enum Parton_t {
		kGluon  	= 21,
		kUquark  	= 1,
		kDquark		= 2,
		kSquark 	= 3,
		kCquark		= 4,
		kBquark		= 5
	};

	Generator();
	Generator(Parton_t parton);
	virtual ~Generator();

	void Init();
	void Generate();
	const Pythia8::Event			&GetEvent() const;

	void SetParton(Parton_t parton) { fParton = parton; }
	void SetPtLimits(double mine, double maxe) { fPtLimits[0] = mine; fPtLimits[1] = maxe; }

	void SetPythiaSeed(unsigned long randomseed);
	void SetPartonRandomSeed(unsigned long randomseed);

private:
	Pythia8::Pythia									fPythia;						/// Pythia engine

	std::array<double, 2>							fPtLimits;						/// Pt limits
	Parton_t										fParton;						/// Parton type

	std::default_random_engine						fRandomEngine;					/// Random engine for parton pt
	std::uniform_real_distribution<double>			fRandomDistribution;			/// Random distribution for parton pt
};

#endif
