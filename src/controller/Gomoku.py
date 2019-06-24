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

dist_from_edge_value = 10
near_last_move_value = 10
near_last_move_dist = 1

class Gomoku:

	def __init__(self, rules, size=19, is_copy=False):
		if (size < 1):
			raise Exception('Board size must be at least 5')
		if not is_copy:
			self.size = size
			self.players = [Player(Player.TYPE.AI), Player(Player.TYPE.AI)]
			self.values = [0, 0]
			self.deltas = [0, 0]
			self.last_moves = [None, None]
			self.current_move = None
			# self.rules = rules
			self.current_player = self.players[0]
			self.board = [[0 for y in range(size)] for i in range(size)]
			self.rules = [RuleFactory.instantiate(key) for key in rules]
			self.end_game = False
			self.minimax = Minmax(heuristic=self.heuristic, get_child_nodes=self.get_child_nodes, depth=2)
			self.intersection_validity_array = [[1 for y in range(size)] for i in range(size)]
			self.remaining_cells = size ** 2
			self.value_grid = self.set_value_grid()
			self.undo_stack = []
			self.turn_index = 0
			self.record_undos = False
			self.game_started = False

	def set_value_grid(self):
		value_grid = [[dist_from_edge_value for y in range(self.size)] for i in range(self.size)]
		for i in range(self.size):
			for j in range(self.size):
				value_grid[i][j] += min(i, j, self.size - 1 - i, self.size - 1 - j)
		return value_grid

	def place(self, interface, pos): #pos: [row, col]
		# print('{} placed at pos {}'.format(self.current_player.index, pos))
		if self.intersection_validity_array[pos[0]][pos[1]] == 1:
			self.simple_place([pos[0], pos[1]])
			self.add_undo('simple_remove', pos)


			# print('PLACE')
			# print('pos', pos)
			# print('    last_moves', self.last_moves)
			self.add_undo('set_last_moves', self.last_moves)
			self.last_moves[self.current_player.index - 1] = copy.deepcopy(self.current_move)
			# print(' => last_moves', self.last_moves)
			
			last_current_move = copy.deepcopy(self.current_move)
			self.add_undo('set_current_move', last_current_move)
			self.current_move = pos.copy()

			# print('========= PLACED ' + str(self.current_player.index))
			self.trigger_rules_effects(interface, pos)

			if self.is_end_state(pos):
				# print('is end state but interface = ', interface)
				self.set_end_game(True)
				self.add_undo('set_end_game', False)
				self.win(interface)
			if self.remaining_cells == 0:
				self.set_end_game(True)
				self.add_undo('set_end_game', False)
				if interface:
					interface.message = "Draw..."
			return True
		return False
	
	def undo(self):
		# print('UNDO')
		if not self.record_undos:
			return 
		callbacks = self.undo_stack.pop()
		self.update_next_turn(-1)
		if callbacks:
			for callback in callbacks:
				# print('executing', callback)
				callback()
		# print('UNDO END')

	def add_undo(self, *value):
		# print('add_undo', value)
		if not self.record_undos:
			return 
		while len(self.undo_stack) - 1 < self.turn_index:
			self.undo_stack.append([])
		
		# for value in values.items():

		name = value[0]
		# value.pop(0)
		copied_value = copy.deepcopy(value[1:])
		# print('copied; ', copied_value)
		fn = getattr(self, name)
		if fn:
			self.undo_stack[self.turn_index].append(lambda: fn(*copied_value))
		# if self.turn_index == 2:
		# 	sys.exit()

	def simple_place(self, pos):
		# print('[simple_place] pos', pos)
		self.remaining_cells -= 1
		self.board[pos[0]][pos[1]] = self.current_player.index

	def simple_remove(self, pos):
		# print('[simple_remove] pos', pos)		
		self.remaining_cells += 1
		self.board[pos[0]][pos[1]] = 0

	def set_last_moves(self, last_moves):
		# print('set_last_moves', self.last_moves, last_moves)
		self.last_moves = last_moves
	
	def set_current_move(self, current_move):
		# print('set_current_move', current_move)
		self.current_move = current_move
	
	def update_validity_array(self, index, value):
		self.intersection_validity_array[index[0]][index[1]] = value
	
	def set_intersection_validity_array(self, value):
		self.intersection_validity_array = value

	def update_next_turn(self, delta):
		self.turn_index += delta
	
	def set_current_player(self, player):
		self.current_player = player

	def set_captures(self, values):
		# print(values)
		# print('set_captures for', values['index'], ' captures: ', values['captures'])
		self.players[values['index'] - 1].captures = values['captures']

	def set_deltas(self, value):
		self.deltas = value
	
	def set_values(self, value):
		self.values = value

	def set_end_game(self, value):
		self.end_game = value

	def remove(self, interface, pos):
		self.simple_remove(pos)
		# print('[remove]', pos)
		self.add_undo('simple_place', pos)
		if interface:
			interface.remove_stone_from(pos)

	def human_turn(self, interface, pos):
		can_place = self.place(interface, pos)
		if can_place:
			interface.place_stone_at(pos)
			self.update_next_turn(1)
			if not self.end_game:
				interface.message = "!"
				self.next_turn(interface)
		else:
			interface.message = "Can't place here"

	def ai_turn(self, interface):
		start_time = time.time()
		self.record_undos = True
		self.update_next_turn(-1)
		res = self.minimax.run(self, None, 11, -math.inf, math.inf, True)
		pygame.time.wait(100)
		self.record_undos = False

		end_time = time.time()

		print('It took {0:.6f} to compute pos'.format(end_time - start_time), res[1], 'for player', interface.current_player.name)

		self.place(interface, res[1])
		self.update_next_turn(1)
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

	def trigger_rules_effects(self, interface, pos):
		for rule in self.rules:
			rule.trigger_effect(self, interface, pos)

	def next_turn(self, interface):
		if interface:
			interface.next_turn()
		self.current_player = self.players[0] if self.current_player == self.players[1] else self.players[1]
		self.intersection_validity_array = [[intersection_validity_pos(self, [i, y]) for y in range(len(self.board))] for i in range(len(self.board))]
		if interface:
			interface.intersection_validity_array = self.intersection_validity_array

	# If cell is empty (== 0) and if no conflict with additional rules
	def get_moves(self, player):
		# print('get_moves')
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
				# print('captures: ', self.current_player.captures)
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
		# print('Heuristic. current_move:', self.current_move, 'current_player', self.current_player.index)
		current_player = self.current_player.index
		other_player = 1 if self.current_player.index == 2 else 2

		score = [{}, {}]

		# get current player's last move self.current_move

		# eval delta for both players

		# value grid

		# print('value grid', self.value_grid)

		score[current_player - 1]['value_grid'] = self.value_grid[self.current_move[0]][self.current_move[1]]

		# near last move

		last_move = copy.deepcopy(self.last_moves[current_player - 1])
		score[current_player - 1]['near_last_move_value'] = near_last_move_value if last_move and abs(last_move[0] - self.current_move[0]) <= near_last_move_dist and abs(last_move[1] - self.current_move[1]) <= near_last_move_dist else 0
		# print('HEURISTIC')
		# print('last_move' , last_move)
		# print('self.current_move' , self.current_move)
		# print('near_last_move_dist', near_last_move_dist)
		# print('score[current_player - 1][near_last_move_value]', score[current_player - 1]['near_last_move_value'])
		# print('HEURISTIC END')
		# eval line

		# streak

		# capture
		# :(
		# if self.current_player.captures == 5:
		# 	score[current_player - 1]['captures']

		# potential capture


		# strategy ??






		def eval_line(start, line):
			streak_num = 1

			# left = []
			# right = []

			for direction in [1, -1]:
				streaking = True
				for i in range(1, 5):
					next_pos = start + line * i * direction
					# if pos out of board
					if next_pos[0] > (len(self.board) - 1) or next_pos[1] > (len(self.board) - 1) or next_pos[0] < 0 or next_pos[1] < 0:
						break
					# if pos current player
					if streaking and self.board[next_pos[0]][next_pos[1]] == current_player:
						streak_num *= (streak_num + 1) ** 2
					else:
						streaking = False
					# if direction == 1: right.append(self.board[next_pos[0]][next_pos[1]])
					# if direction == -1: left.append(self.board[next_pos[0]][next_pos[1]])


			# for i in range(1, 5):
			# 	streaking = True
			# 	for direction in [1, -1]:
			# 		next_pos = start + line * i * direction
			# 		if next_pos[0] > (len(self.board) - 1) or next_pos[1] > (len(self.board) - 1) or next_pos[0] < 0 or next_pos[1] < 0:
			# 			break
			# 		if streaking and self.board[next_pos[0]][next_pos[1]] == current_player:
			# 			streak_num += 1
			# 		else:
			# 			streaking = False
			# 		if direction == 1: right.append(self.board[next_pos[0]][next_pos[1]])
			# 		if direction == -1: left.append(self.board[next_pos[0]][next_pos[1]])

			# left.reverse()


			score[current_player - 1]['streak'] += streak_num

			# if streak_num == 5:
			# 	score[current_player - 1]['streak'] += 1000000
				
			# print('line: ', line, '    ', left, right, 'streak num: ', streak_num, 'streak', score[current_player - 1]['streak'], )


		score[current_player - 1]['streak'] = 0
		# score[current_player - 1]['capture'] = 3 * (0 if not self.current_player.captures else math.factorial(self.current_player.captures))
		# score[current_player - 1]['potential_capture'] = 0
		# score[current_player - 1]['combination'] = 0

		for line in np.array([[0, 1], [1, 0], [1, 1], [-1, 1]]):
		# for line in np.array([[1, 1]]):
			eval_line(np.array(self.current_move), line)
		


		# print('score for player', self.current_player.index, ':', score[current_player - 1])
		# print('score:', sum(score[current_player - 1].values()))
		# sys.exit()
		# return value
		return sum(score[current_player - 1].values())

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
								self.intersection_validity_array[i + k][j + l] == 1):# and len(children) < 10:
									children.add((i + k, j + l))

		if len(children) == 0:
			children.add((int(len(self.board) / 2), int(len(self.board) / 2)))

		# print('children', children)
		# print('remaining spots', self.remaining_cells)
		# end = time.time()
		# print('get_child_node time: ', end - start)
		# print([list(el) for el in list(children)])
		# print('\n')
		return [list(el) for el in list(children)]

	def print_board(self):
		s = [[str(e) for e in row] for row in self.board]
		lens = [max(map(len, col)) for col in zip(*s)]
		fmt = '\t'.join('{{:{}}}'.format(x) for x in lens)
		table = [fmt.format(*row) for row in s]
		print ('\n'.join(table))
		print ('\n')

	def print_my_board(self, board):
		s = [[str(e) for e in row] for row in board]
		lens = [max(map(len, col)) for col in zip(*s)]
		fmt = '\t'.join('{{:{}}}'.format(x) for x in lens)
		table = [fmt.format(*row) for row in s]
		print ('\n'.join(table))
		print ('\n')