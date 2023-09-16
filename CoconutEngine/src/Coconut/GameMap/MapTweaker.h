#pragma once
#include <vector>



namespace Coconut {
	class MapTweaker {
		// This is designed to be not a singleton, there can be many
		// map instance.
	public:
		// Fill the map with some predefined method
		enum class FillMapType {
			zeros = 0,
			random = 1
		};

		MapTweaker();
		MapTweaker(int row, int col);
		MapTweaker(int row, int col, FillMapType fillMapType);
		// fill the map with some predefined methods;


		void fillMap(std::vector< std::vector<int> >& map);


	private:
		//map info
		int m_row;
		int m_column;
		FillMapType s_fillMapType;

		void fitSize(std::vector< std::vector<int> >& map);

		void fillZeros(std::vector< std::vector<int> >& map);
	};
}