#ifndef JETTREE_JETTREEDATA_H_
#define JETTREE_JETTREEDATA_H_
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version. (See cxx source for full Copyright notice)
 */

#include <TObject.h>
#include <vector>

class JetTreeConstituent : public TObject {
public:
	JetTreeConstituent();
	JetTreeConstituent(double px, double py, double pz, double e, int pdg);
	virtual ~JetTreeConstituent() {}

	inline void Set(double px, double py, double pz, double e, int pdg);

	double	GetPx() const { return fPx; }
	double GetPy() const { return fPy; }
	double GetPz() const { return fPz; }
	double GetE() const { return fE; }
	void GetPxPyPzE(double *pxyz);
	int GetPdg() const { return fPDG; }

protected:
	double				fPx;
	double				fPy;
	double				fPz;
	double				fE;
	int					fPDG;

	ClassDef(JetTreeConstituent, 1);
};

class JetTreeData {
public:
	JetTreeData();
	JetTreeData(double px, double py, double pz, double e);
	virtual ~JetTreeData() {}

	void AddConstituent(double px, double py, double pz, double e, int pdg);
	inline void Set(double px, double py, double pz, double e);

	double GetPx() const { return fPx; }
	double GetPy() const { return fPy; }
	double GetPz() const { return fPz; }
	inline void GetPxPyPzE(double *pxyz);
	double GetE() const { return fE; }
	const std::vector<JetTreeConstituent> &GetConstituent() const { return fConstituents; }

	void Reset();

private:
	double								fPx;
	double								fPy;
	double								fPz;
	double								fE;
	std::vector<JetTreeConstituent>		fConstituents;

	ClassDef(JetTreeData, 1);
};

void JetTreeConstituent::Set(double px, double py, double pz, double e, int pdg){
	fPx = px;
	fPy = py;
	fPz = pz;
	fE = e;
	fPDG = pdg;
}

void JetTreeConstituent::GetPxPyPzE(double *pxyz){
	pxyz[0] = fPx;
	pxyz[1] = fPy;
	pxyz[2] = fPz;
	pxyz[3] = fE;
}


void JetTreeData::Set(double px, double py, double pz, double e){
	fPx = px;
	fPy = py;
	fPz = pz;
	fE = e;
}

void JetTreeData::GetPxPyPzE(double *pxyz){
	pxyz[0] = fPx;
	pxyz[1] = fPy;
	pxyz[2] = fPz;
	pxyz[3] = fE;
}

#endif
