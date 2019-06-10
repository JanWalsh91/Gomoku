import numpy as np

from controller.rules.ARule import ARule
from controller.helpers import five_aligned

class CaptureGameEnding(ARule):
	def can_place(self, gomoku, pos):
		other_player = gomoku.players[0] if gomoku.current_player.index == 2 else gomoku.players[1]
		if other_player.captures < 4:
			return True
		coord_lists = five_aligned(gomoku, pos)
		if coord_lists:
			for coord_list in coord_lists:
				for coord in coord_list:
					if coord and self.can_capture(gomoku, coord):
						return False

		return True
	
	def can_capture(self, gomoku, coord):
		current_player = gomoku.current_player.index
		other_player = 1 if gomoku.current_player.index == 2 else 2

		for direction in np.array([[0, 1], [1, 0], [1, 1], [-1, 1]]):
			line = np.full(5, 0)
			line_index = 2
			line[line_index] = current_player

			for i in range(1, 3):
				next_pos = coord + direction * i
				if next_pos[0] > (len(gomoku.board) - 1) or next_pos[1] > (len(gomoku.board) - 1) or next_pos[0] < 0 or next_pos[1] < 0:
					break
				line[line_index + i] = gomoku.board[next_pos[0]][next_pos[1]]

			for i in range(-1, -3, -1):
				next_pos = coord + direction * i
				if next_pos[0] > (len(gomoku.board) - 1) or next_pos[1] > (len(gomoku.board) - 1) or next_pos[0] < 0 or next_pos[1] < 0:
					break
				line[line_index + i] = gomoku.board[next_pos[0]][next_pos[1]]

			if (line[1] == current_player):
				if (line[0] == other_player and line[3] == 0) or (line[0] == 0 and line[3] == other_player):
					return True
			if (line[3] == current_player):
				if (line[1] == 0 and line[4] == other_player) or (line[1] == other_player and line[4] == 0):
					return True

		return False