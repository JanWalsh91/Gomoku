# 	A node is a game gomoku
import numpy as np
import math
import sys
from inspect import signature
import time

class Minmax:

	def __init__(self, heuristic=None, get_moves=None, do_move=None, undo_move=None, hash_state=None, timeout=None, ply_depth=1, max_depth=10):
		print('init Minmax')
		self.heuristic = heuristic
		self.get_moves = get_moves
		self.do_move = do_move
		self.undo_move = undo_move
		self.hash_state = hash_state
		self.timeout = timeout
		self.ply_depth = ply_depth
		self.max_depth = max_depth
		self.transposition_table = {}
		self.compteur = 0
		self.contre_compteur = 0
		self.start_time = None
		self.out_of_time = False

	def run(self, go):
		print('Run AI')

		self.map = {}
		self.best_move = None
		self.best_value = -math.inf
		self.compteur = 0
		self.contre_compteur = 0
		self.out_of_time = False
		if self.timeout:
			self.start_time = time.clock()

		# return VALUE of best node
		def minmaxAlphaBeta(depth, alpha, beta, player):
			if self.timeout and time.clock() - self.start_time > self.timeout:
				self.out_of_time = True

			if not self.map.get(depth):
				print(go.current_player.index,  ' is at depth ', depth)
				self.map[depth] = True

			if depth == 0: # if is winning condition
				# print('Heuristic for depth ', depth, 'move', move)
				if self.hash_state:
					key = self.hash_state()
					val = self.transposition_table.get(key)
					if not val:
						val = self.heuristic()
						self.transposition_table[key] = val
						self.contre_compteur += 1
					else:
						self.compteur += 1
				else:
					val = self.heuristic()
				return player * val


			moves = self.get_moves()

			value = -math.inf
			for move in moves:
				if self.out_of_time:
					print('I BREAK BECAUSE NO TIME')
					break
				self.do_move(move)
				ret = -minmaxAlphaBeta(depth - 1, -beta, -alpha, -player)
				if depth == self.max_depth:
					print('self.values at {} set {}'.format(move, -ret))
					self.values[move[0]][move[1]] = -ret
					if player * ret > self.best_value:
						self.best_move = move
						self.best_value = player * ret
				value = max(value, ret)
				self.undo_move(move)
				alpha = max(alpha, value)
				if alpha >= beta:
					break
			return value


		self.values = np.full(shape=(go.size, go.size), fill_value=-1, dtype=int)
		# moves = self.get_moves()

		# best_move = None
		# best_value = -math.inf

		

		minmaxAlphaBeta(self.max_depth, -math.inf, math.inf, 1)


		print('=== VALUES ===')
		s = [[str(e) for e in row] for row in self.values]
		lens = [max(map(len, col)) for col in zip(*s)]
		fmt = '\t'.join('{{:{}}}'.format(x) for x in lens)
		table = [fmt.format(*row) for row in s]
		print ('\n'.join(table))
		print ('\n')

		print('La valeur du compteur est : ', self.compteur)
		print('La valeur du contre_compteur est : ', self.contre_compteur)

		return self.best_move
	
