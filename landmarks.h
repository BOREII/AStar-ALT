#pragma once

#include <random>
#include <deque>

#include "graph.h"
#include "dijkstra.h"

struct Landmark {
	uint64_t vertex_id;
	std::deque<uint64_t> distances;
};

template<uint8_t NumberOfLandmarks>
std::deque<Landmark> PickLandmarksRandomly(const Graph& graph) {
	std::deque<Landmark> result;
	std::random_device rd;
	std::uniform_int_distribution<uint64_t> vertex_distribution(0ul, graph.getVerticesCount() - 1);

	while (result.size() < NumberOfLandmarks) {
		uint64_t random_vertex = vertex_distribution(rd);
		bool duplicate_found = false;

		for (const auto& landmark : result) {
			if (random_vertex == landmark.vertex_id) {
				duplicate_found = true;
				break;
			}
		}

		if (duplicate_found) {
			continue;
		}

		Landmark new_landmark = {random_vertex, Dijkstra<4u>(graph, random_vertex)};
		result.emplace_back(new_landmark);
	}

	return result;
}

void ComputeVertexHeuristic(
	Graph& graph, const std::deque<Landmark>& landmarks, uint64_t destination) {
	for (uint64_t curr_vertex = 0; curr_vertex < graph.getVerticesCount(); ++curr_vertex) {
		for (const Landmark& landmark : landmarks) {
			uint64_t new_heuristic =
				landmark.distances[destination] > landmark.distances[curr_vertex]
				? landmark.distances[destination] - landmark.distances[curr_vertex]
				: landmark.distances[curr_vertex] - landmark.distances[destination];

			graph.getHeuristic(curr_vertex) =
				std::max(graph.getHeuristic(curr_vertex), new_heuristic);
		}
	}
}

