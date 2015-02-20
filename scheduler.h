#ifndef SCHEDULER_H
#define SCHEDULER_H
#pragma once

#include <iostream>
#include "edge.h"
#include "node.h"
#include<map>
#include<set>
#include<queue>

class Scheduler {

	private:
		int n;
		std::map<std::string, Node*> nodeMap;
		std::queue<std::string> nodesNoDependent;
		std::map<std::string, Node*> sortednodeMap;
		FILE *inputFile;
	
	public:
		Scheduler();
		Scheduler(FILE* inputFile);

		/**
		 * @brief Read file
		 */
		void readFile();

		/**
		 * @brief Set number of tasks
		 */
		void setN(int n);

		/**
		 * @brief Get number of tasks
		 * @return the number of tasks
		 */
		int getN();

		/**
		 * @brief Topological sort
		 * @return true if topological sort can be perfomed, false otherwise
		 */
		bool topologicalSort();

		/**
		 * @brief Get sorted node list determined by topological sort
		 * @return sorted node list
		 */
		std::map<std::string, Node*> getSortedList();

		/**
		 * @brief Get node list
		 * @return node list
		 */
		std::map<std::string, Node*> getNodeMap();

		/**
		 * @brief search what dependencies has the node
		 * @param node - node for which the search is performed
		 * @param currentNodeDependency
		 */
		void searchDependencyNode(Node *node, std::string currentNodeDependency);
		
		/**
		 * @brief Builds the graph
		 */
		void buildGraph();
		
		/**
		 * @brief Calculate the minimum time of scheduling
		 * @return the minim time of scheduling
		 */
		int calculateMinTimeScheduling();
		
		/**
		 * @brief Get total distance from source node
		 * @param distance - map between node and distance
		 * @param sourceNode - the source node where to start calculating the distance
		 * @return total distance from source node
		 */
		int getTotalDistanceFromNode(std::map<Node*, int> *distance, Node *sourceNode);
};
#endif
