// BPNet.h: interface for the BPNet class.
//
// Copyright Gideon Pertzov, 2003
//
// This software is provided "as is" without express or implied
// warranties. You may freely copy and compile this source into
// applications you distribute provided that credit is given to
// the original author.
//
//////////////////////////////////////////////////////////////////////

#ifndef _BPNET_H
#define _BPNET_H

#include <cmath>
#include <vector>
#include "Pattern.h"
using namespace std;

class BPLink;
class BPNode;

class BPNet  
{
// Methods
public:
	virtual ~BPNet();	// destructor
	BPNet();			// default c-tor	
	BPNet( double lr, double mt, int layers, ... ); // c-tor with network parameters
	
	// create network structure
	void	createNetwork( double lr, double mt, const vector<int>& nodeCnt);

	// forward-pass
	void	run();	

	// backward-pass
	void	learn();

	// get number of layers 
	int		getNumLayers()	{ return _nodeCount.size(); }

	// get numer of nodes in a specific layer
	int		getNumNodes(int layerIndex);

	// set values of input nodes
	void	setInput(double value, int inputNodeIndex);
	void	setInput( const Pattern* pattern );

	// set desired output for error computation
	void	setError(double value, int outputNodeIndex);
	void	setError( const Pattern* pattern);

	// get values/errors of output nodes
	double	getOutput(int outputNodeIndex)	const;
	double	getError(int outputNodeIndex)	const;

	// set training parameters
	void	setLearningRate(double lr);
	void	setMomentum(double mt);

	// get training parameters
	double	getLearningRate();
	double	getMomentum();

	// save/load network
	bool save( ofstream &ost ) const;
	bool load( ifstream &ist );

protected:

	// cleanup
	void destroyNetwork();


// Members
protected:
	
	int				_firstMiddleNode;	// index of first node in middle layer
	int				_firstOutputNode;	// index of first node in output layer

	vector<int>		_nodeCount;		// stores number of nodes in each layer
	vector<BPNode*>	_nodes;			// vector of network nodes
	vector<BPLink*> _links;			// vector of links between nodes
};

#endif // _BPNET_H
