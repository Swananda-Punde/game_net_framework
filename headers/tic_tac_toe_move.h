#ifndef TIC_TAC_TOE_MOVE_H
#define TIC_TAC_TOE_MOVE_H

#include<boost/archive/text_iarchive.hpp>
#include<boost/archive/text_oarchive.hpp>

class tic_tac_toe_move {
	public :
		tic_tac_toe_move();
		tic_tac_toe_move(int , int);
		tic_tac_toe_move(tic_tac_toe_move &);
		int get_row();
		int get_col();
		void set_row(int row);
		void set_clo(int col);
	private :
		int row, col;
		template<typename Archive>
		void serialize(Archive &ar, const unsigned int version);
}

#endif
