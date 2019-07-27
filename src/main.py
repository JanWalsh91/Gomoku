
import argparse
import pygame
import sys
import math

import GomokuModule
# from controller.rules.ARule import ARule
# from controller.rules.RuleFactory import RuleFactory
from interface.Interface import Interface
from interface.PlayerViewModel import PlayerViewModel

import time

# default_rules = [RuleFactory.Name.CAPTURE, RuleFactory.Name.CAPTURE_GAME_ENDING, RuleFactory.Name.NO_DOUBLE_THREE]
default_rules = []

# rules_dictionary = {'r' + str(i + 1):  rule for i, rule in enumerate(RuleFactory.Name)}
rules_dictionary = {}


def main():

	print('SET PAUSE')
	pause = False
	play_once = False
	turn = 0
	averageExecutionTime = 0
	
	parser = argparse.ArgumentParser()
	parser.add_argument('-t', action="store_true", dest="test", default=False, help='Launch line test')

	parser.add_argument('-s', action="store", dest="board_size", type=int, help='Size of the board')
	parser.add_argument('-d', action="store", dest="depth", type=int, help='Min max depth')
	
	parser.add_argument('-q', action="store", dest="max_turn", type=int, help='Turn timeout')

	parser.add_argument('-r0', action='store_true', default=False, dest='r0', help='Remove default rules')
	for key in rules_dictionary:
		parser.add_argument('-' + key, action='store_true', default=False, dest=key, help='Rule ' + key)

	args = parser.parse_args()
	if args.test == True:
		test_lines()
		return
	
	rules = [] if args.r0 else default_rules
	for key in rules_dictionary:
		if vars(args)[key]:
			rules.append(rules_dictionary[key])
	# if RuleFactory.Name.CAPTURE_GAME_ENDING in rules and RuleFactory.Name.CAPTURE not in rules:
	# 	rules.append(RuleFactory.Name.CAPTURE)

	# players = [PlayerViewModel.TYPE.AI, PlayerViewModel.TYPE.HUMAN]
	# players = [PlayerViewModel.TYPE.HUMAN, PlayerViewModel.TYPE.AI]
	players = [PlayerViewModel.TYPE.AI, PlayerViewModel.TYPE.AI]
	# players = [PlayerViewModel.TYPE.HUMAN, PlayerViewModel.TYPE.HUMAN]

	# ==== create interface (line_num optional) ==== #
	if args.board_size:
		GomokuModule.init(args.board_size, args.depth if args.depth else 5, int(players[0]), int(players[1]))
		interface = Interface(players, args.board_size)
	else:
		GomokuModule.init(19, args.depth if args.depth else 5, int(players[0]), int(players[1]))
		interface = Interface(players)
	
	def on_key_down(event):
		nonlocal pause
		nonlocal play_once
		print('on key down')
		if event.key == pygame.K_SPACE:
			print('Use PAUSE')
			print(pause)
			pause = not pause
		if event.key == pygame.K_PERIOD:
			play_once = True

	# ==== set up your callbacks ==== #
	def on_click(interface, pos):
		if GomokuModule.is_playing():
			if GomokuModule.is_current_player_AI():
				print('It\'s the AI\'s turn!', interface.current_player.name)
				return
			
			if not GomokuModule.can_place(pos[0], pos[1]):
				print('Can"t place here')
				return 
			GomokuModule.place(pos[0], pos[1])

			value = GomokuModule.get_end_state()
			print('value: ' + str(value))
			if value != -1:
				GomokuModule.set_playing(False)
				if value >= 0:
					interface.message = ("Black" if value == 0 else "White") + " win! " + str(turn)
				else:
					interface.message = "DRAW"
			
			GomokuModule.switch_player()

			interface.place_stone_at(pos)
			interface.render()
			interface.next_turn()
		else:
			print('click start!')

	def on_start(interface):
		print('game has started!')
		GomokuModule.set_playing(True)

	def on_reset(interface):
		print('interface has reset.')
		GomokuModule.reset()
		turn = 0
		averageExecutionTime = 0

	def on_player_change_type(player_view_model):
		GomokuModule.set_player_type(player_view_model.index, player_view_model.type)


	interface.on_key_down = on_key_down
	interface.on_reset = on_reset
	interface.on_start = on_start
	interface.on_grid_click = on_click
	interface.players[0].on_change_type = on_player_change_type
	interface.players[1].on_change_type = on_player_change_type
	
	print('Max Turn: ', args.max_turn)

	while True:
		interface.render()
	
		if GomokuModule.is_playing() and GomokuModule.is_current_player_AI():
			if not pause or play_once:
				turn += 1
				interface.message = str(turn)
				print('===> TURN ', turn)
				if args.max_turn and turn > args.max_turn:
					print('posing')
					pause = True
					args.max_turn = 0
					print('Average execution time: ', str(round(averageExecutionTime / turn, 4)))
					# GomokuModule.set_playing(False)
					if not play_once:
						continue

				start = time.time()
				pos = GomokuModule.run()
				end = time.time()

				print('AI, I choose you', pos, ' for player ', interface.current_player.name, ' in ', round(end - start, 4), 'seconds')
				averageExecutionTime = averageExecutionTime + end - start

				if pos:
					GomokuModule.place(pos[0], pos[1])

					value = GomokuModule.get_end_state()
					print('value: ' + str(value))
					if value != -1:
						GomokuModule.set_playing(False)
						print('Average execution time: ', str(round(averageExecutionTime / turn, 4)))
						if value >= 0:
							interface.message = ("Black" if value == 0 else "White") + " win! " + str(turn)  
						else:
							interface.message = "DRAW"

					interface.place_stone_at(pos)

					GomokuModule.switch_player()
					interface.next_turn()
				else:
					GomokuModule.set_playing(False)
				
			play_once = False


					


def test_lines():
	# init boards
	GomokuModule.test_eval_line()

if __name__ == '__main__':
	main()