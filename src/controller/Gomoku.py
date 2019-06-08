import numpy as np
from enum import Enum, auto

win_length = 3

class Gomoku:

	class Rules(Enum):
		CAPTURE = auto(),
		GAME_ENDING_CAPTURE = auto(),
		NO_DOUBLE_THREE = auto()

	def __init__(self, rules, size=19):
		if (size < 1):
			raise Exception('Board size must be at least 5')
		self.size = size
		self.values = np.zeros(shape=(size,size), dtype=int)
		# players
		self.rules = rules
		


	def place(self, pos, player): #player 1 or 2, pos: [row, col]
		if (0 > pos[0] or pos[0] > self.size or 0 > pos[1] or pos[1] > self.size):
			print('bad place')
			return False
		if (self.values[pos[0]][pos[1]] != 0):
			print('place already has value', self.values[pos[0]][pos[1]])
			return False
		self.values[pos[0]][pos[1]] = player
		print('Placed')
		return True

	# If cell is empty (== 0) and if no conflict with additional rules
	def get_moves(self, player):
		print('get_moves')
		# get list of (all ?) possible placements
		moves = []
		for y, row in enumerate(self.values):
			for x, val in enumerate(row):
				if (val == 0):
					moves.append([y, x])
		return moves

	def heuristic(self, player):
		def eval_line(start, next):
			total_val = 0
			streak_val = 0
			streak_front_blocked = True
			
			return (1 if self.values[start[0]][start[1]] == player else 0) + eval_line(next(start), next) if next(start) else 0
		
		# count value for player
		# {|1111} < {1111}
		# {|1111|} == 0
		# {111} < {1111}
		value = 0

		# horizontal
		for y, row in enumerate(self.values):
			value += eval_line([y, 0], lambda pos: [pos[0], pos[1] + 1] if pos[1] + 1 < self.size else False)
		# vertical
		for x, col in enumerate(self.values.transpose()):
			value += eval_line([0, x], lambda pos: [pos[0] + 1, pos[1]] if pos[0] + 1 < self.size else False)
		# dialognal
		# 
		return value

	def print(self):
		s = [[str(e) for e in row] for row in self.values]
		lens = [max(map(len, col)) for col in zip(*s)]
		fmt = '\t'.join('{{:{}}}'.format(x) for x in lens)
		table = [fmt.format(*row) for row in s]
		print ('\n'.join(table))