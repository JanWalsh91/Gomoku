# 	A node is a game gomoku
import numpy as np
import math
import sys
from inspect import signature

class Minmax:

	def __init__(self, heuristic=None, get_moves=None, do_move=None, undo_move=None, timeout=None, ply_depth=1, max_depth=10):
		print('init Minmax')
		self.heuristic = heuristic
		self.get_moves = get_moves
		self.do_move = do_move
		self.undo_move = undo_move
		self.timeout = timeout
		self.ply_depth = ply_depth
		self.max_depth = max_depth

	def run(self):
		print('Run AI')
		# go.switch_player()

		def minmaxAlphaBeta(depth, move, alpha, beta, maximizing_player):

			if depth == 0: # if is winning condition
				return [self.heuristic(), move]

			moves = self.get_moves()

			# moves.sort()

			for move in moves:
				# returns undo moves
				self.do_move(move)

				res = minmaxAlphaBeta(depth - 1, move, alpha, beta, not maximizing_player)

				# takes undo moves
				self.undo_move(move)

				if maximizing_player:
					if res[0] >= beta:
						return [beta, move]
					if res[0] > alpha:
						alpha = res[0]
				else:
					if res[0] <= alpha:
						return [alpha, move]
					if res[0] < beta:
						beta = res[0]

			if maximizing_player:
				return [alpha, move]
			else:
				return [beta, move]

		return minmaxAlphaBeta(self.max_depth, None, -math.inf, math.inf, True)[1]
	