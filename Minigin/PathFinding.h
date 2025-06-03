#pragma once
#include <map>
#include <vector>
#include <list>
#include <string>
#include <memory>
#include "Vector2.h"
#include "GraphEditor.h"


namespace Engine
{
	struct GraphNode;
	struct NavigationNode {
		Vector2 Position;
		Vector2 Direction;
	};

	class PathFinding final
	{
	public:
		PathFinding();
		~PathFinding() = default;
		
		const std::list<NavigationNode> FindPath(const std::string& graphName, const Engine::Vector2& postion, const Engine::Vector2& end);
		const std::list<NavigationNode> FindPath(Graph* const graph, const Engine::Vector2& postion, const Engine::Vector2& end);
		Graph* const GetGraph(const std::string& graphName);

	private:
		
		bool LoadGraph(const std::string& graphName,Graph* loadedGraph);
		GraphNode* GraphNodeByPosition(Graph* const graph,const Engine::Vector2 position);
		GraphNode* ClosestNeighbour(const std::vector<GraphNode*>& neighbours, GraphNode* node);
		std::vector<GraphNode*> GetNeighbours(Graph* const graph, const GraphNode* node);
		

		std::map <std::string, std::unique_ptr<Graph>> m_LoadedGraphs;
	};
}

