import numpy as np
import time
import copy

from random import randint
import pygame
import math
from collections import Counter
import sys
import random

from controller.Player import Player
from controller.rules.RuleFactory import RuleFactory
from controller.helpers import five_aligned, can_place_pos, intersection_validity_pos
from controller.Minmax import Minmax

dist_from_edge_value = 10
near_last_move_value = 10
near_last_move_dist = 1

EMPTY = -1

class Gomoku:

	def __init__(self, rules, size=19):
		self.size = size
		self.board = np.full(shape=(size, size), fill_value=EMPTY, dtype=int) # 0, 1 => players. -1 empty.
		self.players = [Player(Player.TYPE.HUMAN), Player(Player.TYPE.AI)]
		self.current_player = self.players[0]
		self.heuristic_player = None
		self.is_playing = False
		self.last_moves = [None, None]

	def place(self, pos, player):
		# update board
		self.board[pos[0]][pos[1]] = player.index
		self.last_moves[player.index] = pos

	def switch_player(self):
		self.current_player = self.players[0] if self.current_player == self.players[1] else self.players[1]
		# print(self.get_moves())
		# self.print_my_board(self.board)

	# === minmax functions === 

	def hash_state(self):
		return hash(str(self.board))

	def heuristic(self):

		def heuristic_by_player(player):

			def eval_line(start, line):
				score = 0
				current_streak_score = 0
				streaking = False
				
				for i in range(self.size):
					pos = start + line * i
					if self.board[pos[0]][pos[1]] == player:
						current_streak_score = (current_streak_score * 3 + 3)
						streaking = True
					else:
						score += current_streak_score
						current_streak_score = 0
						streaking = False


				score += current_streak_score
				current_streak_score = 0
				streaking = False	

				# if score != 0:
				# print('line: ', start, line, score)
				return score

			score = 0
			for start in [np.array([x, 0]) for x in range(self.size)]:
				score += eval_line(start, np.array([0, 1]))
			for start in [np.array([0, y]) for y in range(self.size)]:
				score += eval_line(start, np.array([1, 0]))						
			
			# print('final score: ', score, ' for player ', self.heuristic_player.index)
			# self.print_my_board(self.board)
			
			return score
		
		return heuristic_by_player(self.heuristic_player.index) - int(heuristic_by_player(0 if self.heuristic_player.index == 1 else 1) * 1.1)

	# returns [[1, 0], ...]
	def get_moves(self):
		dist_around_last_moves = 1
		
		children = set()

		current_player = self.current_player.index

		for move in [self.last_moves[current_player], self.last_moves[0 if current_player == 1 else 1]]:
			if move:
				for i in range(-dist_around_last_moves, dist_around_last_moves + 1):
					i += move[0]
					for j in range(-dist_around_last_moves, dist_around_last_moves + 1):
						j += move[1]
						if i >= 0 and i < len(self.board) and j >= 0 and j < len(self.board) and self.board[i][j] == EMPTY:
							children.add((i, j))
							not_empty = True


		dist = 2
		not_empty = False
		for i in range(len(self.board)):
			for j in range(len(self.board)):
				if self.board[i][j] != EMPTY:
					# add everything around if valid
					for k in range(-dist, dist + 1):
						for l in range(-dist, dist + 1):
							# if on board
							if i + k >= 0 and i + k < len(self.board) and j + l >= 0 and j + l < len(self.board) and self.board[i + k][j + l] == EMPTY:
								# if can place (TODO)
								children.add((i + k, j + l))
				else:
					not_empty = True

		if len(children) == 0 and not_empty:
			children.add((int(len(self.board) / 2), int(len(self.board) / 2)))
		return [list(el) for el in list(children)]

	def do_move(self, pos):
		self.place(pos, self.current_player)
		self.switch_player()
		return 0

	def undo_move(self, pos):
		self.board[pos[0]][pos[1]] = EMPTY
		self.switch_player()
		return 'zero plus un'

	def print_my_board(self, board):
		s = [[str(e) for e in row] for row in board]
		lens = [max(map(len, col)) for col in zip(*s)]
		fmt = '\t'.join('{{:{}}}'.format(x) for x in lens)
		table = [fmt.format(*row) for row in s]
		print ('\n'.join(table))
		print ('\n')
