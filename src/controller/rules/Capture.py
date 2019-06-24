from controller.rules.ARule import ARule
import numpy as np
import copy

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
			gomoku.remove(interface, pos1)
			gomoku.remove(interface, pos2)
			# print('adding simply place undo', pos1)
			gomoku.add_undo('simple_place', pos1)
			# print('adding simply place undo', pos2)
			gomoku.add_undo('simple_place', pos2)

			gomoku.add_undo('set_captures', {'index': gomoku.current_player.index, 'captures': gomoku.current_player.captures})
			gomoku.current_player.captures += 1
			if interface:
				interface.current_player.captures += 1

			# print('Captured!')

		return

	def is_winning_condition(self, gomoku):
		# if gomoku.current_player.captures >= 5:
		# 	print('Winning by capture')
		return gomoku.current_player.captures >= 5