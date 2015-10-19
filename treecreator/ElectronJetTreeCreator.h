#ifndef ELECTRONJETTREECREATOR_H_
#define ELECTRONJETTREECREATOR_H_
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version. (See cxx source for full Copyright notice)
 */

#include "ElectronJetFinder.h"
#include "Generator.h"
#include "JetTreeData.h"
#include <memory>
#include <string>


]class TFile;
class TTree;

class ElectronJetTreeCreator {
public:
	ElectronJetTreeCreator();
	ElectronJetTreeCreator(Generator::Parton_t parton);
	virtual ~ElectronJetTreeCreator() {}

	void SetPartonID(Generator::Parton_t parton);
	void SetMinPtConstituent(double ptcut);
	void SetEtaRangeConstituent(double etamin, double etamax);
	void SetMinPtLeading(double ptcut);
	void SetJetR(double r);
	void SetOuputFilename(std::string filename) { fOutputFileName = filename; }
	void SetSeed(unsigned long seed);

	void Init();
	void Process(int nevents = 1000);

protected:
	void Generate();
	JetTreeData	ConvertElectronJet(const ElectronJet &inputjet) const;

private:
	Generator 									fGenerator;
	ElectronJetFinder							fJetFinder;

	std::string									fOutputFileName;
	std::unique_ptr<TFile>						fOutputFile;
	std::unique_ptr<TTree>						fOutputTree;
	std::unique_ptr<std::vector<JetTreeData> >	fElectronJets;
};

#endif
