import numpy as np

# TODO issue in that function with ai
def five_aligned(gomoku, pos):
	board = gomoku.board
	coord_list = []
	for direction in [[0, 1], [1, 0], [1, 1], [-1, 1]]:
		sub_coord_list = [None, None, None, None, pos, None, None, None, None]
		num_aligned = 1
		for y, direction in enumerate([direction, [-direction[0], -direction[1]]]):
			next_pos = pos
			for i in range(0, 4):
				next_pos = next_pos.copy()
				next_pos[0] += direction[0]
				next_pos[1] += direction[1]
				if next_pos[0] > (len(gomoku.board) - 1) or next_pos[1] > (len(gomoku.board) - 1) or next_pos[0] < 0 or next_pos[1] < 0 or gomoku.board[next_pos[0]][next_pos[1]] != gomoku.current_player.index:
					break
				sub_coord_list[4 + (-1 if y == 1 else 1) * (i + 1)] = next_pos
				num_aligned += 1
				if num_aligned >= 5:
					coord_list.append(sub_coord_list)
	return coord_list if len(coord_list) > 0 else False

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