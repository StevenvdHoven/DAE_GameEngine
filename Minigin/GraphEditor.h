#pragma once
#include <vector>
#include "Vector2.h"
#include <string>
#include <memory>

namespace Engine
{
	struct GraphNode
	{
		int Index;
		Vector2 Position;
		std::vector<int> Connections;

		void GUI();
	};

	class Graph final
	{
	public:
		Graph() = default;
		Graph(int indexOffset,std::vector<Engine::GraphNode*> nodes): 
			Index{indexOffset},
			m_Nodes(std::move(nodes))
		{}
		~Graph();

		void AddNode(GraphNode* node);
		void RemoveNode(GraphNode* node);

		Engine::GraphNode* GetNodeByIndex(int index);
		int GetIndex() const { return Index; }
		std::vector<Engine::GraphNode*> GetNodes() const { return m_Nodes; }

		static std::unique_ptr<Graph> LoadGraph(const std::string& filePath);
	private:
		int Index;
		std::vector<Engine::GraphNode*> m_Nodes;

	};

	class GraphEditor final
	{
	public:
		GraphEditor();
		~GraphEditor();

		void GUI();
		void Draw();

		

		void CreateNode();
		void DeleteNode();

		void SaveGraph();
		void LoadGraph(const std::string& filePath);

		void SetActive(bool active) { m_Active = active; }
		bool IsActive() const { return m_Active; }

	private:
		void DrawNodes();
		void CheckNodeSelection();

		bool m_Active;
		bool m_ShowNodes;
		bool m_DrawNodes;
		std::string m_FilePath;
		std::unique_ptr<Graph> m_Graph;
		Engine::GraphNode* m_SelectedNode;
	};
}



