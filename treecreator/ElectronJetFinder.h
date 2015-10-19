#ifndef ELECTRONJETFINDER_H_
#define ELECTRONJETFINDER_H_
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version. (See cxx source for full Copyright notice)
 */

#include <array>
#include <vector>

#include <fastjet/JetDefinition.hh>
#include <fastjet/PseudoJet.hh>

#include <Pythia8/Event.h>


class ElectronJet {
public:
	ElectronJet();
	ElectronJet(const fastjet::PseudoJet &jet);
	~ElectronJet() {}

	void SetJetProperties(const fastjet::PseudoJet &jetvec) {
		fJetVector = fastjet::PseudoJet(jetvec.px(), jetvec.py(), jetvec.pz(), jetvec.e());
	}
	void AddConstituent(const Pythia8::Particle &part) { fParticles.push_back(part); }

	const fastjet::PseudoJet &GetPseudoJet() const { return fJetVector; }
	const std::vector<Pythia8::Particle> GetParticles() const { return fParticles; }

	std::vector<const Pythia8::Particle> FindElectrons() const;

protected:
	fastjet::PseudoJet 					fJetVector;
	std::vector<Pythia8::Particle>		fParticles;
};

class ElectronJetFinder {
public:
	struct ParticleStruct : public fastjet::PseudoJet::UserInfoBase {
	public:
		ParticleStruct(const Pythia8::Particle *part = nullptr):
			fastjet::PseudoJet::UserInfoBase(),
			fParticle(part)
		{}
		ParticleStruct(const ParticleStruct &ref):
			fastjet::PseudoJet::UserInfoBase(),
			fParticle(ref.fParticle)
		{}
		ParticleStruct &operator=(const ParticleStruct &ref){
			fastjet::PseudoJet::UserInfoBase::operator=(ref);
			if(this != &ref){
				fParticle = ref.fParticle;
			}
			return *this;
		}
		virtual ~ PartucleStruct() {}

		void SetParticle(const Pythia8::Particle *part) { fParticle = part; }
		const Pythia8::Particle *GetParticle() const { return fParticle; }

	private:
		const Pythia8::Particle 	*fParticle;
	};
	ElectronJetFinder();
	ElectronJetFinder(const fastjet::JetDefinition &jetdef);
	virtual ~ElectronJetFinder();

	void SetParticlePtCut(double minpt, double maxpt = 10000) {
		fPtCut[0] = minpt;
		fPtCut[1] = maxpt;
	}
	void SetParticleEtaCut(double mineta, double maxeta){
		fEtaCut[0] = mineta;
		fEtaCut[1] = maxeta;
	}

	void SetLeadingTrackPtCut(double minpt){ fLeadingTrackPtCut = minpt; }
	void SetElectronPtCut(double minpt) { fElectronPtCut = minpt; }

	void SetJetDefinition(const fastjet::JetDefinition &jetdef) { fJetDefinition = jetdef; }

	void FindJets(const Pythia8::Event & inputEvent);

	const std::vector<ElectronJet> &GetJets() const { return fJets; }

protected:
	std::vector<const fastjet::PseudoJet> FindElectron(const fastjet::PseudoJet &inputjet) const;
	const fastjet::PseudoJet *FindLeading(const fastjet::PseudoJet &inputjet) const;

	fastjet::JetDefinition					fJetDefinition;
	std::vector<ElectronJet>				fJets;

	std::array<double, 2>					fPtCut;
	std::array<double, 2>					fEtaCut;
	double 									fLeadingTrackPtCut;
	double									fElectronPtCut;
};

#endif
