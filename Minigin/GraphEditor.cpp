#include "GraphEditor.h"
#include "imgui.h"
#include "Renderer.h"
#include "json.hpp"
#include <fstream>
#include "GrapEditorCommands.h"
#include "InputManager.h"
#include <SDL.h>

using namespace nlohmann;

Engine::GraphEditor::GraphEditor() :
	m_Active{ true },
	m_ShowNodes{ true },
	m_DrawNodes{ true },
	m_FilePath{ "graph.json" },
	m_Graph{ std::make_unique<Graph>() },
	m_SelectedNode{ nullptr }
{
	auto openGraphEditorCommand = std::make_unique<OpenGraphEditor>();
	InputManager::GetInstance().BindButton(0, SDL_SCANCODE_F1, std::move(openGraphEditorCommand));

	auto addGraphNodeCommand = std::make_unique<AddGraphNodeCommand>();
	InputManager::GetInstance().BindButton(0, SDL_SCANCODE_INSERT, std::move(addGraphNodeCommand));

	auto deleteGraphNodeCommand = std::make_unique<DeleteGraphNodeCommand>();
	InputManager::GetInstance().BindButton(0, SDL_SCANCODE_DELETE, std::move(deleteGraphNodeCommand));

	auto saveGraphCommand = std::make_unique<SaveGraphCommand>();
	InputManager::GetInstance().BindButton(0, SDL_SCANCODE_SPACE, std::move(saveGraphCommand));
}

Engine::GraphEditor::~GraphEditor()
{

}

void Engine::GraphEditor::GUI()
{
	if (!m_Active) 
	{
		return;
	}

	ImGui::Begin("Graph Editor");
	ImGui::SeparatorText("Save & Load");
	if (ImGui::Button("Save Graph"))
	{
		SaveGraph();
	}


	if (ImGui::Button("Load Graph"))
	{
		LoadGraph(m_FilePath);
	}

	char* buffer = new char[256];
	strncpy_s(buffer, 256, m_FilePath.c_str(), m_FilePath.size() + 1);

	if (ImGui::InputText("File Path", buffer, 256))
	{
		m_FilePath = buffer;
	}
	
	delete[] buffer;


	ImGui::Checkbox("Show Nodes", &m_ShowNodes);
	ImGui::Checkbox("Draw Nodes", &m_DrawNodes);

	// Edit selected node's data
	if (m_SelectedNode)
	{
		ImGui::SeparatorText("Selected Node");

		ImGui::Text("Node %d", m_SelectedNode->Index);

		float pos[2] = { m_SelectedNode->Position.x, m_SelectedNode->Position.y };
		if (ImGui::InputFloat2("Position", pos, "%.1f"))
		{
			m_SelectedNode->Position.x = pos[0];
			m_SelectedNode->Position.y = pos[1];
		}

		if (ImGui::Button("Delete Node"))
		{
			m_Graph->RemoveNode(m_SelectedNode);
			m_SelectedNode = nullptr;
		}

		if (ImGui::Button("Clear Connections"))
		{
			// Clear this node's connections
			for (auto other : m_SelectedNode->Connections)
			{
				// Also remove this node from others' connections
				auto neighBourNode = m_Graph->GetNodeByIndex(other);
				neighBourNode->Connections.clear();

			}
			m_SelectedNode->Connections.clear();
		}
	}
	else
	{
		ImGui::Text("No node selected");
	}

	ImGui::SeparatorText("Node Creation");

	if (ImGui::Button("Add Node"))
	{
		m_Graph->AddNode(new GraphNode
			{
				0,
				Vector2{0, 0},
				{}
			});
	}

	// Right-click anywhere in the window to deselect current node
	if (ImGui::IsMouseClicked(1)) // Right mouse button
	{
		m_SelectedNode = nullptr;
	}
	CheckNodeSelection();

	DrawNodes();

	ImGui::End();
}

void Engine::GraphEditor::Draw()
{
	if (!m_Active) return;
	if (!m_DrawNodes) return;

	auto nodes = m_Graph->GetNodes();
	for (auto node : nodes)
	{
		if (!node) continue;

		Renderer::GetInstance().SetColor({ 255,0,0,0 });
		Renderer::GetInstance().RenderFilledOval(node->Position, 5, 5);

		auto connections = node->Connections;
		for (auto connection : connections)
		{
			auto neighBourNode = m_Graph->GetNodeByIndex(connection);
			if (!neighBourNode) continue;

			Renderer::GetInstance().SetColor({ 0,0,255,0 });
			Renderer::GetInstance().RenderLine(node->Position, neighBourNode->Position);
		}
	}
}

void Engine::GraphEditor::CreateNode()
{
	if (!m_Active) return;

	Vector2 mousePos{ ImGui::GetMousePos().x, ImGui::GetMousePos().y };
	Engine::GraphNode* newNode = new Engine::GraphNode{
		0,
		mousePos,
		{}
	};
	m_Graph->AddNode(newNode);
}

void Engine::GraphEditor::DeleteNode()
{
	if (!m_Active) return;

	if (m_SelectedNode)
	{
		m_Graph->RemoveNode(m_SelectedNode);
		m_SelectedNode = nullptr;
	}
}

void Engine::GraphEditor::SaveGraph()
{
	auto nodes = m_Graph->GetNodes();

	json graphData;

	graphData["graph_offset_index"] = m_Graph->GetIndex();
	graphData["graph_nodes"] = json::array();
	for (auto node : nodes)
	{
		json nodeData;
		nodeData["node_index"] = node->Index;
		nodeData["node_position"] = { node->Position.x, node->Position.y };
		nodeData["node_connections"] = json::array();
		for (auto connection : node->Connections)
		{
			nodeData["node_connections"].push_back(connection);
		}
		graphData["graph_nodes"].push_back(nodeData);
	}

	std::ofstream file(m_FilePath);
	if (file.is_open())
	{
		file << graphData.dump(4); // Pretty print with 4 spaces
		file.close();
	}
}


void Engine::GraphEditor::LoadGraph(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		throw std::runtime_error("Could not open graph file");
	}

	

	json graphData;
	file >> graphData;
	int offsetIndex = graphData["graph_offset_index"];

	std::vector<GraphNode*> nodes;

	for (const auto& nodeData : graphData["graph_nodes"])
	{
		auto node = new GraphNode();
		node->Index = nodeData["node_index"];
		node->Position.x = nodeData["node_position"][0];
		node->Position.y = nodeData["node_position"][1];

		for (const auto& connection : nodeData["node_connections"])
		{
			node->Connections.push_back(connection);
		}

		nodes.emplace_back(node);
	}
	m_Graph = std::make_unique<Graph>(offsetIndex,nodes);
	
}

void Engine::GraphEditor::DrawNodes()
{
	if (m_ShowNodes)
	{
		ImGui::BeginChild("Node List", ImVec2(0, 300), true);

		auto nodes = m_Graph->GetNodes();

		for (auto node : nodes)
		{
			ImGui::PushID(node->Index);

			ImGui::Text("Node %d", node->Index);
			ImGui::Text("Pos: (%.1f, %.1f)", node->Position.x, node->Position.y);

			if (m_SelectedNode == node)
			{
				ImGui::SameLine();
				ImGui::Text("<< Selected");
			}

			// Show connections
			ImGui::Text("Connections:");
			auto& connections = node->Connections;
			for (auto conn : connections)
			{
				ImGui::BulletText("-> Node %d ", conn);
				ImGui::SameLine();
				if (ImGui::Button("Remove"))
				{
					node->Connections.erase(std::remove(node->Connections.begin(), node->Connections.end(), conn), node->Connections.end());
				}
			}

			ImGui::Separator();
			ImGui::PopID();
		}

		ImGui::EndChild();


	}
}

void Engine::GraphEditor::CheckNodeSelection()
{
	// Get current mouse position in screen coordinates
	ImVec2 mousePos = ImGui::GetMousePos();

	// We'll convert ImVec2 to your Vector2 (assuming compatible float types)
	Vector2 mouse = { mousePos.x, mousePos.y };

	auto nodes = m_Graph->GetNodes();

	for (auto node : nodes)
	{
		if (!node) continue;
		// Check if mouse is within a small radius of the node
		float dx = mouse.x - node->Position.x;
		float dy = mouse.y - node->Position.y;
		float distSquared = dx * dx + dy * dy;

		const float nodeRadius = 5.0f;
		const float selectRadiusSquared = nodeRadius * nodeRadius;

		// If mouse is within node radius
		if (distSquared < selectRadiusSquared)
		{
			if (ImGui::IsMouseClicked(0))
			{
				if (m_SelectedNode == nullptr)
				{
					// Select this node
					m_SelectedNode = node;
				}
				else if (m_SelectedNode != node)
				{
					// Connect both nodes bidirectionally
					auto& fromConns = m_SelectedNode->Connections;
					if (std::find(fromConns.begin(), fromConns.end(), node->Index) == fromConns.end())
					{
						fromConns.emplace_back(node->Index);
					}

					auto& toConns = node->Connections;
					if (std::find(toConns.begin(), toConns.end(), m_SelectedNode->Index) == toConns.end())
					{
						toConns.emplace_back(m_SelectedNode->Index);
					}

					// Deselect after connection
					m_SelectedNode = nullptr;
				}
			}
			else if (ImGui::IsMouseDown(0) && m_SelectedNode == node)
			{
				// Move node with mouse
				m_SelectedNode->Position = mouse;
			}

			break; // Prevent selecting multiple nodes in one frame
		}
	}
}

void Engine::GraphNode::GUI()
{
	ImGui::Text("Node %d", Index);

	// Position editor (text input)
	ImGui::PushID(Index); // Prevent ID conflicts
	float pos[2] = { Position.x, Position.y };
	if (ImGui::InputFloat2("Position", pos, "%.1f"))
	{
		Position.x = pos[0];
		Position.y = pos[1];
	}
	ImGui::PopID();

	// Connections
	ImGui::BeginChild("Node Connections", ImVec2(0, 100), true);
	for (auto connection : Connections)
	{
		ImGui::Text("-> Node %d at (%.1f, %.1f)", connection);
	}
	ImGui::EndChild();
}

Engine::Graph::~Graph()
{
	for (auto node : m_Nodes)
	{
		delete node; // Clean up memory
	}
	m_Nodes.clear();
}

void Engine::Graph::AddNode(GraphNode* node)
{
	node->Index = Index;
	m_Nodes.emplace_back(node);
	++Index;
}

void Engine::Graph::RemoveNode(GraphNode* node)
{
	auto it = std::remove(m_Nodes.begin(), m_Nodes.end(), node);
	if (it != m_Nodes.end())
	{
		for (auto otherNode : m_Nodes)
		{
			auto& connections = otherNode->Connections;
			otherNode->Connections.erase(std::remove(connections.begin(), connections.end(), otherNode->Index), connections.end());
		}

		m_Nodes.erase(it, m_Nodes.end());
		delete node; // Clean up memory
	}
}

Engine::GraphNode* Engine::Graph::GetNodeByIndex(int index)
{
	auto  it = std::find_if(m_Nodes.begin(), m_Nodes.end(), [index](GraphNode* node) { return node->Index == index; });
	return (it != m_Nodes.end()) ? *it : nullptr;
}
