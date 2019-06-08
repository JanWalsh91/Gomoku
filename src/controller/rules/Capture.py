from controller.rules.ARule import ARule
import numpy as np

class Capture(ARule):

	def trigger_effect(self, board, player, pos, interface):
		pos = np.array(pos)
		current_player = player.index
		other_player = 1 if player.index == 2 else 2

		for direction in [[0, 1], [0, -1], [1, 0], [-1, 0], [1, 1], [1, -1], [-1, 1], [-1, -1]]:
			direction = np.array(direction)
			pos1 = pos + direction
			if pos1[0] > (len(board) - 1) or pos1[1] > (len(board) - 1) or pos1[0] < 0 or pos1[1] < 0 or board[pos1[0]][pos1[1]] != other_player:
				continue
			pos2 = pos1 + direction
			if pos2[0] > (len(board) - 1) or pos2[1] > (len(board) - 1) or pos2[0] < 0 or pos2[1] < 0 or board[pos2[0]][pos2[1]] != other_player:
				continue
			pos3 = pos2 + direction
			if pos3[0] > (len(board) - 1) or pos3[1] > (len(board) - 1) or pos3[0] < 0 or pos3[1] < 0 or board[pos3[0]][pos3[1]] != current_player:
				continue
			# conditions fullfilled
			board[pos1[0]][pos1[1]] = 0
			board[pos2[0]][pos2[1]] = 0
			interface.remove_stone_from(pos1)
			interface.remove_stone_from(pos2)
			print('Captured!')
		return