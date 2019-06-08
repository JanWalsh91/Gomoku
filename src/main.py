from controller.Minmax import Minmax
from controller.Board import Board
from interface.Interface import Interface
from common.Player import Player

# ==== EXAMPLE USAGE ==== #
def main():
	# ==== create interface (line_num optional) ==== #
	interface = Interface([Player(Player.TYPE.AI), Player(Player.TYPE.HUMAN)])

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
		else:
			print('click start!')
	interface.on_grid_click = on_click															# on click

	def on_start(interface):
		print('interface has started!')
		if interface.current_player.is_AI():
			print('AI thinking ...')
			# DO AWESOME CODE
			interface.message = 'AI thinking ...'												# set message
	interface.on_start = on_start																# on start

	def on_reset(interface):																		# on reset
		print('interface has reset.')
	interface.on_reset = on_reset

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
	interface.intersection_validity_array = [[2 for y in range(19)] for i in range(19)]


	# ==== start interface loop! ==== #
	interface.loop()																			# loop










if __name__ == '__main__':
	main()