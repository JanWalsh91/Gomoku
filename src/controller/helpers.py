import numpy as np


def can_place_pos(gomoku, pos):
	for rule in gomoku.rules:
		if not rule.can_place(gomoku, pos):
			return False
	return True

def intersection_validity_pos(gomoku, pos):
	if gomoku.board[pos[0]][pos[1]] >= 1:
		return 0
	if can_place_pos(gomoku, pos):
		return 1
	else:
		return 2