// BPNode.cpp: implementation of the BPNode class.
//
// Copyright Gideon Pertzov, 2003
//
// This software is provided "as is" without express or implied
// warranties. You may freely copy and compile this source into
// applications you distribute provided that credit is given to
// the original author.
//
//////////////////////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "BPNode.h"
#include "BPLink.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// init static member
int BPNode::_idCounter = -1;

BPNode::~BPNode()
{

}


BPNode::BPNode() :	_id(++_idCounter),
					_value(0),
					_error(0),
					_lr(0.3),
					_mt(0.5)
					
{

}


BPNode::BPNode(double lr, double mt) :	_id(++_idCounter),
										_value(0),
										_error(0),
										_lr(lr),
										_mt(mt)
										
{

}


//====================================================================
// Link TO an output node 
//====================================================================
void BPNode::addOutLink(BPLink* link)
{
	assert( link != NULL);

	// add the link to the outLinks vector
	_outLinks.push_back(link);
}


//====================================================================
// Link FROM an input node 
//====================================================================
void BPNode::addInLink(BPLink *link)
{
	assert( link != NULL);

	// add the link to the inLinks vector
	_inLinks.push_back(link);
}




//====================================================================
// Run: forward-pass, input summation and activation
//====================================================================
void BPNode::run()
{
	// iterate on all input links
	int numInputs = _inLinks.size();

	double total = 0;
	for(int i = 0; i != numInputs; ++i)
	{
		// sum weighted values from input nodes
		total += _inLinks[i]->weightedInValue();	
	}

	// pass sum through activation function
	_value = transferFunction(total);
}


//====================================================================
// Learn: backward-pass, error computation and weights adjustment
//====================================================================
void BPNode::learn()
{
	// compute error
	_error = computeError();

	// iterate on all input links
	int numInputs = _inLinks.size();

	BPLink* pLink	= NULL;
	double	change	= 0;
	for(int i = 0; i != numInputs; ++i)
	{
		pLink = _inLinks[i];

		// compute weight change
		// ( learningRate * Error * InputValue )
		change = _lr * _error * pLink->inValue();

		// update weight
		pLink->updateWeight(change);
	}
}



//====================================================================
// Compute node error
//====================================================================
double BPNode::computeError()
{
	double computedError = 0;

	int numOutputLinks = _outLinks.size();
	if (numOutputLinks == 0) 
	{
		// Output Node //

		// The desired output for this node is now stored in _error 
		// This occurs when the BPNet::setError() method is called
		// during training

		// We scale (desired - value) by the derivative of the transfer function:
		//	error = f'(value) * ( desired - value ) 
		//  sigmoid derivative: f'(x) = x * (1 - x)  
		computedError = derivativeFunction( _value ) * ( _error - _value );
	}
	else 
	{
		// Middle Node //

		// loop on all output links
		double total = 0;
		for(int i = 0; i != numOutputLinks; ++i)
		{
			// sum weighted error from each link
			total += _outLinks[i]->weightedOutError();
		}
		
		// scale error by derivative of transfer function
		computedError = derivativeFunction( _value ) * total;
	}

	return computedError;
}


//====================================================================
// Save node data to file
//====================================================================
bool BPNode::save( ofstream &ost ) const
{
	if ( !ost.good() )
		return false;

	ost << setw(4) << _id << endl;				// id
	ost << setprecision(16) << _lr << endl;		// learning rate
	ost << setprecision(16) << _mt << endl;		// momentum
	ost << setprecision(16) << _value << endl;	// value
	ost << setprecision(16) << _error << endl;	// error
	
	return true;
}


//====================================================================
// Load node data from file
//====================================================================
bool BPNode::load( ifstream &ist )
{
	if ( !ist.good() )
		return false;

	ist >> _id;		// id
	ist >> _lr;		// learning rate
	ist >> _mt;		// momentum
	ist >> _value;	// value
	ist >> _error;	// error

	return true;
}