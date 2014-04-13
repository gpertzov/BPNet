// BPNode.h: interface for the BPNode class.
//
// Copyright Gideon Pertzov, 2003
//
// This software is provided "as is" without express or implied
// warranties. You may freely copy and compile this source into
// applications you distribute provided that credit is given to
// the original author.
//
//////////////////////////////////////////////////////////////////////
#ifndef _BPNODE_H
#define _BPNODE_H

#include <cmath>
#include <vector>
using namespace std;

class BPLink;


class BPNode  
{
// Methods
public:

	virtual ~BPNode();				// destructor
	BPNode();						// default c-tor
	BPNode(double lr, double mt);	// c-tor with learning rate and momentum
	
	void run();		// forward-pass 
	void learn();	// backward-pass

	// get node id
	int  id() const						{ return _id; }

	// value
	double	getValue() const			{ return _value;	}
	void	setValue(double val)		{ _value = val;		}
	
	// error
	double	getError()  const			{ return _error;	}
	void	setError(double err)		{ _error = err;		}

	// learning rate
	double	getLearningRate() const		{ return _lr;		}
	void	setLearningRate(double l)	{ _lr = l;			}

	// momentum
	double	getMomentum() const			{ return _mt;		}
	void	setMomentum(double m)		{ _mt = m;			}

	// links
	void addOutLink(BPLink *link);
	void addInLink(BPLink *link);

	// save/load node
	bool save( ofstream &ost ) const;
	bool load( ifstream &ist );

protected:

			double	computeError();
	inline	double	transferFunction(double val);
	inline	double	derivativeFunction(double val);



// Members
protected:
	
	int		_id;	// node unique id

	double	_value; // current value
	double	_error; // last error

	double	_lr;	// learning rate
	double	_mt;	// momentum

	vector<BPLink*> _inLinks;	// vector of input links (e.g. from previous layer)
	vector<BPLink*> _outLinks;	// vector of output links (e.g. to next layer)

private:
    static int _idCounter; // counter for issuing node id's
};

//////////////////////////////////////////////////////////////////////
// BPNode inline functions
//////////////////////////////////////////////////////////////////////


//====================================================================
// Transfer/activation function
//====================================================================
inline double BPNode::transferFunction(double val)
{
	// sigmoid
	return 1.0 / (1.0 + exp(-val));
}



//====================================================================
// Derivative of transfer function
//====================================================================
inline double BPNode::derivativeFunction(double val)
{
	// DERIVATIVE of sigmoid
	return (val * ( 1.0 - val ));
}


#endif // _BPNODE_H
