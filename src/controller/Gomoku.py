import numpy as np
from random import randint


from controller.Player import Player
from controller.rules.RuleFactory import RuleFactory
from controller.helpers import five_aligned, can_place_pos, intersection_validity_pos
from controller.Minmax import Minmax

class Gomoku:

	def __init__(self, rules, size=19):
		if (size < 1):
			raise Exception('Board size must be at least 5')
		self.size = size
		self.players = [Player(Player.TYPE.HUMAN), Player(Player.TYPE.AI)]
		self.rules = rules
		self.current_player = self.players[0]
		self.board = [[0 for y in range(size)] for i in range(size)]
		self.rules = [RuleFactory.instantiate(key) for key in rules]
		self.end_game = False
		self.minimax = Minmax(heuristic=self.heuristic, get_child_nodes=self.get_child_nodes, depth=2)
		self.intersection_validity_array = [[1 for y in range(size)] for i in range(size)]
		self.remaining_cells = size ** 2

	def place(self, interface, pos): #pos: [row, col]
		print('place')
		if self.intersection_validity_array[pos[0]][pos[1]] == 1:
			self.remaining_cells -= 1
			self.board[pos[0]][pos[1]] = self.current_player.index
			print('========= PLACED ' + str(self.current_player.index))
			self.trigger_rules_effects(interface, pos)

			if self.is_end_state(pos):
				self.win(interface)
				self.end_game = True
			if self.remaining_cells == 0:
				self.end_game = True
				interface.message = "Draw..."
			return True
		return False
	
	def remove(self, interface, pos):
		self.board[pos[0]][pos[1]] = 0
		interface.remove_stone_from(pos)
		self.remaining_cells += 1



	# TODO: already done in the next turn function
	# def can_place(self, pos):
		# if (0 > pos[0] or pos[0] > self.size or 0 > pos[1] or pos[1] > self.size):
		# 	print('bad place')
		# 	return False
		# if (self.board[pos[0]][pos[1]] != 0):
		# 	print('place already has value', self.board[pos[0]][pos[1]])
		# 	return False
		# for rule in self.rules:
		# 	if not rule.can_place(self, pos):
		# 		print('Rule ', rule.name, ' says NO')
		# 		return False
		# return True
		# return interface.intersection_validity_array[pos[0]][pos[1]] == 1

	def trigger_rules_effects(self, interface, pos):
		for rule in self.rules:
			rule.trigger_effect(self, interface, pos)

	def next_turn(self, interface):
		self.current_player = self.players[0] if self.current_player == self.players[1] else self.players[1]
		self.intersection_validity_array = [[intersection_validity_pos(self, [i, y]) for y in range(len(self.board))] for i in range(len(self.board))]
		interface.intersection_validity_array = self.intersection_validity_array
		

	# If cell is empty (== 0) and if no conflict with additional rules
	def get_moves(self, player):
		print('get_moves')
		# get list of (all ?) possible placements
		moves = []
		for y, row in enumerate(self.board):
			for x, val in enumerate(row):
				if (val == 0):
					moves.append([y, x])
		return moves
	
	def is_end_state(self, pos):
		for rule in self.rules:
			if rule.is_winning_condition(self):
				return True
		
		if five_aligned(self, pos):
			return True

		return False

	def win(self, interface):
		interface.win()

	def reset(self):
		print('reset')
		self.players[0].captures = 0
		self.players[1].captures = 0
		self.board = [[0 for y in range(len(self.board))] for i in range(len(self.board))]
		self.intersection_validity_array = [[1 for y in range(len(self.board))] for i in range(len(self.board))]
		self.current_player = self.players[0]
		self.end_game = False
		self.remaining_cells = len(self.board) ** 2

	def heuristic(self, player):
		return randint(0, 9)
		def eval_line(start, next):
			total_val = 0
			streak_val = 0
			streak_front_blocked = True
			
			return (1 if self.board[start[0]][start[1]] == player else 0) + eval_line(next(start), next) if next(start) else 0
		
		# count value for player
		# {|1111} < {1111}
		# {|1111|} == 0
		# {111} < {1111}
		value = 0

		# horizontal
		for y, row in enumerate(self.board):
			value += eval_line([y, 0], lambda pos: [pos[0], pos[1] + 1] if pos[1] + 1 < self.size else False)
		# vertical
		for x, col in enumerate(self.board.transpose()):
			value += eval_line([0, x], lambda pos: [pos[0] + 1, pos[1]] if pos[0] + 1 < self.size else False)
		# dialognal
		# 
		return value

	def get_child_nodes(self):
		children = []
		for i in range(len(self.board)):
			for j in range(len(self.board)):
				if self.intersection_validity_array[i][j] == 1:
					children.append([i, j])
		# print('children', children)
		# print('remaining spots', self.remaining_cells)
		return children

	def print(self):
		s = [[str(e) for e in row] for row in self.board]
		lens = [max(map(len, col)) for col in zip(*s)]
		fmt = '\t'.join('{{:{}}}'.format(x) for x in lens)
		table = [fmt.format(*row) for row in s]
		print ('\n'.join(table))