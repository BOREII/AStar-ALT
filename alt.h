#pragma once

#include "graph.h"
#include "priority_queue.h"
#include "profile.h"

// Принимает на вход граф, начальную и конечные вершины
// Возвращает расстоянние до конечной вершины
template<uint64_t NumberOfChildren>
uint64_t ALT (const Graph& graph, uint64_t source, uint64_t destination) {
	std::deque<uint64_t> distances(graph.getVerticesCount());

	PriorityQueue<NumberOfChildren> priorityQueue(graph.getVerticesCount());
	priorityQueue.MakeVertexActive(source);
	priorityQueue.SetDistance(source, 0lu);

	while (!priorityQueue.Empty()) {
		// Извлекаем очередную вершину с минимальным расстоянием
		uint64_t current_min_vertex = priorityQueue.RemoveMin();
		uint64_t current_distance = priorityQueue.GetDistanceToVertex(current_min_vertex);

		// Записываем расстояние до нее
		distances[current_min_vertex] = current_distance;

		// Делаем релаксацию, если расстояние до вершины вообще было найдено,
		// то есть, что она лежит в данной компоненте связности
		if (current_distance != std::numeric_limits<uint64_t>::max()) {
			for (uint64_t edge_id : graph.getOutgoingEdgesIds(current_min_vertex)) {
				const auto edge = graph.getEdgePropertiesById(edge_id);

				uint64_t new_distance =
					current_distance + edge.distance +
						graph.getHeuristic(edge.to) - graph.getHeuristic(edge.from);

				if (priorityQueue.GetDistanceToVertex(edge.to) > new_distance)
				{
					priorityQueue.SetDistance(edge.to, new_distance);
				}

			}
		}

		// Как только прорелаксировались из конечной вершины можно выходить, расстояние до нее найдено
		if (current_min_vertex == destination) {
			break;
		}
	}

	return distances[destination] == std::numeric_limits<uint64_t>::max()
		? distances[destination]
		: distances[destination] + graph.getHeuristic(source) - graph.getHeuristic(destination);
}

// Версия, замеряющая работу
template<uint64_t NumberOfChildren>
uint64_t ALT (const Graph& graph, uint64_t source, uint64_t destination, TotalDuration& d) {
	ADD_DURATION(d);


	std::deque<uint64_t> distances(graph.getVerticesCount());

	PriorityQueue<NumberOfChildren> priorityQueue(graph.getVerticesCount());
	priorityQueue.MakeVertexActive(source);
	priorityQueue.SetDistance(source, 0lu);

	while (!priorityQueue.Empty()) {
		// Извлекаем очередную вершину с минимальным расстоянием
		uint64_t current_min_vertex = priorityQueue.RemoveMin();
		uint64_t current_distance = priorityQueue.GetDistanceToVertex(current_min_vertex);

		// Записываем расстояние до нее
		distances[current_min_vertex] = current_distance;

		// Делаем релаксацию, если расстояние до вершины вообще было найдено,
		// то есть, что она лежит в данной компоненте связности
		if (current_distance != std::numeric_limits<int64_t>::max()) {
			for (uint64_t edge_id : graph.getOutgoingEdgesIds(current_min_vertex)) {
				const auto edge = graph.getEdgePropertiesById(edge_id);

				uint64_t new_distance =
					current_distance + edge.distance +
					graph.getHeuristic(edge.to) - graph.getHeuristic(edge.from);

				if (priorityQueue.GetDistanceToVertex(edge.to) > new_distance)
				{
					priorityQueue.SetDistance(edge.to, new_distance);
				}

			}
		}

		// Как только прорелаксировались из конечной вершины можно выходить, расстояние до нее найдено
		if (current_min_vertex == destination) {
			break;
		}
	}

	return distances[destination] + graph.getHeuristic(source) - graph.getHeuristic(destination);
}