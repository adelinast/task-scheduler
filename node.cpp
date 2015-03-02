#include "node.h"
#include "edge.h"
#include<vector>
#include<string>
#include <memory>
using namespace std;

Node::Node(std::string name, int taskExecuteTime, std::vector<std::string> taskDependenciesList)
{
	this->name = name;
	this->taskExecuteTime = taskExecuteTime;
   	this->taskDependenciesList = taskDependenciesList;
}

Node::~Node()
{
}

Node* Node::addEdge(Node *node)
{
	auto edge = std::make_shared<Edge>(this, node);
	this->outEdges.push_back(edge);
	node->inEdges.push_back(edge);
	return this;
}

std::string Node::getName()
{
	return name;
}

std::vector<std::string> Node::getDependencies()
{
	return taskDependenciesList;
}

int Node::getExecutionTime()
{
	return taskExecuteTime;
}

int Node::getDependenciesNumber()
{
	return taskDependenciesList.size();
}
