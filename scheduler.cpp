//============================================================================
// Name        : scheduler.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
//============================================================================

#include <iostream>
#include <map>
#include <algorithm>
#include <climits>
#include <string.h>
#include "edge.h"
#include "node.h"
#include "scheduler.h"

using namespace std;

Scheduler::Scheduler()
{
}

Scheduler::Scheduler(FILE* inputFile)
{
	n = 0;
	this->inputFile = inputFile;
	readFile();
}

int Scheduler::getN()
{
	return n;
}

void Scheduler::setN(int n)
{
	this->n = n;
}
void extractDependencies(char *p, std::vector<std::string> &taskDependenciesList)
{
	int count = 0;
	
	while (p != NULL )
	{
		if (count >= 1)
		{
			std::string str(p);
			taskDependenciesList.push_back(str);
		}
		p = strtok(NULL, " \n");
		count++;
	}
}

char *extractTaskInfo(char * buf, std::string &taskName, int *taskExecutionTime, size_t *taskDepNumber) 
{
	char *p = strtok(buf, " ");

	if (p != NULL)
	{
		taskName.assign(p, strlen(p));
	}
			
	p = strtok(NULL, " ");
	if (p != NULL)
	{
		*taskExecutionTime = atoi(p);
	}
			
	p = strtok(NULL, " ");
	if (p != NULL)
	{
		*taskDepNumber = atoi(p);
	}

	return p;
}

void Scheduler::readFile()
{
	int line = 0;

	while (!feof(this->inputFile))
	{
		std::vector<std::string> taskDependenciesList;
		char buf[256] = {0};
		line++;

		fgets(buf, sizeof buf, this->inputFile);

		if (line == 1)
		{
			n = atoi(buf);
		}
		else
		{
			std::string taskName;
			int taskExecutionTime;
			size_t taskDepNumber;
			
			char *p = extractTaskInfo(buf, taskName, &taskExecutionTime, &taskDepNumber);
			
			extractDependencies(p, taskDependenciesList);

			Node *node = new Node(taskName, taskExecutionTime, taskDependenciesList);
			std::pair<std::string, Node*> pair;
			pair.first=node->getName();
			pair.second = node;
			nodeMap.insert(pair);

			if (taskDependenciesList.size() != taskDepNumber)
			{
				printf("file format error");
			}
			
			if (taskDependenciesList.size() == 0)
			{
				nodesNoDependent.push(node->getName());
			}
		}
	}
}

void topSortUtil(Node *node, std::queue<std::string> *nodesNoDependent)
{
	// for each node m with an edge e from node to m do
	std::list<Edge*>::iterator it = node->outEdges.begin();
	while (it != node->outEdges.end())
	{
		// remove edge e from the graph
		Edge *e = (Edge*)*it;
		Node *m = e->to;

		// remove edge from node
		node->outEdges.erase(it++); 

		// remove edge from m
		m->inEdges.remove(e); 

		//if m has no other incoming edges then insert m into S
		if (m->inEdges.empty())
		{
			(*nodesNoDependent).push(m->getName());
		}
	}
}

bool Scheduler::topologicalSort()
{
	while (!nodesNoDependent.empty())
	{
		// remove a node
		std::string nodeName = nodesNoDependent.front();
		nodesNoDependent.pop();

		std::map<std::string, Node*>::iterator it = this->nodeMap.find(nodeName);

		Node *node=it->second;

		sortednodeMap.insert(std::pair<std::string, Node*>(nodeName, node));

		topSortUtil(node, &nodesNoDependent);
	}

	//Check to see if all edges are removed
	bool cycle = false;

	for (std::map<std::string, Node*>::iterator it=nodeMap.begin(); it!=nodeMap.end(); ++it)
	{
		Node * node = it->second;
		if (!node->inEdges.empty())
		{
			cycle = true;
			break;
		}
	}

	if (cycle)
	{
		printf("Cycle present, topological sort not possible");
		return false;
	}

	return true;
}


void Scheduler::searchDependencyNode(Node *node, std::string nodeName)
{
	std::map<std::string, Node*>::iterator it = this->nodeMap.find(nodeName);
	it->second->addEdge(node);
}

void Scheduler::buildGraph()
{
	for (auto it=this->nodeMap.begin(); it!=this->nodeMap.end(); ++it)
	{
		Node *node = (Node*)it->second;
		std::vector<std::string> nodeDependencies = node->getDependencies();
		int nodeDependencyListSize = nodeDependencies.size();

		for (int i = 0; i < nodeDependencyListSize ; i++)
		{
			std::string currentNodeDependency = nodeDependencies[i];
			searchDependencyNode(node, currentNodeDependency);
		}
	}
}

std::map<std::string, Node*> Scheduler::getSortedList()
{
	return sortednodeMap;
}

std::map<std::string, Node*> Scheduler::getNodeMap()
{
	return nodeMap;
}

int getCorrespondingDistance(std::map<Node*, int> distance, Node *node)
{
	std::map<Node*, int>::iterator it = distance.find(node);
	int distanceNodeNeighBour = -1;

	if (it != distance.end())
	{
		distanceNodeNeighBour = it->second;
	}

	return distanceNodeNeighBour;
}

int Scheduler::getTotalDistanceFromNode(std::map<Node*, int> *distance, Node *sourceNode)
{
	int maxDistance = 0;
	int totalDistanceSourceNode = 0;

	for (std::list<Edge*>::iterator it = sourceNode->outEdges.begin() ; it != sourceNode->outEdges.end(); ++it)
	{
		Edge *edge = *it;
		Node *nodeNeighBour = edge->to;
		int distanceNodeNeighBour = getCorrespondingDistance(*distance, nodeNeighBour);

		if (distanceNodeNeighBour == INT_MIN)
		{
			getTotalDistanceFromNode(distance, nodeNeighBour);
		}

		maxDistance = std::max(maxDistance, getCorrespondingDistance(*distance, nodeNeighBour));
	}

	totalDistanceSourceNode = maxDistance + sourceNode->getExecutionTime();

	(*distance)[sourceNode] = totalDistanceSourceNode;

	return totalDistanceSourceNode;
}

int Scheduler::calculateMinTimeScheduling()
{
	int maxDistance = INT_MIN;

	std::map<Node*, int> distance;

	buildGraph();

	if (false == topologicalSort())
	{
		return -1;
	}

	// rebuild graph, edges were removed during topological sort
	buildGraph();

	for (std::map<std::string, Node*>::iterator it=sortednodeMap.begin(); it!=sortednodeMap.end(); ++it)
	{
		distance.insert(std::pair<Node*, int>(it->second, INT_MIN));
	}

	for (std::map<std::string, Node*>::iterator it=sortednodeMap.begin(); it!=sortednodeMap.end(); ++it)
	{
		Node *node = it->second;
		if (distance.at(node) == INT_MIN)
		{
			maxDistance = std::max(maxDistance, getTotalDistanceFromNode(&distance, node));
		}
	}

	return maxDistance;
}
