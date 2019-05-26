#pragma once

#include <string>
#include <fstream>

#include "graph.h"

void SkipLines(std::ifstream& input, size_t n_strings = 1) {
	for (size_t i = 0; i < n_strings; ++i) {
		std::string tmp_str;
		std::getline(input, tmp_str);
	}
	input.ignore(1);
}

Graph::EdgePtr ParseEdge(std::istream& input) {
	input.ignore(2);
	auto edge_ptr = std::make_shared<Edge>();
	input >> edge_ptr->from >> edge_ptr->to >> edge_ptr->distance;
	// Делаем индексацию с нуля
	--edge_ptr->from;
	--edge_ptr->to;
	return edge_ptr;
}

Graph BuildMap(const std::string& file_name) {
	std::ifstream input(file_name);

	SkipLines(input, 4);
	input.ignore(4);

	uint64_t n_vertices, n_edges;
	input >> n_vertices >> n_edges;

	SkipLines(input, 2);

	Graph graph(n_vertices);
	for (uint64_t i = 0; i < n_edges; ++i) {
		graph.addEdge(ParseEdge(input));
	}

	return graph;
}