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
#include "ElectronJetTreeCreator.h"

#include <TTree.h>

#include <Pythia8/Event.h>
#include <fastjet/JetDefinition.hh>
#include <fastjet/PseudoJet.hh>

#include <vector>

ElectronJetTreeCreator::ElectronJetTreeCreator() :
	fJetFinder(),
	fGenerator(),
	fOutputFileName("JetTree.root"),
	fOutputFile(),
	fOutputTree(),
	fElectronJets(std::unique_ptr<std::vector<JetTreeData> >(new std::vector<JetTreeData>))
{
}

ElectronJetTreeCreator::ElectronJetTreeCreator(Generator::Parton_t parton):
	fJetFinder(),
	fGenerator(parton),
	fOutputFileName("JetTree.root"),
	fOutputFile(),
	fOutputTree(),
	fElectronJets(std::unique_ptr<std::vector<JetTreeData> >(new std::vector<JetTreeData>))
{
}

void ElectronJetTreeCreator::Init() {
	fOutputFile = std::unique_ptr<TFile>(new TFile(fOutputFileName.c_str(), "RECREATE"));
	fOutputTree = std::unique_ptr<TTree>(new TTree("JetTree", "Electron jet tree"));
	fOutputTree->Branch("jets", &(fElectronJets.get()));
}

void ElectronJetTreeCreator::Process(int nevents) {
	for(int iev = 0; iev < nevents; iev++){
		Generate();
		fOutputTree->Write();
	}
}

void ElectronJetTreeCreator::Generate() {
	fElectronJets->clear();

	fGenerator.Generate();
	fJetFinder.FindJets(fGenerator.GetEvent());
	for(auto injet : fJetFinder.GetJets()){
		fElectronJets->push_back(ConvertElectronJet(injet));
	}
}

JetTreeData ElectronJetTreeCreator::ConvertElectronJet(const ElectronJet &inputjet) const {
	fastjet::PseudoJet jetvec = inputjet.GetPseudoJet();
	JetTreeData result(jetvec.px(), jetvec.py(), jetvec.pz(), jetvec.E());
	for(auto myconst : inputjet.GetParticles()){
		result.AddConstituent(myconst.px(), myconst.py(), myconst.pz(), myconst.e(), myconst.id());
	}
	return result;
}

void ElectronJetTreeCreator::SetPartonID(Generator::Parton_t parton){
	fGenerator.SetParton(parton);
}

void ElectronJetTreeCreator::SetMinPtConstituent(double ptcut){
	fJetFinder.SetParticlePtCut(ptcut);
}

void ElectronJetTreeCreator::SetEtaRangeConstituent(double etamin, double etamax){
	fJetFinder.SetParticleEtaCut(etamin, etamax);
}

void ElectronJetTreeCreator::SetMinPtLeading(double ptcut){
	fJetFinder.SetLeadingTrackPtCut(ptcut);
}
void ElectronJetTreeCreator::SetSeed(unsigned long seed){
	// for simplicity pythia and the pt generator engine get the same seed
	fGenerator.SetPartonRandomSeed(seed);
	fGenerator.SetPythiaSeed(seed);
}

void ElectronJetTreeCreator::SetJetR(double r){
	fJetFinder.SetJetDefinition(fastjet::JetDefinition(fastjet::antikt_algorithm, r));
}

