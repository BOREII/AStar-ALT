#pragma once

#include "priority_queue.h"
#include "graph.h"

// Принимает на вход граф и начальную вершину
// Возвращает вектор расстояний до всех вершин

template<uint64_t NumberOfChildren>
std::deque<uint64_t> Dijkstra(const Graph& graph, uint64_t vertex_index) {
	std::deque<uint64_t> distances(graph.getVerticesCount());

	PriorityQueue<NumberOfChildren> priorityQueue(graph.getVerticesCount());
	priorityQueue.MakeVertexActive(vertex_index);
	priorityQueue.SetDistance(vertex_index, 0lu);

	while (!priorityQueue.Empty()) {
		// Извлекаем очередную вершину с минимальным расстоянием
		uint64_t current_min_vertex = priorityQueue.RemoveMin();
		uint64_t current_distance = priorityQueue.GetDistanceToVertex(current_min_vertex);

		// Записываем расстояние до нее в ответ
		distances[current_min_vertex] = current_distance;

		// Делаем релаксацию, если расстояние до вершины вообще было найдено,
		// то есть, что она лежит в данной компоненте связности
		if (current_distance != std::numeric_limits<int64_t>::max()) {
			for (uint64_t edge_id : graph.getOutgoingEdgesIds(current_min_vertex)) {
				const auto edge = graph.getEdgePropertiesById(edge_id);

				if (priorityQueue.GetDistanceToVertex(edge.to) > current_distance + edge.distance) {
					priorityQueue.SetDistance(edge.to, current_distance + edge.distance);
				}

			}
		}
	}

	return distances;
}
