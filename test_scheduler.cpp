#include "gtest/gtest.h"

#include "scheduler.h"

TEST(Scheduler, setGetN)
{
	Scheduler scheduler;
	
	scheduler.setN(10);

	EXPECT_EQ(10, scheduler.getN());
}

TEST(Scheduler, readFile)
{
	FILE *fin = NULL;
	int count = 0;
	fin = fopen("scheduler.in", "r+");
	if (fin == NULL)
	{
		printf("File not found\n");
		exit(0);
	}

	Scheduler scheduler(fin);

	EXPECT_EQ(0, scheduler.readFile());
	
	std::map<std::string, Node*> nodeMap = scheduler.getNodeMap();

	for (std::map<std::string, Node*>::iterator it=nodeMap.begin(); it!=nodeMap.end(); ++it)
	{
		switch(count)
		{
			case 0:
				EXPECT_STREQ(it->first.c_str(), "task1");
				EXPECT_STREQ( it->second->getName().c_str(), "task1");
				EXPECT_EQ(it->second->getExecutionTime(), 7);
				EXPECT_EQ(it->second->getDependenciesNumber(), 2);
				EXPECT_STREQ(it->second->getDependencies()[0].c_str(), "task2");
				EXPECT_STREQ(it->second->getDependencies()[1].c_str(), "task3");
				break;
			case 1:
				EXPECT_STREQ(it->first.c_str(), "task2");
				EXPECT_STREQ( it->second->getName().c_str(), "task2");
				EXPECT_EQ(it->second->getExecutionTime(), 1);
				EXPECT_EQ(it->second->getDependenciesNumber(), 0);
				break;
			case 2:
				EXPECT_STREQ(it->first.c_str(), "task3");
				EXPECT_STREQ( it->second->getName().c_str(), "task3");
				EXPECT_EQ(it->second->getExecutionTime(), 5);
				EXPECT_EQ(it->second->getDependenciesNumber(), 0);
				break;
		}
		count++;
	}

	fclose(fin);
}

TEST(Scheduler, buildGraph)
{
	FILE *f = NULL;
	int count = 0;	
	
	f = fopen("scheduler.in", "r+");
	if (f == NULL)
	{
		printf("File not found\n");
		exit(0);
	}

	Scheduler scheduler(f);
	
	EXPECT_EQ(0, scheduler.readFile());

	scheduler.buildGraph();

	std::map<std::string, Node*> nodeMap = scheduler.getNodeMap();

	for (std::map<std::string, Node*>::iterator it=nodeMap.begin(); it!=nodeMap.end(); ++it)
	{
		Node *node = (Node*)it->second;
		
		if (node->getName().compare("task1"))
		{
			for (std::list<Edge*>::iterator it=node->inEdges.begin(); it!=node->inEdges.end(); ++it)
			{
				Edge* edge = *it;
				switch(count)
				{
					case 0:
						EXPECT_STREQ(edge->from->getName().c_str(), "task2");
						EXPECT_STREQ(edge->to->getName().c_str(), "task1");
						break;
					case 1:
						EXPECT_STREQ(edge->from->getName().c_str(), "task3");
						EXPECT_STREQ(edge->to->getName().c_str(), "task1");
						break;
					count++;
				}
			}
		}
		else if (node->getName().compare("task2"))
		{
			for (std::list<Edge*>::iterator it=node->outEdges.begin(); it!=node->outEdges.end(); ++it)
			{
				Edge* edge = *it;
				EXPECT_STREQ(edge->from->getName().c_str(), "task2");
				EXPECT_STREQ(edge->to->getName().c_str(), "task1");
			}
		}
		else if (node->getName().compare("task3"))
		{
			for (std::list<Edge*>::iterator it=node->outEdges.begin(); it!=node->outEdges.end(); ++it)
			{
				Edge* edge = *it;
				EXPECT_STREQ(edge->from->getName().c_str(), "task3");
				EXPECT_STREQ(edge->to->getName().c_str(), "task1");
			}
		}

		if (node->getName().compare("task1") == 0)
		{
			EXPECT_STREQ(node->getDependencies()[0].c_str(), "task2");
			EXPECT_STREQ(node->getDependencies()[1].c_str(), "task3");
		}
	}
}

TEST(Scheduler, topologicalSort)
{
	FILE *f = NULL;
	int count = 0;	
	
	f = fopen("scheduler.in", "r+");
	if (f == NULL)
	{
		printf("File not found\n");
		exit(0);
	}

	Scheduler scheduler(f);

	EXPECT_EQ(0, scheduler.readFile());

	scheduler.buildGraph();
	
	EXPECT_TRUE(scheduler.topologicalSort());

	std::map<std::string, Node*> sortedList = scheduler.getSortedList();

	for (std::map<std::string, Node*>::iterator it=sortedList.begin(); it!=sortedList.end(); ++it)
	{
		switch (count)
		{
			case 1:
					EXPECT_STREQ(it->second->getName().c_str(), "task1");
					break;
			case 2:
					EXPECT_STREQ(it->second->getName().c_str(), "task2");
					break;
			case 3:
					EXPECT_STREQ(it->second->getName().c_str(), "task3");
					break;
		}
	}

	fclose(f);
}

TEST(Scheduler, calculateMinTimeScheduling)
{
	FILE *f = NULL;
	f = fopen("scheduler.in", "r+");
	if (f == NULL)
	{
		printf("File not found\n");
		exit(0);
	}

	Scheduler scheduler(f);

	EXPECT_EQ(0, scheduler.readFile());
	
	EXPECT_EQ(12, scheduler.calculateMinTimeScheduling());

	fclose(f);
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv); 
    RUN_ALL_TESTS();
	//std::getchar(); // keep console window open until Return keystroke

	return 0;
}
