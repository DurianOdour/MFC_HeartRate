#pragma once
#include <vector>
#include <exception>
#include <algorithm>
#include "Eigen/Dense"

using namespace Eigen;
typedef std::vector<int> vectori;
typedef std::vector<double> vectord;

class CFilter
{
public:
	CFilter();
	~CFilter();
	void filtfilt(vectord B, vectord A, const vectord & X, vectord & Y);
private:
	void add_index_range(vectori & indices, int beg, int end, int inc);
	void add_index_const(vectori & indices, int value, size_t numel);
	void append_vector(vectord & vec, const vectord & tail);
	vectord subvector_reverse(const vectord & vec, int idx_end, int idx_start);
	void filter(vectord B, vectord A, const vectord & X, vectord & Y, vectord & Zi);
	bool compare(const vectord & original, const vectord & expected, double tolerance);
};

