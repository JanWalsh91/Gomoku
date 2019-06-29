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
		self.is_playing = False

	def place(self, pos, player):
		# update board
		self.board[pos[0]][pos[1]] = player.index

	def switch_player(self):
		self.current_player = self.players[0] if self.current_player == self.players[1] else self.players[1]
		# print(self.get_moves())
		# self.print_my_board(self.board)

	# === minmax functions === 

	def heuristic(self):

		def eval_line(start, line):
			# print('eval line', start, line)

			score = 0
			current_streak_score = 0
			streaking = False
			
			for i in range(self.size):
				pos = start + line * i
				if self.board[pos[0]][pos[1]] == self.current_player.index:
					current_streak_score = (current_streak_score + 1) ** 2
					streaking = True
				else:
					score += current_streak_score
					current_streak_score = 0
					streaking = False


			score += current_streak_score
			current_streak_score = 0
			streaking = False	

			# if score != 0:
			# 	print('line: ', start, score)

			return score

		score = 0
		for i in range(self.size):
			for start in [np.array([x, 0]) for x in range(self.size)]:
				score += eval_line(start, np.array([0, 1]))
			for start in [np.array([0, y]) for y in range(self.size)]:
				score += eval_line(start, np.array([1, 0]))						
		
		return score

	# returns [[1, 0], ...]
	def get_moves(self):
		dist = 2
		children = set()
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
		self.switch_player()
		self.place(pos, self.current_player)
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
