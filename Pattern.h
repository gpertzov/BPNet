// Pattern.h: interface for the Pattern class.
//
// Copyright Gideon Pertzov, 2003
//
// This software is provided "as is" without express or implied
// warranties. You may freely copy and compile this source into
// applications you distribute provided that credit is given to
// the original author.
//
//////////////////////////////////////////////////////////////////////

#ifndef _PATTERN_H
#define _PATTERN_H

#include <vector>
using namespace std;

class Pattern  
{
// Methods
public:
	Pattern(int inSize, int outSize);
	Pattern( int inSize, int outSize, int id, ... );
	virtual ~Pattern();
	
	// set/get pattern id
	void	setId(int id)	  { _id = id; }
	int		getId()		const { return _id; }

	// get sizes of input/output sets
	int		inSize()	const { return _inVec.size();  }
	int		outSize()	const { return _outVec.size(); }

	// get input/output values
	double	getInput(int index) const;
	double	getOutput(int index) const;

	// set input/output values
	void	setInput(double value, int index);
	void	setOutput(double value, int index);

	// save/load pattern
	bool save( ofstream &ost ) const;
	bool load( ifstream &ist );

private:
	Pattern(); // private constructor

// Members
protected:

	int				_id;	
	vector<double>	_inVec;		// vector of input values
	vector<double>	_outVec;	// vector of desired output values
};

#endif // _PATTERN_H

