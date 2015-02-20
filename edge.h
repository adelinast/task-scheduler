/*
 * edge.cpp
 */
#ifndef EDGE_H
#define EDGE_H

#include "node.h"

using namespace std;

class Node;

class Edge {
	public:
		Edge(Node *from, Node *to)
		{
			this->from = from;
			this->to = to;
		}
		
		Node *from;
		Node *to;
};

#endif
