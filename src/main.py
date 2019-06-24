
import argparse
import pygame
import sys
from threading import Thread


from controller.Minmax import Minmax
from controller.Gomoku import Gomoku
from controller.rules.ARule import ARule
from controller.rules.RuleFactory import RuleFactory
from interface.Interface import Interface

import time

# default_rules = [RuleFactory.Name.CAPTURE, RuleFactory.Name.CAPTURE_GAME_ENDING, RuleFactory.Name.NO_DOUBLE_THREE]
default_rules = []

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
				print('It\'s the AI\'s turn!', interface.current_player.name)
				return
			go.human_turn(interface, pos)
		else:
			print('click start!')

	def on_start(interface):
		print('game has started!')
		go.game_started = True

	def on_reset(interface):
		print('interface has reset.')
		go.reset()

	def on_player_change_type(player_view_model):
		go.players[interface.players.index(player_view_model)].type = player_view_model.type


	interface.on_reset = on_reset
	interface.on_start = on_start
	interface.on_grid_click = on_click
	interface.players[0].on_change_type = on_player_change_type
	interface.players[1].on_change_type = on_player_change_type


	# ==== start loop! ==== #
	turn = 0
	while True:
		if go.game_started and not go.end_game and go.current_player.is_AI():
			turn += 1
			print('======================', interface.current_player.name, go.current_player.index, 'will play', '======================')
			go.ai_turn(interface)
		interface.render()
		if turn >= 10:
			go.end_game = True

if __name__ == '__main__':
	main()