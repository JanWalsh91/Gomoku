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


	def run(self, gomoku, pos, depth, alpha, beta, maximizing_player):
		# print('run', depth)

		# cal delta, update gomoku


		if depth == 0 or (pos and self.is_end_gomoku(gomoku, pos)):
			return [self.eval_node(gomoku, pos), pos]
		children = self.get_child_nodes()
		if maximizing_player:
			print('MAXIMIZING for ', len(children), 'children')
			max_eval = [-math.inf]
			for i, child in enumerate(children):
				print('MAXIMIZING for child ', i)
				# place a stone at this child pos
				# gomoku.print_board()
				gomoku.update_next_turn(1)
				gomoku.place(None, child)
				# gomoku.current_player = gomoku.players[0] if gomoku.current_player == gomoku.players[1] else gomoku.players[1]
				gomoku.add_undo('set_intersection_validity_array', gomoku.intersection_validity_array)
				gomoku.next_turn(None)

				gomoku.print_my_board(gomoku.intersection_validity_array)
				eval = self.run(gomoku, child, depth - 1, alpha, beta, False)
				
				gomoku.undo()
				gomoku.current_player = gomoku.players[0] if gomoku.current_player == gomoku.players[1] else gomoku.players[1]

				max_eval = max_eval if max_eval[0] > eval[0] else eval
				alpha = max(alpha, eval[0])
				# print('MAX alpha: {}, beta: {}'.format(alpha, beta))
				if beta <= alpha:
					break
			# sys.exit()
			return max_eval
		else:
			print('MINIMIZING for ', len(children), 'children')
			min_eval = [math.inf]
			for child in children:
				# place a stone at this child pos
				# gomoku.print_board()
				gomoku.update_next_turn(1)
				gomoku.place(None, child)
				gomoku.current_player = gomoku.players[0] if gomoku.current_player == gomoku.players[1] else gomoku.players[1]
		
				gomoku.add_undo('set_intersection_validity_array', gomoku.intersection_validity_array)		
				gomoku.next_turn(None)
				
				gomoku.print_my_board(gomoku.intersection_validity_array)
				eval = self.run(gomoku, child, depth - 1, alpha, beta, True)
				gomoku.undo()
				gomoku.current_player = gomoku.players[0] if gomoku.current_player == gomoku.players[1] else gomoku.players[1]
				# gomoku.next_turn(None)
				# gomoku.print_board()
				# undo
				min_eval = min_eval if min_eval[0] < eval[0] else eval
				beta = min(beta, eval[0])
				# print('MIN alpha: {}, beta: {}'.format(alpha, beta))
				if beta <= alpha:
					break
			# sys.exit()
			return min_eval



	def is_end_gomoku(self, gomoku, pos):
		gomoku.is_end_gomoku(pos)

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