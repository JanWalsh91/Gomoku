from controller.Player import Player
from controller.rules.RuleFactory import RuleFactory

class Gomoku:

	def __init__(self, rules, size=19):
		if (size < 1):
			raise Exception('Board size must be at least 5')
		self.size = size
		self.players = [Player(Player.TYPE.HUMAN), Player(Player.TYPE.HUMAN)]
		self.rules = rules
		self.current_player = self.players[0]
		self.board = [[0 for y in range(size)] for i in range(size)]
		self.rules = [RuleFactory.instantiate(key) for key in rules]
		self.end_game = False

	def place(self, interface, pos): #pos: [row, col]
		print('place')
		if self.can_place(pos):
			self.board[pos[0]][pos[1]] = self.current_player.index
			print('========= PLACED ' + str(self.current_player.index))
			self.trigger_rules_effects(interface, pos)

			if self.is_end_state():
				self.win(interface)
				self.end_game = True
			return True
		return False
		
	def can_place(self, pos):
		if (0 > pos[0] or pos[0] > self.size or 0 > pos[1] or pos[1] > self.size):
			print('bad place')
			return False
		if (self.board[pos[0]][pos[1]] != 0):
			print('place already has value', self.board[pos[0]][pos[1]])
			return False
		for rule in self.rules:
			if not rule.can_place(self, pos):
				print('Rule ', rule.name, ' says NO')
				return False
		return True

	def trigger_rules_effects(self, interface, pos):
		for rule in self.rules:
			rule.trigger_effect(self, interface, pos)

	def next_turn(self):
		self.current_player = self.players[0] if self.current_player == self.players[1] else self.players[1]

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
	
	def is_end_state(self):
		for rule in self.rules:
			if rule.is_winning_condition(self):
				return True
		return False

	def win(self, interface):
		interface.message = interface.current_player.name + ' 1! :)'
		interface.is_playing = False

	def reset(self):
		print('reset')
		# self.players[0].captures = 0
		# self.players[1].captures = 0
		# print(self.board, len(self.board))
		# self.board = [[0 for y in range(len(self.board))] for i in range(len(self.board))]
		# self.current_player = self.players[0]
		# self.end_game = False

	def heuristic(self, player):
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

	def print(self):
		s = [[str(e) for e in row] for row in self.board]
		lens = [max(map(len, col)) for col in zip(*s)]
		fmt = '\t'.join('{{:{}}}'.format(x) for x in lens)
		table = [fmt.format(*row) for row in s]
		print ('\n'.join(table))