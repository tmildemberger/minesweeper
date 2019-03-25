#include <iostream>

#include <random>
#include <vector>
#include <ctime>
#include <cmath>
#include <utility>
#include <algorithm>
struct Space {
public:
    int n_around { 0 };
    bool is_mine { false };
    bool is_flagged { false };
    bool got_dug { false };
};

class MineSweeper {
public:
    MineSweeper(int rows, int cols, double bombs_ratio) : rows_{rows}, cols_{cols}{
        for (int row { 0 }; row < rows; ++row) {
            for (int col { 0 }; col < cols; ++col) {
                grid.emplace_back();
            }
        }
        if (bombs_ratio > 0.85) bombs_ratio = 0.85;
        else if (bombs_ratio < 0.05) bombs_ratio = 0.05;
        needed_bombs =
            static_cast<int>(std::round(rows_ * cols_ * bombs_ratio));
        if (needed_bombs < 1) needed_bombs = 1;
        total_size = rows_ * cols_;
    }
    void gen_mines(int selected) {
        std::mt19937 mt_generator { static_cast<unsigned int>(std::time(nullptr)) };
        std::uniform_int_distribution<> grid_gen { 0, static_cast<int>(grid.size()) };
        // std::uniform_int_distribution<> col_gen { 0, cols_ };
        int bombs_made { 0 };
        while (bombs_made < needed_bombs) {
            int index { grid_gen(mt_generator) };
            if (index != selected && !grid[index].is_mine) {
                grid[index].is_mine = true;
                ++bombs_made;
            } else {
                // std::cout << needed_bombs - bombs_made << " bombs to go\n";
            }
        }
        // for (std::size_t i { 0 }; i < grid.size(); ++i) {
        //     if (grid[i].is_mine) {
        //         std::cout << i / cols_ << ' ' << i % cols_ << '\n';
        //     }
        // }
	for (int row { 0 }; row < rows_; ++row) {
	    for (int col { 0 }; col < cols_; ++col) {
		std::pair<int, int> places_to_look[]
		{ {-1, -1}, {-1, 0}, {-1, 1},
		  { 0, -1},          { 0, 1},
		  { 1, -1}, { 1, 0}, { 1, 1} };
                for (const auto& place : places_to_look) {
	            int place_r { row + place.first };
		    int place_c { col + place.second };
		    if (place_r >= 0 && place_r < rows_ &&
		        place_c >= 0 && place_c < cols_) {
		        if (grid[place_r * cols_ + place_c].is_mine) {
			    ++grid[row * cols_ + col].n_around;
			}
		    }
		}
	    }
	}
//        for (std::size_t idx { 0 }; idx < grid.size(); ++idx) {
//            int places_to_look[] 
//                                { -cols_ - 1, -cols_, -cols_ + 1,   // top
//                                  -1, 1,                            // middle
//                                  cols_ - 1, cols_, cols_ + 1 };    // bottom
//            for (auto place : places_to_look) {
//                if (idx + place >= 0 && idx + place < grid.size()) {
//                    if (grid[idx + place].is_mine) ++grid[idx].n_around;
//                }
//            }
//        }
    }
    void step(int row, int col) {
        int index { row * cols_ + col };
        if (!mines_set) {
            gen_mines(index);
            mines_set = true;
        }
        if (grid[index].is_flagged) {
            std::cout << "space is flagged\n";
        } else if (grid[index].is_mine) {
            std::cout << "you blew\n";
            lost = true;
        } else if (!(grid[index].got_dug)) {
            grid[index].got_dug = true;
            if (total_size == ++needed_bombs) {
                won = true;
            } else {
                std::cout << "there was nothing\n";
                std::cout << "there are " << grid[index].n_around << " near\n";
            }
        }
    }
    void show() {
        for (int row { 0 }; row < rows_; ++row) {
            for (int col { 0 }; col < cols_; ++col) {
                const auto& sp { grid[row * cols_ + col] };
                if (sp.is_flagged) {
                    std::cout << 'f';
                } else if (sp.got_dug) {
                    std::cout << sp.n_around;
                } else {
                    std::cout << 'o';
                }
            }
            std::cout << '\n';
        }
    }
    // void flag(int row, int col);
    // void print() {
    //     for ()
    // }
    bool lost { false };
    bool won { false };
private:
    std::vector<Space> grid;
    int rows_;
    int cols_;
    int total_size;
    int needed_bombs;
    bool mines_set { false };
};

int main() {
    MineSweeper mines { 3, 3, 0.1 };
    int row { 0 };
    int col { 0 };
    // std::cin >> row >> col;
    // mines.flag(row, col);
    // mines.step(row, col);
    // mines.show();
    // std::cin >> row >> col;
    // mines.flag(row, col);
    while (!mines.won && !mines.lost) {
        mines.show();
        std::cin >> row >> col;
        mines.step(row, col);
    }
    if (mines.won) {
        std::cout << "you won!\n";
    } else {
        std::cout << "you lost! good luck next time\n";
    }
    std::cout << "mineswepper\n";
    return 0;
}
