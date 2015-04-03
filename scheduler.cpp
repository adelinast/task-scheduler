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
const int lineMax = 8096;

Scheduler::Scheduler()
{
}

Scheduler::Scheduler(FILE* inputFile):inputFile(inputFile)
{
	n = 0;
}

Scheduler::~Scheduler()
{
	for (auto &entry : nodeMap)
	{
		Node * node = entry.second;
		delete node;
	}
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
	
	while (p != NULL)
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

int extractTaskInfo(char * buf, std::string &taskName, int *taskExecutionTime, size_t *taskDepNumber, char **current)
{
	int rc = 0;
	char *p = strtok(buf, " ");

	if (p != NULL)
	{
		taskName.assign(p, strlen(p));
	}
	else
	{
		printf("Error: Input file has wrong format, one of the fields is missing\n");
		rc = -1;
	}

	if (rc == 0)
	{
		p = strtok(NULL, " ");
		if (p != NULL)
		{
			*taskExecutionTime = atoi(p);
		}
		else
		{
			printf("Error: Input file has wrong format, one of the fields is missing\n");
			rc = -1;
		}
	}

	if (rc == 0)
	{
		p = strtok(NULL, " ");
		if (p != NULL)
		{
			*taskDepNumber = atoi(p);
		}
		else
		{
			printf("Error: Input file has wrong format, task dep number is missing\n");
			rc = -1;
		}
	}

	*current = p;

	return rc;
}

int Scheduler::addNode(std::string taskName, int taskExecutionTime, size_t taskDepNumber, std::vector<std::string> taskDependenciesList)
{
	if (taskDependenciesList.size() != taskDepNumber)
	{
		printf("Error: Input File has wrong format, the number of dependencies is incorrect %d %d\n", taskDependenciesList.size(), taskDepNumber);
		return -1;
	}
	
	Node *node = new Node(taskName, taskExecutionTime, taskDependenciesList);
	
	std::pair<std::string, Node*> pair;
	pair.first = node->getName();
	pair.second = node;
	nodeMap.insert(pair);

	return 0;
}

void Scheduler::addNodeWithNoDep(std::string taskName, std::vector<std::string> taskDependenciesList)
{
	if (taskDependenciesList.size() == 0)
	{
		nodesNoDependency.push(taskName);
	}
}

int Scheduler::readFile()
{
	int line = 0;
	int rc = 0;

	while (!feof(this->inputFile) && (rc == 0))
	{
		std::vector<std::string> taskDependenciesList;
		char buf[lineMax] = {0};
		line++;

		if (fgets(buf, sizeof buf, this->inputFile) == NULL)
		{
			break;
		}

		if (line == 1)
		{
			n = atoi(buf);
		}
		else
		{
			std::string taskName;
			int taskExecutionTime = 0;
			size_t taskDepNumber = 0;
			
			char *taskInfo = nullptr;
			rc = extractTaskInfo(buf, taskName, &taskExecutionTime, &taskDepNumber, &taskInfo);

			if (taskInfo != nullptr && rc == 0)
			{
				extractDependencies(taskInfo, taskDependenciesList);
				rc = addNode(taskName, taskExecutionTime, taskDepNumber, taskDependenciesList);
			}

			if (rc == 0)
			{
				addNodeWithNoDep(taskName, taskDependenciesList);
			}

		}
	}

	return rc;
}

void topSortUtil(Node *node, std::queue<std::string> *nodesNoDependency)
{
	// for each node m with an edge e from node to m do
	auto it = node->outEdges.begin();
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
			(*nodesNoDependency).push(m->getName());
		}
	}
}

bool Scheduler::topologicalSort()
{
	while (!nodesNoDependency.empty())
	{
		// remove a node
		std::string nodeName = nodesNoDependency.front();
		nodesNoDependency.pop();

		auto it = this->nodeMap.find(nodeName);

		Node *node=it->second;

		sortednodeMap.insert(std::pair<std::string, Node*>(nodeName, node));

		topSortUtil(node, &nodesNoDependency);
	}

	//Check to see if all edges are removed
	bool cycle = false;

	for (auto &entry : nodeMap)
	{
		Node * node = entry.second;
		if (!node->inEdges.empty())
		{
			cycle = true;
			break;
		}
	}

	if (cycle)
	{
		printf("Error: Cycle present, topological sort not possible");
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
	for (auto &entry : nodeMap)
	{
        Node *node = (Node*)entry.second;
		auto nodeDependencies = node->getDependencies();
		
		for (auto currentNodeDependency: nodeDependencies)
		{
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
		Edge* edge = *it;
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
		if (distance[node] == INT_MIN)
		{
			maxDistance = std::max(maxDistance, getTotalDistanceFromNode(&distance, node));
		}
	}

	return maxDistance;
}
