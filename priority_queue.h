#pragma once

#include <limits>
#include <cstdint>
#include <deque>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <iostream>

template<uint8_t NumberOfChildren>
class PriorityQueue {
public:
	// Инициализирует структуру для графа на n_vertex вершинах
	explicit PriorityQueue(uint64_t n_vertex);

	// Возвращает текущее найденное расстояние до вершины. Если оно еще не найдено - возвращается
	// std::numeric_limits<uint64_t>::max();
	uint64_t GetDistanceToVertex(uint64_t vertex_index) const;

	// Сообщает о том, что некоторый путь до вершины уже найден, но он еще не оптимален
	// Это означает, что vertex_index на данный момент распологается в очереди (на краю волнового фронта)
	bool IsActive(uint64_t vertex_index) const;

	// Делает вершину активной и возвращает true, если вершина уже была активной возвращается false
	bool MakeVertexActive(uint64_t vertex_index);

	// Делает вершину не активной и возвращает true если вершина была активной, иначе возвращается false
	bool MakeVertexInactive(uint64_t vertex_index);

	// Изменяет текущее расстояние до вершины, вершина может быть как активной так и не активной
	void SetDistance(uint64_t vertex_index, uint64_t distance);

	// Возвращает номер активной вершины, расстояние до которой минимально
	uint64_t GetMinIndex() const;

	// Возвращает номер активной вершины, расстояние до которой минимально и делает ее неактивной
	uint64_t RemoveMin();

	// true если не осталось активных вершин
	bool Empty() const;

private:
	static const uint64_t INF = std::numeric_limits<uint64_t>::max();

	struct Vertex {
		uint64_t index;
		uint64_t distance = INF;
		bool is_active = false;
	};

	void ShiftUp(uint64_t vertex_index);
	void ShiftDown(uint64_t vertex_index);

	std::deque<Vertex> heap;
	uint64_t heap_size;
	std::deque<uint64_t> positions;
};

template<uint8_t NumberOfChildren>
PriorityQueue<NumberOfChildren>::PriorityQueue(uint64_t n_vertex)
	: heap(n_vertex), heap_size(n_vertex), positions(n_vertex)
{
	for (uint64_t idx = 0; idx < n_vertex; ++idx) {
		heap[idx].index = idx;
		positions[idx] = idx;
	}
}

template<uint8_t NumberOfChildren>
uint64_t PriorityQueue<NumberOfChildren>::GetDistanceToVertex(uint64_t vertex_index) const {
	return heap.at(positions.at(vertex_index)).distance;
}

template<uint8_t NumberOfChildren>
bool PriorityQueue<NumberOfChildren>::IsActive(uint64_t vertex_index) const {
	return heap.at(positions.at(vertex_index)).is_active;
}

template<uint8_t NumberOfChildren>
bool PriorityQueue<NumberOfChildren>::MakeVertexActive(uint64_t vertex_index) {
	return IsActive(vertex_index) ? false : heap.at(positions.at(vertex_index)).is_active = true;
}

template<uint8_t NumberOfChildren>
bool PriorityQueue<NumberOfChildren>::MakeVertexInactive(uint64_t vertex_index) {
	return IsActive(vertex_index) ? !(heap.at(positions.at(vertex_index)).is_active = false) : false;
}

template<uint8_t NumberOfChildren>
void PriorityQueue<NumberOfChildren>::SetDistance(uint64_t vertex_index, uint64_t distance) {
	heap.at(positions.at(vertex_index)).distance = distance;
	ShiftUp(vertex_index);
}

template<uint8_t NumberOfChildren>
uint64_t PriorityQueue<NumberOfChildren>::GetMinIndex() const {
	return heap.at(0).index;
}

template<uint8_t NumberOfChildren>
uint64_t PriorityQueue<NumberOfChildren>::RemoveMin() {
	uint64_t idx_of_min = heap.at(0).index;
	MakeVertexInactive(idx_of_min);

	// Меняем корень с последним элементом в куче
	std::swap(positions.at(idx_of_min), positions.at(heap.at(heap_size - 1).index));
	std::swap(heap.at(0), heap.at(heap_size - 1));
	--heap_size;
	ShiftDown(heap.at(0).index);
	return idx_of_min;
}

template<uint8_t NumberOfChildren>
bool PriorityQueue<NumberOfChildren>::Empty() const {
	return heap_size == 0;
}

template<uint8_t NumberOfChildren>
void PriorityQueue<NumberOfChildren>::ShiftUp(uint64_t vertex_index) {
	while (positions.at(vertex_index) != 0 &&
		heap.at(positions.at(vertex_index)).distance <
			heap.at((positions.at(vertex_index) - 1) / NumberOfChildren).distance) {

		uint64_t parent_idx = heap.at((positions.at(vertex_index) - 1) / NumberOfChildren).index;

		std::swap(heap.at(positions.at(vertex_index)),
		          heap.at(positions.at(parent_idx)));

		std::swap(positions.at(vertex_index),
		          positions.at(parent_idx));
	}
}

template<uint8_t NumberOfChildren>
void PriorityQueue<NumberOfChildren>::ShiftDown(uint64_t vertex_index) {
	while (positions.at(vertex_index) * NumberOfChildren + 1 < heap_size) {

		// Проверяем, нужно ли спускаться дальше,
		// и если нужно, выбираем минимального ребенка

		uint64_t idx_of_lowest = vertex_index;

		for (uint64_t parent_position = positions.at(vertex_index) * NumberOfChildren + 1;
		     parent_position <= (positions.at(vertex_index) + 1) * NumberOfChildren &&
			     parent_position < heap_size; ++parent_position) {
			if (heap.at(parent_position).distance <
				heap.at(positions.at(idx_of_lowest)).distance) {
				idx_of_lowest = heap.at(parent_position).index;
			}
		}

		if (idx_of_lowest == vertex_index) {
			break;
		}

		// Меняем позиции в куче
		std::swap(heap.at(positions.at(vertex_index)), heap.at(positions.at(idx_of_lowest)));
		std::swap(positions.at(vertex_index), positions.at(idx_of_lowest));
	}
}
