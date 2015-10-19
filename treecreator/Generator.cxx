/****************************************************************************
 * Analysis of electrons in jets 							                *
 * Copyright (C) 2015  Markus Fasel, Lawrence Berkeley National Laboratory  *
 *                                                                          *
 * This program is free software: you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * This program is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 ****************************************************************************/
#include "Generator.h"

#include <cfloat>
#include <cmath>

#include <sstream>

#include <Pythia8/Event.h>
#include <Pythia8/ParticleData.h>

#ifndef DBL_EPSILON
#define DBL_EPSILON 1e-9
#endif

/**
 * Constructor
 */
Generator::Generator() :
	fPythia(),
	fPtLimits(),
	fParton(kGluon),
	fRandomEngine(),
	fRandomDistribution()
{
	fPtLimits[0] = fPtLimits[1] = 0;
}

/**
 * Constructor, setting  parton type
 *
 * @param parton Type of the parton pair to generate
 */
Generator::Generator(Parton_t parton) :
	fPythia(),
	fPtLimits(),
	fParton(parton),
	fRandomEngine(),
	fRandomDistribution()
{
	fPtLimits[0] = fPtLimits[1] = 0;
}

/**
 * Destructor
 */
Generator::~Generator() {
}

void Generator::Init(){
	fPythia.readString("ProcessLevel:all = off");
	fPythia.readString("Next:numberShowInfo = 0");
	fPythia.readString("Next:numberShowProcess = 0");
	fPythia.readString("Next:numberShowEvent = 0");

	fPythia.init();
}

/**
 * Generates new qqbar/ppbar pair carrying the whole momentum in
 * x direction. It uses the Pythia function
 *
 *   append(PDG Code, Status, Color, Anitcolor, Px, Py, Pz, Et, Mass)
 *
 * Explanation of the settings:
 *
 *   - Status: 23 = outgoing
 *
 *   - Color codes:
 *       "To avoid confusion it is recommended that integer tags larger than
 *        MAXNUP (i.e. 500) are used. The actual value of the tag has no meaning
 *        beyond distinguishing the lines in a given process." (hep-ph/0109068)
 *     -> For Pythia statinh with 100 this means we start with 101 (+102 in
 *        case of gluone)
 *
 * The initial partons are then put into a full shower. The energy of the partons
 * is taken randomly from the energy range set from outside.
 */
void Generator::Generate(){
	// Generate and set qqbar / gg pair
	int color, anticolor;
	double 	pt,
			et,
			mass;
	if(std::abs(fPtLimits[0] - fPtLimits[1]) < DBL_EPSILON){
		pt = fPtLimits[0]
	} else {
		pt = fPtLimits[0] + (fPtLimits[1] - fPtLimits[0]) * fRandomDistribution();
	}
	if(fParton == kGluon){
		color = 101;
		anticolor = 102;
		et = pt;
		mass = 0;
	} else {
		color = 101;
		anticolor = 0;
		mass = fPythia.particleData.m0(fParton);
		et = sqrt(pt*pt+mass*mass);
	}
	fPythia.event.append(fParton, 23, color, anticolor, pt, 0., 0., et, mass);
	fPythia.event.append(fParton, 23, anticolor, color, -pt, 0., 0., et, mass);
	// Generate event
	fPythia.next();
}

/**
 * Access to pythia event
 * @return Reference to the PYTHIA event
 */
const Pythia8::Event &Generator::GetEvent() const {
	return fPythia.event;
}

/**
 * Set the Pythia random seed
 *
 * @param randomseed new random seed
 */
void Generator::SetPythiaSeed(unsigned long randomseed){
	fPythia.readString("Random:setSeed = on");
	std::stringstream seedstring;
	seedstring << "Random:seed = " << randomseed;
	fPythia.readString(seedstring.str());
}

/**
 * Set the random seed for the parton pair pt generation
 *
 * @param randomseed Randon seed
 */
void Generator::SetPartonRandomSeed(unsigned long randomseed){
	std::seed_seq  myseed(randomseed);
	fRandomEngine.seed(myseed);
}
