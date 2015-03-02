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
#include <memory>

using namespace std;

class Edge;

class Node {

private:
	std::string name;
	int taskExecuteTime;
	std::vector<std::string> taskDependenciesList;

public:
	std::list<shared_ptr<Edge>> inEdges;
	std::list<shared_ptr<Edge>> outEdges;

	Node();
	Node(std::string name, int taskExecuteTime, std::vector<std::string> taskDependenciesList);
	~Node();

	Node* addEdge(Node *node);

	std::string getName();

	std::vector<std::string> getDependencies();

	int getExecutionTime();

	int getDependenciesNumber();
};

#endif


