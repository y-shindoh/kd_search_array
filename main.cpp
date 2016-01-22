/* -*- coding: utf-8; tab-width: 4 -*- */
/**
 * @file	main.cpp
 * @brief	kd_search_array.hppの動作確認用コマンド
 * @author	Yasutaka SHINDOH / 新堂 安孝
 */

#include <cstdio>
#include <array>
#include <vector>
#include "kd_search_array.hpp"

#define	M	6
#define	N	2

/**
 * サンプル・コマンド
 */
int
main()
{
	const std::array<int, 2> values[M] = {{{2, 1}},
										  {{2, 2}},
										  {{4, 2}},
										  {{6, 2}},
										  {{3, 3}},
										  {{5, 4}}};
	const std::array<int, 2> keys[N][2] = {{{{2, 0}},
											{{4, 4}}},
										   {{{4, 2}},
											{{10, 5}}}};
	std::vector<size_t> output;

	ys::KDSearchArray<int, 2> kdt;

	if (kdt.prepare(values, M)) {
		for (size_t i(0); i < (size_t)N; ++i) {
			kdt.find(values, keys[i][0], keys[i][1], output);
			for (auto j : output) std::printf("%lu\n", j);
			std::printf("\n");
			output.clear();
		}
	}

	return 0;
}
