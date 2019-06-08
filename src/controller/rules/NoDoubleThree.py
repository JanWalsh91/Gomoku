from controller.rules.ARule import ARule
import numpy as np
import collections	

class NoDoubleThree(ARule):
	def can_place(self, board, player, pos):
		pos = np.array(pos)
		current_player = player.index
		other_player = 1 if player.index == 2 else 2
		nb_free_threes = 0

		for direction in [[0, 1], [1, 0], [1, 1], [-1, 1]]:
			line = np.zeros(9)
			line_index = 4
			line[line_index] = current_player

			direction = np.array(direction)

			for i in range(1, 5):
				pos1 = pos + direction * i
				if pos1[0] > (len(board) - 1) or pos1[1] > (len(board) - 1) or pos1[0] < 0 or pos1[1] < 0:
					break
				# print('update line BEFORE', line)
				# print('line_index + i', line_index + i)
				# print('board[pos1[0]][pos1[1]]', board[pos1[0]][pos1[1]])
				line[line_index + i] = board[pos1[0]][pos1[1]]
				# print('update line AFTER ', line)

			for i in range(-1, -5, -1):
				pos1 = pos + direction * i
				if pos1[0] > (len(board) - 1) or pos1[1] > (len(board) - 1) or pos1[0] < 0 or pos1[1] < 0:
					break
				line[line_index + i] = board[pos1[0]][pos1[1]]

			print('update line AFTER ', line)

			# if board[pos1[0]][pos1[1]] == current_player:
			# 	nb_player_stones += 1
			# pos2 = np.array(pos1) + np.array(direction)			
			# if pos2[0] > (len(board) - 1) or pos2[1] > (len(board) - 1) or pos2[0] < 0 or pos2[1] < 0 or board[pos2[0]][pos2[1]] == other_player:
			# 	continue
			# if board[pos2[0]][pos2[1]] == current_player:
			# 	nb_player_stones += 1
			# pos3 = np.array(pos2) + np.array(direction)			
			# if pos3[0] > (len(board) - 1) or pos3[1] > (len(board) - 1) or pos3[0] < 0 or pos3[1] < 0 or board[pos3[0]][pos3[1]] == other_player:
			# 	continue
			# if board[pos3[0]][pos3[1]] == current_player:
			# 	nb_player_stones += 1

			# if nb_player_stones == 2:

		return True