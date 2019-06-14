# 	A node is a game state

import math
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


	def run(self, state, pos, depth, alpha, beta, maximizing_player):
		# print('run', depth)
		if depth == 0 or (pos and self.is_end_state(state, pos)):
			return [self.eval_node(state, pos), pos]
		children = self.get_child_nodes()
		if maximizing_player:
			# print('MAXIMIZING')
			max_eval = [-math.inf]
			for child in children:
				# place a stone at this child pos
				new_state = state.copy()
				new_state.place(None, child)
				eval = self.run(new_state, child, depth - 1, alpha, beta, False)
				max_eval = max_eval if max_eval[0] > eval[0] else eval
				alpha = max(alpha, eval[0])
				# print('MAX alpha: {}, beta: {}'.format(alpha, beta))
				if beta <= alpha:
					break
			return max_eval
		else:
			# print('MINIMIZING')
			min_eval = [math.inf]
			for child in children:
				# place a stone at this child pos
				new_state = state.copy()
				new_state.place(None, child)
				eval = self.run(new_state, child, depth - 1, alpha, beta, True)
				min_eval = min_eval if min_eval[0] < eval[0] else eval
				beta = min(beta, eval[0])
				# print('MIN alpha: {}, beta: {}'.format(alpha, beta))
				if beta <= alpha:
					break
			return min_eval



	def is_end_state(self, state, pos):
		state.is_end_state(pos)

	def eval_node(self, state, pos):
		# print('eval_node pos', pos)
		return state.heuristic()



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

	# def is_end_state(self, node):
	# 	# 	determines if node configuration is a winning state
	# 	return True

	# def eval_node(self, node):
	# 	#	calculates the weight of a node based on heuristic function
	# 	# 	heavily adjusted for win/loss state
	# 	return 