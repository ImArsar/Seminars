#include <iostream>
#include <vector>
#include <algorithm>


class Graph
{
protected:
	size_t vertex_count_, edge_count_;
	bool is_directed_;

public:
	typedef size_t Vertex;
	Graph(size_t vertex_count, bool is_directed) : vertex_count_(vertex_count), is_directed_(is_directed), edge_count_(0) {}

	size_t getVertexCount() const
	{
		return vertex_count_;
	}

	size_t getEdgeCount() const
	{
		if (is_directed_ == 0)
		{
			return edge_count_ / 2;
		}
		return edge_count_;
	}

	bool isDirected() const
	{
		return is_directed_;
	}

	virtual void addEdge(const Vertex& start, const Vertex& finish) = 0;
	virtual size_t getVertexDeg(const Vertex& vertex) const = 0;
	virtual std::vector<std::vector<Vertex>> getAdjList() const = 0;
	virtual std::vector<Vertex> getNeighbors(const Vertex& v) const = 0;
};


class GraphAdjList : public Graph
{
private:
	std::vector<std::vector<Vertex>> adj_list_;

public:
	GraphAdjList(size_t vertex_count, bool is_directed) : Graph(vertex_count, is_directed), adj_list_(vertex_count + 1) {}

	void addEdge(const Vertex& start, const Vertex& finish) override
	{
		adj_list_[start].push_back(finish);
		if (!is_directed_)
		{
			adj_list_[finish].push_back(start);
		}
		edge_count_++;
	}

	size_t getVertexDeg(const Vertex& vertex) const override
	{
		return adj_list_[vertex].size();
	}

	std::vector<std::vector<Vertex>> getAdjList() const override
	{
		return adj_list_;
	}

	std::vector<Vertex> getNeighbors(const Vertex& vertex) const override
	{
		return adj_list_[vertex];
	}
};


namespace GraphProcessing
{
	enum VertexMark { WHITE, GREY, BLACK };

	bool DFS_visit_isCycleGraph(const Graph& graph, const Graph::Vertex& vertex, std::vector<VertexMark>& vertex_marks)
	{
		vertex_marks[vertex] = GREY;
		std::vector<Graph::Vertex> neighbors = graph.getNeighbors(vertex);
		for (Graph::Vertex u : neighbors)
		{
			if (vertex_marks[u] == WHITE)
			{
				if (DFS_visit_isCycleGraph(graph, u, vertex_marks))
				{
					return true;
				}
			}
			else if (vertex_marks[u] == GREY)
			{
				return true;
			}
		}
		vertex_marks[vertex] = BLACK;
		return false;
	}

	bool isCycleGraph(const Graph& graph)
	{
		std::vector<VertexMark> vertex_marks(graph.getVertexCount() + 1, WHITE);
		for (Graph::Vertex u = 1; u < graph.getVertexCount() + 1; ++u)
		{
			if (vertex_marks[u] == WHITE)
			{
				if (DFS_visit_isCycleGraph(graph, u, vertex_marks))
				{
					return true;
				}
			}
		}
		return false;
	}

	void DFS_visit_TopSort(const Graph& graph, const Graph::Vertex& vertex, std::vector<bool>& used, std::vector<Graph::Vertex>& vertices_in_right_order)
	{
		used[vertex] = true;
		std::vector<Graph::Vertex> neighbors = graph.getNeighbors(vertex);
		for (Graph::Vertex u : neighbors)
		{
			if (!used[u])
			{
				DFS_visit_TopSort(graph, u, used, vertices_in_right_order);
			}
		}
		vertices_in_right_order.push_back(vertex);
	}

	std::vector<Graph::Vertex> TopSort(const Graph& graph)
	{
		if (!graph.isDirected() || isCycleGraph(graph))
		{
			return {};
		}
		std::vector<bool> used(graph.getVertexCount() + 1, false);
		std::vector<Graph::Vertex> vertices_in_right_order;
		for (Graph::Vertex u = 1; u < graph.getVertexCount() + 1; ++u)
		{
			if (!used[u])
			{
				DFS_visit_TopSort(graph, u, used, vertices_in_right_order);
			}
		}
		std::reverse(vertices_in_right_order.begin(), vertices_in_right_order.end());
		return vertices_in_right_order;
	}
}


int main()
{
	size_t n, m;
	std::cin >> n >> m;
	GraphAdjList graph_adj_list(n, true);
	for (Graph::Vertex i = 1; i < m + 1; ++i)
	{
		Graph::Vertex A, B;
		std::cin >> A >> B;
		graph_adj_list.addEdge(A, B);
	}
	std::vector<Graph::Vertex> vertices_in_right_order = GraphProcessing::TopSort(graph_adj_list);
	if (vertices_in_right_order.empty())
	{
		std::cout << "No";
	}
	else
	{
		std::cout << "Yes" << std::endl;
		for (Graph::Vertex i : vertices_in_right_order)
		{
			std::cout << i << ' ';
		}
	}
	system("PAUSE");
	return 0;
}