
import argparse
# import pygame
from controller.Minmax import Minmax
from controller.Gomoku import Gomoku
from controller.rules.ARule import ARule
from controller.rules.RuleFactory import RuleFactory
from interface.Interface import Interface

import time

default_rules = [RuleFactory.Name.CAPTURE, RuleFactory.Name.CAPTURE_GAME_ENDING, RuleFactory.Name.NO_DOUBLE_THREE]

rules_dictionary = {'r' + str(i + 1):  rule for i, rule in enumerate(RuleFactory.Name)}

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
			can_place = go.place(interface, pos)
			if can_place:
				interface.place_stone_at(pos)														# place stone (color based on curent player, or pass as param)
				if not go.end_game:
					interface.message = "!"
					go.next_turn(interface)
					interface.next_turn()																# start next player's turn
			else:
				interface.message = "Can't place here"
		else:
			print('click start!')

	def on_start(interface):
		print('game has started!')
		if interface.current_player.is_AI():
			print('AI thinking ...')
			# DO AWESOME CODE
			interface.message = 'AI thinking ...'												# set message
			res = go.minimax.run(go, None, 2, True)
			# print('res: ', res)
			go.place(interface, res[1])
			interface.place_stone_at(res[1])
			if not go.end_game:
				interface.message = "!"
				go.next_turn(interface)
				interface.next_turn()
			print('AI thought!')

	def on_reset(interface):																		# on reset
		print('interface has reset.')
		go.reset()

	def on_new_turn(interface):
		# print('new turn! It is ' + interface.current_player.name + '\'s turn.')
		if interface.current_player.is_AI():
			print('AI thinking ...', interface.current_player.name)
			res = go.minimax.run(go, None, 2, True)
			# print('res: ', res)
			go.place(interface, res[1])
			interface.place_stone_at(res[1])
			if not go.end_game:
				interface.message = str(go.remaining_cells)
				go.next_turn(interface)
				# pygame.time.wait(500)
				# TODO: calling next turn here blocks the pygame main loop
				interface.next_turn()
				print('AI thought ...', interface.current_player.name)
	
	interface.on_new_turn = on_new_turn															# on new turn
	interface.on_reset = on_reset
	interface.on_start = on_start
	interface.on_grid_click = on_click

	# ==== start interface loop! ==== #
	interface.loop()																			# loop

if __name__ == '__main__':
	main()