import argparse

from classes.Minmax import Minmax
from classes.Board import Board
from interface.Game import Game


def main():

	parser = argparse.ArgumentParser()
	parser.add_argument('-s', action="store", dest="board_size", type=int, help='Size of the board')
	args = parser.parse_args()

	game = Game(args.board_size)
	board = Board(args.board_size)

	# ==== set up your callbacks ==== #
	def on_click(game, index):
		if game.is_playing:
			if game.current_player.is_AI():
				print('It\'s the AI\'s turn!');
				return
			# check that index is ok ...
			# ...
			# ...
			game.place_stone_at(index)														# place stone (color based on curent player, or pass as param)
			# game.place_stone_at([10, 10], RED)
			# game.remove_stone_from([0, 0])												# remove stone
			game.next_turn()																# start next player's turn
			new_turn()
		else:
			print('Start the game first!')
			game.message = 'Start the game first!'

	def on_start(game):
		print('game has started!')
		if game.current_player.is_AI():
			print('AI thinking ...')
			# DO AWESOME CODE
			game.message = 'AI thinking ...'												# set message

	def on_reset(game):
		print('game has reset.')
		# Reset controller

	def new_turn():
		print('new turn! It is ' + game.current_player.name + '\'s turn.')
		if game.current_player.is_AI():
			print('AI thinking ...')
			# DO AWESOME CODE
			# ...
			# index = AI.get_best_pos()
			# game.place_stone_at(index)
			# game.remove_stone_from(index2)

	game.on_reset = on_reset
	game.on_start = on_start
	game.on_grid_click = on_click

	# ==== start game loop! ==== #
	game.game_loop()


	board.place([0, 0], 1)
	board.place([0, 1], 1)
	board.place([1, 1], 1)
	board.place([2, 1], 2)
	board.print()
	print(board.get_moves())
	print(board.heuristic(1))
	# minmax = Minmax(
	# 	heuristic=lambda b: b.heuristic(0), #heuristic for player 0 
	# 	get_child_nodes=lambda b: b.get_moves(),
	# 	depth=10)



if __name__ == '__main__':
	main()