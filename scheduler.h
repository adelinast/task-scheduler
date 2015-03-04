#ifndef SCHEDULER_H
#define SCHEDULER_H
#pragma once

#include <iostream>
#include "edge.h"
#include "node.h"
#include <map>
#include <set>
#include <queue>
#include <unordered_map>

class Scheduler {

	public:
		Scheduler();
		
		Scheduler(FILE* inputFile);

		~Scheduler();

		/**
		 * @brief Read file
		 */
		int readFile();

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
		 * @return true if topological sort can be performed, false otherwise
		 */
		bool topologicalSort();

		/**
		 * @brief Get sorted node map determined by topological sort
		 * @return sorted node map
		 */
		std::map<std::string, Node*> getSortedList();

		/**
		 * @brief Get node map
		 * @return node map
		 */
		std::unordered_map<std::string, Node*> getNodeMap();

	
		/**
		 * @brief Builds the graph
		 */
		void buildGraph();
		
		/**
		 * @brief Calculate the minimum time of scheduling
		 * @return the minimum time of scheduling
		 */
		int calculateMinTimeScheduling();

	private:

		/**
		 * @brief Add a node to nodeMap
		 * @param taskName - name of the task
		 * @param taskExecutionTime - the time needed to run the task
		 * @param taskDepNumber - the number of dependencies
		 * @param taskDependenciesList - the list of task dependencies
		 */
		int addNode(std::string taskName, int taskExecutionTime, size_t taskDepNumber, std::vector<std::string> taskDependenciesList);

		/**
		 * @brief Add a node with no dependencies to nodeSet
		 * @param taskName - name of the task
		 * @param taskDependenciesList - the list of task dependencies
		 */
		void addNodeWithNoDep(std::string taskName, std::vector<std::string> taskDependenciesList);
		
		/**
		 * @brief search what dependencies has the node
		 * @param node - node for which the search is performed
		 * @param currentNodeDependency - the current node name
		 */
		void searchDependencyNode(Node *node, std::string currentNodeDependency);
		
		/**
		 * @brief Get total distance from source node
		 * @param distance - map between node and distance
		 * @param sourceNode - the source node where to start calculating the distance
		 * @return total distance from source node
		 */
		int getTotalDistanceFromNode(std::map<Node*, int> *distance, Node *sourceNode);

		int n;

		std::unordered_map<std::string, Node*> nodeMap;

		std::queue<std::string> nodesNoDependency;

		std::map<std::string, Node*> sortednodeMap;

		FILE *inputFile;
};
#endif
