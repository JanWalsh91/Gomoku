import numpy as np
import time
import copy

from random import randint
import pygame
import math
from collections import Counter
import sys

from controller.Player import Player
from controller.rules.RuleFactory import RuleFactory
from controller.helpers import five_aligned, can_place_pos, intersection_validity_pos
from controller.Minmax import Minmax

class Gomoku:

	def __init__(self, rules, size=19, is_copy=False):
		if (size < 1):
			raise Exception('Board size must be at least 5')
		if not is_copy:
			self.size = size
			self.players = [Player(Player.TYPE.AI), Player(Player.TYPE.AI)]
			# self.rules = rules
			self.current_player = self.players[0]
			self.board = [[0 for y in range(size)] for i in range(size)]
			self.rules = [RuleFactory.instantiate(key) for key in rules]
			self.end_game = False
			self.minimax = Minmax(heuristic=self.heuristic, get_child_nodes=self.get_child_nodes, depth=2)
			self.intersection_validity_array = [[1 for y in range(size)] for i in range(size)]
			self.remaining_cells = size ** 2

	def place(self, interface, pos): #pos: [row, col]
		# print('{} placed at pos {}'.format(self.current_player.index, pos))
		if self.intersection_validity_array[pos[0]][pos[1]] == 1:
			self.remaining_cells -= 1
			self.board[pos[0]][pos[1]] = self.current_player.index
			# print('========= PLACED ' + str(self.current_player.index))
			self.trigger_rules_effects(interface, pos)

			if self.is_end_state(pos):
				self.end_game = True
				self.win(interface)
			if self.remaining_cells == 0:
				self.end_game = True
				if interface:
					interface.message = "Draw..."
			return True
		return False
	
	def remove(self, interface, pos):
		self.board[pos[0]][pos[1]] = 0
		if interface:
			interface.remove_stone_from(pos)
		self.remaining_cells += 1

	def human_turn(self, interface, pos):
		can_place = self.place(interface, pos)
		if can_place:
			interface.place_stone_at(pos)
			if not self.end_game:
				interface.message = "!"
				self.next_turn(interface)
		else:
			interface.message = "Can't place here"

	def ai_turn(self, interface):
		start_time = time.time()
		res = self.minimax.run(self, None, 2, -math.inf, math.inf, True)
		end_time = time.time()

		print('It took {0:.6f} to compute pos'.format(end_time - start_time), res[1], 'for player', interface.current_player.name)

		self.place(interface, res[1])
		interface.place_stone_at(res[1])

		# TODO: calling next turn here blocks the pygame main loop
		print('interface.render_game_objects()')
		interface.render_game_objects() # meh ?
		
		if not self.end_game:
			interface.message = str(self.remaining_cells)
			self.next_turn(interface)
			# pygame.time.wait(500)
			# time.sleep(0.5)
			print('AI thought ...', interface.current_player.name)

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
		interface.next_turn()
		self.current_player = self.players[0] if self.current_player == self.players[1] else self.players[1]
		self.intersection_validity_array = [[intersection_validity_pos(self, [i, y]) for y in range(len(self.board))] for i in range(len(self.board))]
		interface.intersection_validity_array = self.intersection_validity_array
		print(interface.current_player.name, 'will play')
		if self.current_player.is_AI():
			self.ai_turn(interface)

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
				print('rule ', rule.name, 'says WIN at pos', pos, ' for player', self.current_player.index)
				return True
		if five_aligned(self, pos):
			print('rule five aligned says WIN at pos', pos, ' for player', self.current_player.index)
			return True

		return False

	def win(self, interface):
		if interface:
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

	def heuristic(self):
		def eval_line(start, direction):
			# print('eval_line', start, direction)

			def eval_streak(value):
				# print('eval_streak')
				if prev:
					if value == prev:
						streak_val[value] += streak_val[value]		
					else:
						if not streak_front_blocked:
							if value == 0:
								total_val[other_player] += streak_val[other_player] * 2
							else:
								total_val[other_player] += streak_val[value]
						elif value == 0:
							total_val[other_player] += streak_val[other_player]
						streak_val[other_player] = 1

			total_val = [None, 0, 0]
			streak_val = [None, 1, 1]
			streak_front_blocked = True

			pos = start
			prev = None

			while pos[0] < len(self.board) and pos[1] < len(self.board):
				value = self.board[pos[0]][pos[1]]
				if value == 0:
					eval_streak(value)				
					streak_front_blocked = False
				else: 
					other_player = 1 if value == 2 else 2
					total_val[value] += 1 
					eval_streak(value)
				prev = value
				pos[0] += direction[0]
				pos[1] += direction[1]
				
			
			value = 1 if value == 2 else 2
			eval_streak(value)
			return total_val[self.current_player.index] - total_val[1 if self.current_player.index == 2 else 2]
		
		# count value for player
		# {|1111} < {1111}
		# {|1111|} == 0
		# {111} < {1111}
		value = 0

		# horizontal
		for i in range(len(self.board)):
			value += eval_line([i, 0], [0, 1])			
		# vertical
		for i in range(len(self.board)):
			value += eval_line([0, i], [1, 0])
		# dialognal
		# 

		# print('value:', value)
		# sys.exit()
		return value


	def copy(self):
		new_go = Gomoku(self.rules, self.size, is_copy=True)

		new_go.size = self.size
		new_go.players = [copy.copy(self.players[0]), copy.copy(self.players[1])]
		new_go.current_player = new_go.players[0] if self.current_player.index == 1 else new_go.players[1]
		new_go.board = copy.deepcopy(self.board)
		new_go.rules = self.rules
		new_go.end_game = self.end_game
		new_go.minimax = self.minimax
		new_go.intersection_validity_array = copy.deepcopy(self.intersection_validity_array)
		new_go.remaining_cells = self.remaining_cells
	
		return new_go

	def get_child_nodes(self):
		dist = 2
		# start = time.time()
		children = set()
		for i in range(len(self.board)):
			for j in range(len(self.board)):
				if self.board[i][j] != 0:
					# add everything around if valid
					for k in range(-dist, dist + 1):
						for l in range(-dist, dist + 1):
							# if on board
							if (i + k >= 0 and i + k < len(self.board) and j + l >= 0 and j + l < len(self.board) and
								# if can place
								self.intersection_validity_array[i + k][j + l] == 1):
									children.add((i + k, j + l))

		if len(children) == 0:
			children.add((int(len(self.board) / 2), int(len(self.board) / 2)))

		# print('children', children)
		# print('remaining spots', self.remaining_cells)
		# end = time.time()
		# print('get_child_node time: ', end - start)
		return [list(el) for el in list(children)]

	def print(self):
		s = [[str(e) for e in row] for row in self.board]
		lens = [max(map(len, col)) for col in zip(*s)]
		fmt = '\t'.join('{{:{}}}'.format(x) for x in lens)
		table = [fmt.format(*row) for row in s]
		print ('\n'.join(table))