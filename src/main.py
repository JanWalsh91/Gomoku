
import argparse

from controller.Minmax import Minmax
from controller.Gomoku import Gomoku
from interface.Interface import Interface
from common.Player import Player

default_rules = [Gomoku.Rules.CAPTURE, Gomoku.Rules.GAME_ENDING_CAPTURE, Gomoku.Rules.NO_DOUBLE_THREE]

rules_dictionary = {'r' + str(i + 1):  rule for i, rule in enumerate(Gomoku.Rules)}

# ==== EXAMPLE USAGE ==== #
def main():

	parser = argparse.ArgumentParser()
	parser.add_argument('-s', action="store", dest="board_size", type=int, help='Size of the board')

	parser.add_argument('-r0', action='store_true', default=False, dest='r0', help='Remove default rules')
	for key in rules_dictionary:
		parser.add_argument('-' + key, action='store_true', default=False, dest=key, help='Rule ' + key)

	args = parser.parse_args()
	rules = [] if args.r0 else default_rules
	for key in rules_dictionary:
		if vars(args)[key]:
			rules.append(rules_dictionary[key])
	if Gomoku.Rules.GAME_ENDING_CAPTURE in rules and Gomoku.Rules.CAPTURE not in rules:
		rules.append(Gomoku.Rules.CAPTURE)

	# ==== create interface (line_num optional) ==== #
	if args.board_size:
		interface = Interface([Player(Player.TYPE.AI), Player(Player.TYPE.HUMAN)], args.board_size)
		board = Gomoku(rules, args.board_size)
	else:
		interface = Interface([Player(Player.TYPE.AI), Player(Player.TYPE.HUMAN)])
		board = Gomoku(rules)

	# ==== set up your callbacks ==== #
	def on_click(interface, index):
		if interface.is_playing:
			if interface.current_player.is_AI():
				print('It\'s the AI\'s turn!')
				return
			# check that index is ok ...
			# ...
			# ...
			interface.place_stone_at(index)														# place stone (color based on curent player, or pass as param)
			# interface.place_stone_at([10, 10], RED)
			# interface.remove_stone_from([0, 0])												# remove stone
			interface.next_turn()																# start next player's turn
			# board.blabla
		else:
			print('click start!')

	def on_start(interface):
		print('interface has started!')
		if interface.current_player.is_AI():
			print('AI thinking ...')
			# DO AWESOME CODE
			interface.message = 'AI thinking ...'												# set message

	def on_reset(interface):																		# on reset
		print('interface has reset.')

	def on_new_turn(interface):
		print('new turn! It is ' + interface.current_player.name + '\'s turn.')
		if interface.current_player.is_AI():
			print('AI thinking ...')
			# DO AWESOME CODE
			# ...
			# index = AI.get_best_pos()
			# interface.place_stone_at(index)
			# interface.remove_stone_from(index2)
	interface.on_new_turn = on_new_turn															# on new turn

	# TODO: update interfaces and colors
	# interface.intersection_validity_array = [[1 for y in range(19)] for i in range(19)]


	interface.on_reset = on_reset
	interface.on_start = on_start
	interface.on_grid_click = on_click

	# ==== start interface loop! ==== #
	interface.loop()																			# loop







	board.place([0, 0], 1)
	board.place([0, 1], 1)
	board.place([1, 1], 1)
	board.place([2, 1], 2)
	board.print()
	print(board.get_moves())
	print(board.heuristic(1))


if __name__ == '__main__':
	main()