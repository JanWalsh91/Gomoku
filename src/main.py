
import argparse

from controller.Minmax import Minmax
from controller.Gomoku import Gomoku
from controller.rules.ARule import ARule
from controller.rules.RuleFactory import RuleFactory
from interface.Interface import Interface

default_rules = [RuleFactory.Name.CAPTURE, RuleFactory.Name.CAPTURE_GAME_ENDING, RuleFactory.Name.NO_DOUBLE_THREE]

rules_dictionary = {'r' + str(i + 1):  rule for i, rule in enumerate(RuleFactory.Name)}

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
	if RuleFactory.Name.CAPTURE_GAME_ENDING in rules and RuleFactory.Name.CAPTURE not in rules:
		rules.append(RuleFactory.Name.CAPTURE)


	# ==== create interface (line_num optional) ==== #
	if args.board_size:
		go = Gomoku(rules, args.board_size)
		interface = Interface(go.players, args.board_size)
	else:
		go = Gomoku(rules)
		interface = Interface(go.players)

	# ==== set up your callbacks ==== #
	def on_click(interface, pos):
		if interface.is_playing:
			if interface.current_player.is_AI():
				print('It\'s the AI\'s turn!')
				return
			# check that pos is ok ...
			# ...
			# ...
			can_place = go.place(interface, pos)
			if can_place:
				interface.place_stone_at(pos)														# place stone (color based on curent player, or pass as param)
				if not go.end_game:
					interface.message = "!"
					# interface.place_stone_at([10, 10], RED)
					# interface.remove_stone_from([0, 0])												# remove stone
					interface.next_turn()																# start next player's turn
					go.next_turn()
			else:
				interface.message = "Can't place here"
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
		go.reset()

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







	# board.place([0, 0], 1)
	# board.place([0, 1], 1)
	# board.place([1, 1], 1)
	# board.place([2, 1], 2)
	# board.print()
	# print(board.get_moves())
	# print(board.heuristic(1))


if __name__ == '__main__':
	main()