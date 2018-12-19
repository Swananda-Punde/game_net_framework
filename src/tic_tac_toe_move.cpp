#include<boost/archive/text_iarchive.hpp>
#include<boost/archive/text_oarchive.hpp>
#include"../headers/tic_tac_toe.h"

tic_tac_toe_move::tic_tac_toe_move(){
	row = col = -1;
}

tic_tac_toe_move::tic_tac_toe_move(int row, int col):row(row), col(col){
}

tic_tac_toe_move::tic_tac_toe_move(tic_tac_toe_move &obj){
	row = obj.row;
	col = obj.col;
}

int tic_tac_toe_move::get_row(){
	return row;
}

int tic_tac_toe_move::get_col(){
	return col;
}

void tic_tac_toe_move::set_row(int row){
	this->row = row;
}

void tic_tac_toe_move::set_col(int col){
	this->col = col;
}

template<typename Archive>
void tic_tac_toe_move::serialize(Archive &ar, const unsigned int version){
	ar & row;
	ar & col;
}
