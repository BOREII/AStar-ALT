#pragma once

#include <cstdint>
#include <deque>
#include <memory>
#include <utility>
#include <random>

struct Edge {
	uint64_t id, from, to, distance;
};

class Graph {
public:
	using EdgePtr = std::shared_ptr<Edge>;

	explicit Graph(uint64_t n_vertices);
	Graph(const Graph&) = default;
	Graph(Graph&&) noexcept = default;

	void addEdge(uint64_t from, uint64_t to, uint64_t distance);
	void addEdge(EdgePtr&& edge_ptr);

	const std::deque<uint64_t>& getOutgoingEdgesIds(uint64_t vertex_index) const;
	const Edge& getEdgePropertiesById(uint64_t edge_id) const;
	Edge& getEdgePropertiesById(uint64_t);
	const uint64_t& getHeuristic(uint64_t vertex_index) const ;
	uint64_t& getHeuristic(uint64_t vertex_index);

	uint64_t getVerticesCount() const;

private:
	uint64_t n_vertices_;
	std::deque<std::deque<uint64_t>> outgoing_edges_ids_;
	std::deque<EdgePtr> edges_properties_;
	std::deque<uint64_t> vertex_heuristics_;
};

Graph::Graph(uint64_t n_vertices)
	: n_vertices_(n_vertices)
	, outgoing_edges_ids_(n_vertices)
	, edges_properties_(0ul)
	, vertex_heuristics_(n_vertices) {
}

void Graph::addEdge(std::shared_ptr<Edge>&& edge_ptr) {
	auto edge_id = static_cast<uint64_t>(edges_properties_.size());
	edge_ptr->id = edge_id;
	outgoing_edges_ids_[edge_ptr->from].emplace_back(edge_id);
	edges_properties_.emplace_back(std::move(edge_ptr));
}

void Graph::addEdge(uint64_t from, uint64_t to, uint64_t distance) {
	auto edge_ptr = std::make_shared<Edge>();
	edge_ptr->from = from;
	edge_ptr->to = to;
	edge_ptr->distance = distance;
	addEdge(std::move(edge_ptr));
}

const std::deque<uint64_t>& Graph::getOutgoingEdgesIds(uint64_t vertex_index) const {
	if (vertex_index >= n_vertices_) {
		throw std::out_of_range("Vertex index must be less than " + std::to_string(n_vertices_));
	}
	return outgoing_edges_ids_[vertex_index];
}

const Edge& Graph::getEdgePropertiesById(uint64_t edge_id) const {
	return *edges_properties_[edge_id];
}

Edge& Graph::getEdgePropertiesById(uint64_t edge_id) {
	return *edges_properties_[edge_id];
}


uint64_t Graph::getVerticesCount() const {
	return n_vertices_;
}

const uint64_t& Graph::getHeuristic(uint64_t vertex_index) const {
	if (vertex_index >= n_vertices_) {
		throw std::out_of_range("Vertex index must be less than " + std::to_string(n_vertices_));
	}
	return vertex_heuristics_[vertex_index];
}

uint64_t& Graph::getHeuristic(uint64_t vertex_index) {
	if (vertex_index >= n_vertices_) {
		throw std::out_of_range("Vertex index must be less than " + std::to_string(n_vertices_));
	}
	return vertex_heuristics_[vertex_index];
}