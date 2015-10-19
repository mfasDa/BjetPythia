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
#include "ElectronJetFinder.h"

#include <fastjet/ClusterSequence.hh>

ElectronJetFinder::ElectronJetFinder():
		fJetDefinition(),
		fJets(),
		fPtCut(),
		fEtaCut(),
		fLeadingTrackPtCut(0.),
		fElectronPtCut(0.)
{
	fJetDefinition = fastjet::JetDefinition(fastjet::antikt_algorithm, 0.4);
	fPtCut[0] = 0; fPtCut[1] = 1e9;
	fEtaCut[0] = -1e9; fEtaCut[1] = 1e9;
}

ElectronJetFinder::ElectronJetFinder(const fastjet::JetDefinition &jetdef):
		fJetDefinition(jetdef),
		fJets(),
		fPtCut(),
		fEtaCut(),
		fLeadingTrackPtCut(0.),
		fElectronPtCut(0.)
{
	fPtCut[0] = 0; fPtCut[1] = 1e9;
	fEtaCut[0] = -1e9; fEtaCut[1] = 1e9;
}

ElectronJetFinder::~ElectronJetFinder() {
}

void ElectronJetFinder::FindJets(const Pythia8::Event &input){
	fJets.clear();
	std::vector<fastjet::PseudoJet> inputparticles;
	for(int ipart = 0; ipart < input.size(); ipart++){
		const Pythia8::Particle &mypart = input[ipart];
		if(!mypart.isFinal()) continue;
		fastjet::PseudoJet inputjet(mypart.px(), mypart.py(), mypart.pz(), mypart.e());
		inputjet.set_user_info(new ElectronJetFinder::ParticleStruct(&mypart));
		inputparticles.push_back(inputjet);
	}

	fastjet::ClusterSequence jetfinder(inputparticles, fJetDefinition);
	std::vector<fastjet::PseudoJet> recjets = sorted_by_pt(jetfinder.jets());

	// find jets with electron, apply leading track and leading electron cut
	for(auto testjet : recjets){
		std::vector<const fastjet::PseudoJet> electrons = FindElectron(testjet);
		if(!electrons.size()) continue;
		const fastjet::PseudoJet *leadingpart = FindLeading(testjet);
		if(leadingpart->pt() < this->fLeadingTrackPtCut) continue;
		// jet accepted
		ElectronJet accepted(testjet);
		for(auto constituent : testjet.constituents){
			accepted.AddConstituent(*(static_cast<ParticleStruct *>(constituent.user_info_ptr())->GetParticle()));
		}
		fJets.push_back(accepted);
	}
}

std::vector<const fastjet::PseudoJet> ElectronJetFinder::FindElectron(const fastjet::PseudoJet &inputjet) const {
	std::vector<const fastjet::PseudoJet>  result;
	for(auto constiter : inputjet.constituents()){
		const Pythia8::Particle *underlying = static_cast<ParticleStruct *>(inputjet.user_info_ptr())->GetParticle();
		if(std::abs(underlying->id()) == 11){
			if(underlying->pT() > this->fElectronPtCut){
				result.push_back(constiter);
			}
		}
	}
	return result;
}

const fastjet::PseudoJet *ElectronJetFinder::FindLeading(const fastjet::PseudoJet &inputjet) const {
	return &(sorted_by_pt(inputjet.constituents()))[0];
}

ElectronJet::ElectronJet():
		fJetVector(),
		fParticles()
{
}

ElectronJet::ElectronJet(const fastjet::PseudoJet &jetvec):
		fJetVector(fastjet::PseudoJet(jetvec.px(), jetvec.py(), jetvec.pz(), jetvec.e())),
		fParticles()
{
}

std::vector<const Pythia8::Particle> ElectronJet::FindElectrons() const {
	std::vector<const Pythia8::Particle> result;
	for(auto constit : fParticles){
		if(std::abs(constit.id()) == 11){
			result.push_back(constit);
		}
	}
	return result;
}
