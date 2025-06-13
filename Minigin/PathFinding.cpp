#include "PathFinding.h"
#include <unordered_set>
#include <algorithm>


using namespace Engine;

Engine::PathFinding::PathFinding()
{
}

const std::list<NavigationNode> Engine::PathFinding::FindPath(const std::string& graphName, const Engine::Vector2& postion, const Engine::Vector2& end)
{
	return FindPath(GetGraph(graphName),postion,end);
}

const std::list<NavigationNode> Engine::PathFinding::FindPath(Graph* const graph, const Engine::Vector2& postion, const Engine::Vector2& end)
{
	std::list<NavigationNode> path{};
	const auto startNode{ GraphNodeByPosition(graph,postion) };
	const auto endNode{ GraphNodeByPosition(graph,end) };

	GraphNode* currentNode{ graph->GetNodeByIndex(startNode->Index)};
	int iteration{ 0 };
	while (currentNode != endNode)
	{
		if (currentNode == nullptr) break;

		const auto neighbours{ GetNeighbours(graph,currentNode) };
		if (neighbours.size() == 0) break;

		GraphNode* nextNode{ neighbours[0]};
		float closestDistanceToEnd{ std::numeric_limits<float>::max() };

		for (const auto& neighbour : neighbours)
		{
			if (neighbour == nullptr) continue;

			const float distanceToEnd{ Vector2::Distance(neighbour->Position,endNode->Position) };
			if (distanceToEnd < closestDistanceToEnd)
			{
				nextNode = neighbour;
				closestDistanceToEnd = distanceToEnd;
			}
		}

		const float distanceCurrentToEnd{ Vector2::Distance(currentNode->Position,endNode->Position) };
		if (closestDistanceToEnd > distanceCurrentToEnd)
		{
			return path;
		}

		Vector2 direction{ (nextNode->Position - currentNode->Position).Normalize() };
		if (std::abs(direction.x) > std::abs(direction.y))
		{
			direction.x = direction.x > 0 ? 1.0f : -1.0f;
			direction.y = 0.0f;
		}
		else
		{
			direction.y = direction.y > 0 ? 1.0f : -1.0f;
			direction.x = 0.0f;
		}

		path.emplace_back(NavigationNode
			{
				currentNode->Position,
				direction
			});
		currentNode = nextNode;
		++iteration;
		const int length{ static_cast<int>(graph->GetNodes().size()) };
		if (iteration > length)
		{
			return {};
		}
	}

	path.emplace_back(NavigationNode
		{
			currentNode->Position,
			{}
		});

	return path;
	
}

Graph* const Engine::PathFinding::GetGraph(const std::string& graphName)
{
	auto it{ m_LoadedGraphs.find(graphName) };
	if (it == m_LoadedGraphs.end())
	{
		Graph* loadedGraph{ LoadGraph(graphName) };
		if (loadedGraph != nullptr)
		{
			return loadedGraph;
		}
	}
	

	return m_LoadedGraphs.at(graphName).get();
}

Graph* Engine::PathFinding::LoadGraph(const std::string& graphName)
{
	auto graph{ Graph::LoadGraph(graphName) };
	if (graph == nullptr) return nullptr;

	auto loadedGraph = graph.get();
	m_LoadedGraphs.emplace(graphName, std::move(graph));
	return loadedGraph;
}

GraphNode* Engine::PathFinding::GraphNodeByPosition(Graph* const graph, const Engine::Vector2 position)
{
	GraphNode* closestNode{ nullptr };
	float closestDistance{ std::numeric_limits<float>::max() };
	const auto& allNodes{ graph->GetNodes() };
	std::for_each(allNodes.begin(), allNodes.end(), [&closestNode,&closestDistance,&position](GraphNode* node)
		{
			float distance{ Vector2::Distance(node->Position,position) };
			if (distance < closestDistance)
			{
				closestDistance = distance;
				closestNode = node;
			}
		});
	return closestNode;
}

GraphNode* Engine::PathFinding::ClosestNeighbour(const std::vector<GraphNode*>& neighbours, GraphNode* node)
{
	GraphNode* closestNode{ nullptr };
	float closestDistance{ std::numeric_limits<float>::max() };
	std::for_each(neighbours.begin(), neighbours.end(), [&closestNode, &closestDistance,&node](GraphNode* n)
		{
			float distance{ Vector2::Distance(n->Position,node->Position) };
			if (distance < closestDistance)
			{
				closestDistance = distance;
				closestNode = node;
			}
		});
	return closestNode;
}

std::vector<GraphNode*> Engine::PathFinding::GetNeighbours(Graph* const graph, const GraphNode* node)
{
	const auto& connections{ node->Connections };
	std::vector<GraphNode*> neighbours{};
	neighbours.reserve(connections.size());

	for (const auto& connection : connections)
	{
		auto neighbour{ graph->GetNodeByIndex(connection) };
		neighbours.emplace_back(neighbour);
	}

	return neighbours;
}
