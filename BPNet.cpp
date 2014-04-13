// BPNet.cpp: implementation of the BPNet class.
//
// Copyright Gideon Pertzov, 2003
//
// This software is provided "as is" without express or implied
// warranties. You may freely copy and compile this source into
// applications you distribute provided that credit is given to
// the original author.
//
//////////////////////////////////////////////////////////////////////

#include <ctime>
#include <cstdarg>
#include <iostream>
#include <fstream>
#include "BPNet.h"
#include "BPLink.h"
#include "BPNode.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BPNet::~BPNet()
{
	// cleanup
	destroyNetwork();
}


BPNet::BPNet() :	_firstMiddleNode(0),
					_firstOutputNode(0)
{

}


BPNet::BPNet( double lr, double mt, int layers, ... )
{
	va_list vl;

	// construct layers vector according to argument list
	va_start(vl, layers );

	vector<int> layersVec(layers);
	for (int i = 0; i != layers; ++i)
		layersVec[i] = va_arg(vl, int);

	va_end( vl );

	// create network
	createNetwork( lr, mt, layersVec);
}


//====================================================================
// Create neural-network according to parameters
//====================================================================
void BPNet::createNetwork( double lr, double mt, const vector<int>& nodeCnt)
{
	srand((unsigned)time(NULL));

	// destroy existing network
	destroyNetwork();

	// copy node count vector (number of nodes in each layer)
	_nodeCount	= nodeCnt; 

	int numLayers = _nodeCount.size();
	if ( numLayers == 0 )
		return; // nothing to do if no layers


	// create nodes
	int numLinks = 0;
	for(int i = 0; i < numLayers; ++i)
	{
		int numNodes = _nodeCount[i];
		for(int j = 0; j < numNodes; ++j)
		{
			// create a new node 
			BPNode* node = new BPNode(lr, mt);

			// add it to the nodes vector
			_nodes.push_back(node);
		}

		// count links while we're at it
		if ( i > 0 )
		  numLinks += _nodeCount[i-1] * numNodes; 
	}
	
	// set index of first middle node 
	// (index of last input node + 1)
	_firstMiddleNode = _nodeCount[0];

	// set index of first output node 
	// (total number of nodes - number of output nodes)
	_firstOutputNode = _nodes.size() - _nodeCount[numLayers-1];



	// create links
	for (i = 0; i < numLinks; ++i)               
	{
		// create new link
		BPLink* link = new BPLink;

		// add it to the links vector
		_links.push_back(link);
	}


	// connect layers
	int current		= 0;
	int curLayer	= 0;
	int	nextLayer	= _firstMiddleNode;

	BPNode* pInNode		= NULL;
	BPNode* pOutNode	= NULL;
	BPLink*	pLink		= NULL;

	for (i = 0; i < numLayers-1; ++i)
	{
		for (int j = 0; j < _nodeCount[i+1]; ++j)
			for (int k = 0; k < _nodeCount[i]; ++k)
			{
				pInNode		= _nodes[curLayer+k];
				pOutNode	= _nodes[nextLayer+j];
				pLink		= _links[current++];

				// connect input node to output node
				pLink->connect(pInNode, pOutNode);
			}

		curLayer = nextLayer;
		nextLayer += _nodeCount[i+1];
	}


}


//====================================================================
// Destroy neural-network
//====================================================================
void BPNet::destroyNetwork()
{
	// cleanup links and nodes
	int numLinks = _links.size();
	for(int i = 0; i != numLinks; ++i)
		delete _links[i];

	int numNodes = _nodes.size();
	for(i = 0; i != numNodes; ++i)
		delete _nodes[i];

	_links.clear();
	_nodes.clear();
	_nodeCount.clear();

	_firstMiddleNode	= 0;
	_firstOutputNode	= 0;

}



//====================================================================
// Get number of nodes in a specific layer
//====================================================================
int BPNet::getNumNodes(int layerIndex)
{
	assert ( _nodeCount.size() > layerIndex && layerIndex >= 0);

	return _nodeCount[layerIndex];
}


//====================================================================
// Set value of input node
//====================================================================
void BPNet::setInput(double value, int inputNodeIndex)
{
	assert ( _nodeCount.size() != 0 && inputNodeIndex >= 0 && inputNodeIndex < _nodeCount[0] );

	_nodes[inputNodeIndex]->setValue(value);
}


//====================================================================
// Set value of input node (using pattern)
//====================================================================
void BPNet::setInput( const Pattern* pattern )
{
	assert ( pattern != NULL && _nodeCount.size() != 0 && pattern->inSize() == _nodeCount[0]);

	
	int numInputNodes = _nodeCount[0];
	for (int  i = 0;  i < numInputNodes; ++i) 
	{
        _nodes[i]->setValue(pattern->getInput(i));
	}
}


//====================================================================
// Get value of output node
//====================================================================
double BPNet::getOutput(int outputNodeIndex) const
{
	assert ( _nodeCount.size() != 0 && outputNodeIndex >= 0 && outputNodeIndex < _nodeCount[_nodeCount.size()-1] );
	
	return _nodes[ _firstOutputNode + outputNodeIndex ]->getValue();
}


//====================================================================
// Get error of output node
//====================================================================
double BPNet::getError(int outputNodeIndex) const
{
	assert ( _nodeCount.size() != 0 && outputNodeIndex >= 0 && outputNodeIndex < _nodeCount[_nodeCount.size()-1] );
	
	return _nodes[ _firstOutputNode + outputNodeIndex ]->getError();
}


//====================================================================
// Set desired output for error computation
//====================================================================
void BPNet::setError(double value, int outputNodeIndex)
{
	assert ( _nodeCount.size() != 0 && outputNodeIndex >= 0 && outputNodeIndex < _nodeCount[_nodeCount.size()-1] );

	_nodes[ _firstOutputNode + outputNodeIndex ]->setError(value);
}


//====================================================================
// Set desired output for error computation (using Pattern)
//====================================================================
void BPNet::setError( const Pattern* pattern)
{
	assert ( pattern != NULL && _nodeCount.size() != 0 && pattern->outSize() == _nodeCount[_nodeCount.size()-1]);

	int numOutputNodes = _nodeCount[_nodeCount.size()-1];
	for (int  i = 0;  i < numOutputNodes; ++i) 
          _nodes[i + _firstOutputNode]->setError(pattern->getOutput(i));
}


//====================================================================
// Set learning rate for all nodes
//====================================================================	
void BPNet::setLearningRate(double lr)
{
	int numNodes = _nodes.size();
	for(int i = 0; i != numNodes; ++i)
		_nodes[i]->setLearningRate(lr);
}



//====================================================================
// Set momentum for all nodes
//====================================================================	
void BPNet::setMomentum(double mt)
{
	int numNodes = _nodes.size();
	for(int i = 0; i != numNodes; ++i)
		_nodes[i]->setMomentum(mt);
}


//====================================================================
// Get learning rate
//====================================================================	
double BPNet::getLearningRate()
{
	assert( _nodes.size() != 0);

	return _nodes[0]->getLearningRate();
}


//====================================================================
// Get momentum
//====================================================================	
double BPNet::getMomentum()
{
	assert( _nodes.size() != 0);

	return _nodes[0]->getMomentum();
}


//====================================================================
// Run - forward pass
//====================================================================	
void BPNet::run()
{
	// run only nodes in middle and output layers
	// (input nodes don't have input links)
	int numNodes = _nodes.size();
	for (int i = _firstMiddleNode; i != numNodes; ++i) 
	{
		_nodes[i]->run();
	}
}



//====================================================================
// Learn - backward pass
//====================================================================	
void BPNet::learn()
{
	// we loop backwards from output nodes towards the middle nodes
	int numNodes = _nodes.size();
	for(int i = numNodes-1; i >= _firstMiddleNode; i--)
	{
		_nodes[i]->learn();
	}
}


//====================================================================
// Save network to file
//====================================================================
bool BPNet::save( ofstream &ost ) const
{
	if ( !ost.good() )
		return false;

	int numLayers = _nodeCount.size();
	int numNodes = _nodes.size();
	int numLinks = _links.size();

	ost << numLayers << endl; // num layers
	
	for(int i = 0; i != numLayers; ++i)
		ost << _nodeCount[i] << endl; // number of nodes in each layer
	
	ost << numNodes << endl; // total number of nodes
	ost << numLinks << endl; // total number of links

	// save nodes data
	for(i = 0; i != numNodes; ++i)
		_nodes[i]->save(ost);

	// save links data
	for(i = 0; i != numLinks; ++i)
		_links[i]->save(ost);
	
	if (!ost.good())
		return false;

	return true;
}


//====================================================================
// Load network from file
//====================================================================
bool BPNet::load( ifstream &ist )
{
	if ( !ist.good() )
		return false;
	
	int numLayers	= 0;
	int numNodes	= 0;
	int numLinks	= 0;

	ist >> numLayers;	// num layers
	
	vector<int> layers(numLayers);
	for(int i = 0; i != numLayers; ++i)
		ist >> layers[i];	// number of nodes in each layer

	ist >> numNodes;
	ist >> numLinks;

	// create network structure (nodes and links)
	createNetwork(0, 0, layers);
	
	// load nodes data
	for (i = 0; i != numNodes; ++i)
		_nodes[i]->load(ist);

	// load links data
	for(i = 0; i != numLinks; ++i)
		_links[i]->load(ist);


	if (!ist.good())
		return false;

	return true;
}