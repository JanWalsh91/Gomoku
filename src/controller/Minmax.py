# 	A node is a game gomoku

import math
import sys
from inspect import signature

class Minmax:

	def __init__(self, heuristic=None, get_child_nodes=None, depth=10):
		print('init Minmax')
		if (heuristic == None or not callable(heuristic) or get_child_nodes == None or not callable(get_child_nodes)):
			raise Exception('heuristic or get_child_nodes param must be a callable')
		# sig = signature(heuristic)
		# print(sig.parameters)
		self.get_child_nodes = get_child_nodes
		self.heuristic = heuristic
		self.depth = depth


		self.prunned = 0
		self.not_prunned = 0



 	#pseudo code
	# def run(self, gomoku, depth):
	# 	gomoku.current_move
		
	# 	if depth == 0:
	# 		return gomoku.delta_heuristic(gomoku.current_pos)

	# 	dict = gomoku.get_pos() # {pos: [], delta_value:}
	# 	for pos in positions:
	# 		delta_values = gomoku.delta_heuristic(pos.pos) # values {player1_delta: , player2_delta: }
	# 		dict[pos].value - delta_values[current_player].score - delta_values[other_player].score

	# 	dict.sort()

	# 	for pos in positions:
	# 		set undo fn
	# 		gomoko.place()

	# 		value = run(gomoku, depth)

	# 		undo()

	# 		if prune:
	# 			break

	# def gomoku.delta_heuristic():
	# 	[] = eval()
	# 	return  []

	# def gomoku.place():


	# def gomoku.get_children():
	# 	for pos in positions:
	# 		self.place(pos)
	# 		self.update_board_value(pos)

	# def gomoku.update_board_value(pos):
	# 	res = self.calc_delta(pos)
	# 	self.score.player1 += res[0]
	# 	self.score.player2 += res[0]
	#end


	def run(self, gomoku, pos, depth, alpha, beta, maximizing_player):
		# print('depth:', depth)
		
		# place
		# eval delta => update gomoku score values

		if depth == 0 or (pos and self.is_end_gomoku(gomoku, pos)):
			# return 
			return [self.eval_node(gomoku, pos), pos]
		children = self.get_child_nodes()
		if maximizing_player:
			# print('MAXIMIZING for player', gomoku.current_player.index)
			max_eval = [-math.inf]
			for i, child in enumerate(children):
				# print('MAXIMIZING for child ', i)

				gomoku.update_next_turn(1)
				gomoku.place(None, child)

				gomoku.add_undo('set_intersection_validity_array', gomoku.intersection_validity_array)
				gomoku.next_turn(None)

				eval = self.run(gomoku, child, depth - 1, alpha, beta, False)
				
				gomoku.undo()
				gomoku.current_player = gomoku.players[0] if gomoku.current_player == gomoku.players[1] else gomoku.players[1]

				# Prunning
				max_eval = max_eval if max_eval[0] >= eval[0] else eval
				alpha = max(alpha, eval[0])
				if beta <= alpha:
					self.prunned += 1
					break
				self.not_prunned += 1

				# print('prunned: ', (self.prunned / self.not_prunned))
			return max_eval
		else:
			# print('MINIMIZING for player', gomoku.current_player.index)
			min_eval = [math.inf]
			for child in children:
				gomoku.update_next_turn(1)
				gomoku.place(None, child)
		
				gomoku.add_undo('set_intersection_validity_array', gomoku.intersection_validity_array)		
				gomoku.next_turn(None)
				
				eval = self.run(gomoku, child, depth - 1, alpha, beta, True)

				gomoku.undo()
				gomoku.current_player = gomoku.players[0] if gomoku.current_player == gomoku.players[1] else gomoku.players[1]

				# Prunning
				min_eval = min_eval if min_eval[0] <= eval[0] else eval
				beta = min(beta, eval[0])
				if beta <= alpha:
					break

			return min_eval



	def is_end_gomoku(self, gomoku, pos):
		gomoku.is_end_state(pos)

	def eval_node(self, gomoku, pos):
		return gomoku.heuristic()



	def get_best_child(self, node):
		return 
	# def max(self, node, depth):
	# 	#	return child node with highest cost
	# 	return move

	# def min(self, depth):
	# 	#	return child node with lowest cost
	# 	return move

	# def get_child_nodes(self, node):
	# 	#	determines child nodes
	# 	return child_nodes

	# def is_end_gomoku(self, node):
	# 	# 	determines if node configuration is a winning gomoku
	# 	return True

	# def eval_node(self, node):
	# 	#	calculates the weight of a node based on heuristic function
	# 	# 	heavily adjusted for win/loss gomoku
	# 	return 