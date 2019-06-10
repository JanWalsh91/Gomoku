from controller.rules.ARule import ARule
import numpy as np

class Capture(ARule):

	def trigger_effect(self, gomoku, interface, pos):
		pos = np.array(pos)
		other_player = gomoku.players[0] if gomoku.current_player.index == 2 else gomoku.players[1]

		for direction in [[0, 1], [0, -1], [1, 0], [-1, 0], [1, 1], [1, -1], [-1, 1], [-1, -1]]:
			direction = np.array(direction)
			pos1 = pos + direction
			if pos1[0] > (len(gomoku.board) - 1) or pos1[1] > (len(gomoku.board) - 1) or pos1[0] < 0 or pos1[1] < 0 or gomoku.board[pos1[0]][pos1[1]] != other_player.index:
				continue
			pos2 = pos1 + direction
			if pos2[0] > (len(gomoku.board) - 1) or pos2[1] > (len(gomoku.board) - 1) or pos2[0] < 0 or pos2[1] < 0 or gomoku.board[pos2[0]][pos2[1]] != other_player.index:
				continue
			pos3 = pos2 + direction
			if pos3[0] > (len(gomoku.board) - 1) or pos3[1] > (len(gomoku.board) - 1) or pos3[0] < 0 or pos3[1] < 0 or gomoku.board[pos3[0]][pos3[1]] != gomoku.current_player.index:
				continue
			# conditions fullfilled
			gomoku.board[pos1[0]][pos1[1]] = 0
			gomoku.board[pos2[0]][pos2[1]] = 0
			interface.remove_stone_from(pos2)
			interface.remove_stone_from(pos1)

			gomoku.current_player.captures += 1
			interface.current_player.captures += 1

			print('Captured!')

		return