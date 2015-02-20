/*
 * node.h
 *
 *      Author: Adelina Stanciu
 */
#ifndef NODE_H
#define NODE_H

#include "edge.h"
#include <vector>
#include <string>
#include <list>

using namespace std;

class Edge;

class Node {

private:
	std::string name;
	int taskExecuteTime;
	std::vector<std::string> taskDependenciesList;

public:
	std::list<Edge*> inEdges;
	std::list<Edge*> outEdges;

	Node();
	Node(std::string name, int taskExecuteTime, std::vector<std::string> taskDependenciesList);

	Node* addEdge(Node *node);

	std::string getName();

	std::vector<std::string> getDependencies();

	int getExecutionTime();

	int getDependenciesNumber();
};

#endif


