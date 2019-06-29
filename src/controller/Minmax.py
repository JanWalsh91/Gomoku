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

	def run(self, go):
		print('Run AI')

		values = np.full(shape=(5, 5), fill_value=-1, dtype=int)

		def minmaxAlphaBeta(depth, move, alpha, beta, maximizing_player):

			if depth == 0: # if is winning condition
				return [self.heuristic(), move]

			moves = self.get_moves()

			# moves.sort()

			for move in moves:
				# returns undo moves
				self.do_move(move)
				if depth == self.max_depth:
					# print('move:', move)
					# go.print_my_board(go.board)
					# print('')
				res = minmaxAlphaBeta(depth - 1, move, alpha, beta, not maximizing_player)

				if depth == self.max_depth:
					values[move[0]][move[1]] = res[0]

				# takes undo moves
				self.undo_move(move)

				if maximizing_player:
					if res[0] >= beta[0]:
						return beta
					if res[0] > alpha[0]:
						alpha = res
				else:
					if res[0] <= alpha[0]:
						return alpha
					if res[0] < beta[0]:
						beta = res

			if maximizing_player:
				return alpha
			else:
				return beta

		ret = minmaxAlphaBeta(self.max_depth, None, [-math.inf, None], [math.inf, None], True)[1]

		print('=== VALUES ===')		
		s = [[str(e) for e in row] for row in values]
		lens = [max(map(len, col)) for col in zip(*s)]
		fmt = '\t'.join('{{:{}}}'.format(x) for x in lens)
		table = [fmt.format(*row) for row in s]
		print ('\n'.join(table))
		print ('\n')

		return ret
	