#pragma once

#include "dijkstra.h"
#include "profile.h"
#include "graph.h"
#include "alt.h"
#include "landmarks.h"
#include "map_building.h"

// Тестируем время работы, вызываем Дейкстру и ALT из нескольких случайных вершин
void TestTimeNY() {
	Graph graph = BuildMap("USA-road-t.NY.txt");
	const int VERTEX_NUMBER = 5;

	std::random_device rd;
	std::uniform_int_distribution<uint64_t> vertex_distribution(0ul, graph.getVerticesCount() - 1);

	std::vector<uint64_t> random_sources(VERTEX_NUMBER), random_dests(VERTEX_NUMBER);
	for (int i = 0; i < VERTEX_NUMBER; ++i) {
		random_sources[i] = vertex_distribution(rd);
		random_dests[i] = vertex_distribution(rd);
	}


	{
		LOG_DURATION("New York: Dijkstra")
		for (int i = 0; i < VERTEX_NUMBER; ++i) {
			Dijkstra<4>(graph, random_sources[i]);
		}
	}

	std::deque<Landmark> landmarks;
	{
		LOG_DURATION("New York: Landmarks")
		landmarks = PickLandmarksRandomly<16>(graph);
	}

	{   // Считаем только работу ALT
		TotalDuration ny_alt("New York: ALT");
		for (int i = 0; i < VERTEX_NUMBER; ++i) {
			ComputeVertexHeuristic(graph, landmarks, random_dests[i]);
			ALT<4>(graph, random_sources[i], random_dests[i], ny_alt);
		}
	}
}

void TestTimeBAY() {
	Graph graph = BuildMap("USA-road-t.BAY.txt");
	const int VERTEX_NUMBER = 5;

	std::random_device rd;
	std::uniform_int_distribution<uint64_t> vertex_distribution(0ul, graph.getVerticesCount() - 1);

	std::vector<uint64_t> random_sources(VERTEX_NUMBER), random_dests(VERTEX_NUMBER);
	for (int i = 0; i < VERTEX_NUMBER; ++i) {
		random_sources[i] = vertex_distribution(rd);
		random_dests[i] = vertex_distribution(rd);
	}


	{
		LOG_DURATION("San Francisco Bay Area: Dijkstra")
		for (int i = 0; i < VERTEX_NUMBER; ++i) {
			Dijkstra<4>(graph, random_sources[i]);
		}
	}

	std::deque<Landmark> landmarks;
	{
		LOG_DURATION("San Francisco Bay Area: Landmarks")
		landmarks = PickLandmarksRandomly<16>(graph);
	}

	{   // Считаем только работу ALT
		TotalDuration ny_alt("San Francisco Bay Area: ALT");
		for (int i = 0; i < VERTEX_NUMBER; ++i) {
			ComputeVertexHeuristic(graph, landmarks, random_dests[i]);
			ALT<4>(graph, random_sources[i], random_dests[i], ny_alt);
		}
	}
}
