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
#include "jettree/JetTreeData.h"

JetTreeData::JetTreeData():
		TObject(),
		fPx(0),
		fPy(0),
		fPz(0),
		fE(0),
		fConstituents()
{
}

JetTreeData::JetTreeData(double px, double py, double pz, double e) :
		TObject(),
		fPx(px),
		fPy(py),
		fPz(pz),
		fE(e),
		fConstituents()
{
}

void JetTreeData::AddConstituent(double px, double py, double pz, double e, int pdg){
	fConstituents.push_back(JetTreeConstituent(px, py, pz, e, pdg));
}

void JetTreeData::Reset(){
	fPx = 0;
	fPy = 0;
	fPz = 0;
	fE = 0;
	fConstituents.clear();
}

JetTreeConstituent::JetTreeConstituent():
		TObject(),
		fPx(0),
		fPy(0),
		fPz(0),
		fE(0),
		fPDG(0)
{
}

JetTreeConstituent::JetTreeConstituent(double px, double py, double pz, double e, int pdg):
		TObject(),
		fPx(px),
		fPy(py),
		fPz(pz),
		fE(e),
		fPDG(pdg)
{
}
