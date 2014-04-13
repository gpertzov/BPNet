// BPLink.h: interface for the BPLink class.
//
// Copyright Gideon Pertzov, 2003
//
// This software is provided "as is" without express or implied
// warranties. You may freely copy and compile this source into
// applications you distribute provided that credit is given to
// the original author.
//
//////////////////////////////////////////////////////////////////////

#ifndef _BPLINK_H
#define _BPLINK_H

#include <cassert>
#include <cmath>
#include "BPNode.h"


class BPLink  
{
// Methods
public:
	BPLink();
	virtual ~BPLink();

	// get link id
	int  id() const	{ return _id; }

	// init link weight
	void initWeight();

	// connect 2 nodes
	void connect(BPNode* inNode, BPNode* outNode);

	// update link weight
	inline	void  updateWeight(double change);

	// get input node VALUE
	inline double inValue() const;

	// get WEIGHTED input node VALUE
	inline double weightedInValue() const;

	// get WEIGHTED output ERROR
	inline double weightedOutError() const;
	
	// save/load link
	bool save( ofstream &ost ) const;
	bool load( ifstream &ist );

// Members
protected:

	int		_id; // link unique id

	double	_weight;	// link weight
	double	_delta;		// delta from previous weight change

	BPNode*	_pInNode;	// pointer to input node
	BPNode* _pOutNode;	// pointer to output node

private:
    static int _idCounter; // counter for issuing link id's
};


//////////////////////////////////////////////////////////////////////
// BPLink inline functions
//////////////////////////////////////////////////////////////////////


//====================================================================
// Return the input node's VALUE 
//====================================================================
inline double BPLink::inValue() const
{
	assert ( _pInNode != NULL );

	return ( _pInNode->getValue() );
}


//====================================================================
// Return the input node's VALUE multiplied by the WEIGHT
//====================================================================
inline double BPLink::weightedInValue() const
{
	assert ( _pInNode != NULL );

	return ( _pInNode->getValue() * _weight );
}


//====================================================================
// Return the output node's ERROR multiplied by the WEIGHT
//====================================================================
inline double BPLink::weightedOutError() const
{
	assert ( _pOutNode != NULL );

	return ( _pOutNode->getError() * _weight );
}


//====================================================================
// Update link's weight
//====================================================================
inline void BPLink::updateWeight(double change)
{
	assert ( _pOutNode != NULL );

	// get momentum value from output node
	double momentum = _pOutNode->getMomentum();

	// update weight with current change and fraction of previous change
	double deltaW = (change + (momentum * _delta));
	_weight += deltaW;
	
	// store delta for next time
	_delta = deltaW;			       
}




#endif // _BPLINK_H
