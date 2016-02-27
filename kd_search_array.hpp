/* -*- coding: utf-8; tab-width: 4 -*- */
/**
 * @file	kd_search_array.hpp
 * @brief	要素の追加・削除をしない配列版kD木
 * @author	Yasutaka SHINDOH / 新堂 安孝
 */

#ifndef	__KD_SEARCH_ARRAY_HPP__
#define	__KD_SEARCH_ARRAY_HPP__	"kd_search_array.hpp"

#include <cassert>
#include <array>
#include <vector>
#include <algorithm>

namespace ys
{
	/**
	 * 要素の追加・削除をしない配列版kD木
	 */
	template<typename TYPE, size_t N>
	class KDSearchArray
	{
	private:

		size_t* tree_;	///< kD木の本体
		size_t length_;	///< 配列 @a tree_ の容量

		/**
		 * kD木の構築
		 * @param[in,out]	buffer	配列 @a values のインデックス (作業領域)
		 * @param[in]	values	データ
		 * @param[in]	index	kD木の中で確定させるインデックス
		 * @param[in]	from	配列 @a buffer の処理領域の始点
		 * @param[in]	to	配列 @a buffer の処理領域の終点
		 * @param[in]	depth	kD木の深さ
		 */
		void
		build(size_t* buffer,
			  const std::array<TYPE, N>* values,
			  size_t index,
			  size_t from,
			  size_t to,
			  size_t depth)
			{
				assert(tree_);
				assert(buffer);
				assert(values);
				assert(from <= to);

				size_t k = (from + to) / 2;
				if (from < to) {
					size_t d = depth % N;
					std::stable_sort(buffer + from, buffer + to + 1,
									 [values, d] (size_t l, size_t r) { return values[l][d] <= values[r][d]; });
				}
				tree_[index] = buffer[k];

				if (from < k) build(buffer, values, index * 2 + 1, from, k - 1, depth + 1);
				if (k < to) build(buffer, values, index * 2 + 2, k + 1, to, depth + 1);
			}

	public:

		/**
		 * コンストラクタ
		 */
		KDSearchArray()
			: tree_(0), length_(0)
			{
				;
			}

		/**
		 * コピー・コンストラクタ (使用禁止)
		 */
		KDSearchArray(const KDSearchArray<TYPE, N>&) = delete;

		/**
		 * 代入演算子 (使用禁止)
		 */
		KDSearchArray&
		operator =(const KDSearchArray<TYPE, N>&) = delete;

		/**
		 * デストラクタ
		 */
		virtual
		~KDSearchArray()
			{
				if (tree_) delete [] tree_;
			}

		/**
		 * kD木の準備
		 * @param[in]	values	データ
		 * @param[in]	length	配列 @a values の要素数
		 */
		bool
		prepare(const std::array<TYPE, N>* values,
				size_t length)
			{
				assert(values);
				assert(0 < length);
				assert(length < ~(size_t)0);

				size_t* buffer(0);
				size_t l(1);
				while (l < length) l *= 2;

				if (tree_) {
					delete [] tree_;
					tree_ = 0;
				}

				try {
					tree_ = new size_t[l];
					buffer = new size_t[length];
				}
				catch (...) {
					;
				}

				if (!tree_ || !buffer) {
					if (tree_) delete [] tree_;
					if (buffer) delete [] buffer;
					return false;
				}

				std::fill(tree_, tree_ + l, ~(size_t)0);
				for (size_t i(0); i < length; ++i) buffer[i] = i;
				build(buffer, values, 0, 0, length - 1, 0);
				length_ = l;
				delete [] buffer;

				return true;
			}

		/**
		 * kD木の探索
		 * @param[in]	values	データ
		 * @param[in]	from	探索範囲の始点
		 * @param[in]	to	探索範囲の終点
		 * @param[out]	points	探索範囲内にある @a values 内の点のインデックス
		 * @param[in]	index	kD木内での探索対象のインデックス
		 * @param[in]	depth	kD木内での探索対象の深さ
		 * @note	通常利用では、引数 @a index, @a depth はデフォルト値で良い。
		 */
		void
		find(const std::array<TYPE, N>* values,
			 const std::array<TYPE, N>& from,
			 const std::array<TYPE, N>& to,
			 std::vector<size_t>& points,
			 size_t index = 0,
			 size_t depth = 0)
			{
				assert(tree_);
				assert(0 < length_);
				assert(values);
				assert(tree_[index] < ~(size_t)0);

				size_t x = tree_[index];
				bool f(true);

				for (size_t i(0); i < N; ++i) {
					if (from[i] <= values[x][i] && values[x][i] <= to[i]) continue;
					f = false;
					break;
				}

				if (f) points.push_back(x);

				size_t k = index * 2 + 1;
				size_t d = depth % N;
				if (k < length_ && tree_[k] < ~(size_t)0 && from[d] <= values[x][d]) {
					find(values, from, to, points, k, depth + 1);
				}

				++k;
				if (k < length_ && tree_[k] < ~(size_t)0 && values[x][d] <= to[d]) {
					find(values, from, to, points, k, depth + 1);
				}
			}
	};
};

#endif	// __KD_SEARCH_ARRAY_HPP__
