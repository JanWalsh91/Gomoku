from controller.rules.ARule import ARule
import numpy as np
import collections	

class NoDoubleThree(ARule):
	def can_place(self, gomoku, pos):
		pos = np.array(pos)
		current_player = gomoku.current_player.index
		other_player = 1 if gomoku.current_player.index == 2 else 2
		nb_free_threes = 0

		for direction in [[0, 1], [1, 0], [1, 1], [-1, 1]]:
			if nb_free_threes >= 2:
				break 
			line = np.full(9, other_player)
			line_index = 4
			line[line_index] = current_player

			direction = np.array(direction)

			for i in range(1, 5):
				pos1 = pos + direction * i
				if pos1[0] > (len(gomoku.board) - 1) or pos1[1] > (len(gomoku.board) - 1) or pos1[0] < 0 or pos1[1] < 0:
					break
				line[line_index + i] = gomoku.board[pos1[0]][pos1[1]]

			for i in range(-1, -5, -1):
				pos1 = pos + direction * i
				if pos1[0] > (len(gomoku.board) - 1) or pos1[1] > (len(gomoku.board) - 1) or pos1[0] < 0 or pos1[1] < 0:
					break
				line[line_index + i] = gomoku.board[pos1[0]][pos1[1]]

			for i in range(0, 4):
				i += 1
				subline = line[i:i+4]
				counter = collections.Counter(subline)
				if counter[current_player] == 3 and counter[0] == 1:
					indices = np.where(subline == current_player)[0] + i
					if line[indices[0] - 1] == 0 and line[indices[-1] + 1] == 0:
						nb_free_threes += 1
						break
		return nb_free_threes < 2