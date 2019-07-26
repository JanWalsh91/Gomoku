import sys, pygame
import pygame.gfxdraw
from enum import Enum, auto
from interface.GameObject import GameObject
from interface.gui.Gui import Gui
from interface.gui.Button import Button
from interface.PlayerViewModel import PlayerViewModel
from interface.gui.TextBox import TextBox
from interface.board.Board import Board
from interface.colors import *

BOARD_SIZE = 800 # BOARD (not window) size in pixels (=> determines window size based on ratio)

class Interface:
	"""
	Creates a Interface

	Args:
		line_num? (int=19): Number of lines on the grid

	Attributes:
		players (Player[]): list of players
		current_player (Player)
		message (string): message in message text box
	
	Set Callbacks:
		on_start
		on_reset
		on_click_grid
		on_new_turn
	"""
	ratio = 3 / 2

	def __init__(self, players, line_num=19):
		self.__on_current_player_change = []
		self.__on_grid_click = []
		self.__on_start = []
		self.__on_reset = []
		self.__on_key_down = []
		# self.__on_new_turn = []
		self.is_playing = False
		self.is_reset = True

		self.line_num = line_num # number of lines vertically and horizontally on board

		pygame.init()
		pygame.display.set_caption('Gomoku')

		self.screen = pygame.display.set_mode([int(BOARD_SIZE * Interface.ratio), BOARD_SIZE])
			
		self.board = Board(BOARD_SIZE, line_num=line_num, on_grid_click=lambda board, index: self.grid_click(index))

		self.players = [
			PlayerViewModel('Black', players[0]),
			PlayerViewModel('White', players[1])
		]

		self.current_player = self.players[0]

		self.init_gui()

		self.board.grid.hover_color_valid = self.current_player.stone_color # set hover color of current player

		self.game_objects = [
			self.board,
			self.gui
		]


	def init_gui(self):
		self.gui = Gui((BOARD_SIZE, 0), (BOARD_SIZE * Interface.ratio - BOARD_SIZE, BOARD_SIZE), background_color=LIGHT_GREY, border_color=GREY, border_width=5)
		
		# === player VS AI ===
		player_vs_AI_wrapper = Gui((5, 2.5), (90, 15), background_color=LIGHT_GREY, border_color=GREY, border_width=5)
		
		player1_btn = Button((5, 10), (30, 80), border_color=self.players[0].stone_color, border_width=5, font_size=20, color=self.players[0].stone_color)
		player1_btn.on_click = lambda button: self.players[0].change_type() if not self.is_playing else print('Can\'t change player type while in game!')
		player2_btn = Button((65, 10), (30, 80), border_color=self.players[1].stone_color, border_width=5, font_size=20, color=self.players[1].stone_color)
		player2_btn.on_click = lambda button: self.players[1].change_type() if not self.is_playing else print('Can\'t change player type while in game!')

		vs_textbox = TextBox((40, 40), (20, 20), text='vs', background_color=LIGHT_GREY, color=DARK_GREY)

		player_vs_AI_wrapper.insert(player1_btn)
		player_vs_AI_wrapper.insert(vs_textbox)
		player_vs_AI_wrapper.insert(player2_btn)

		def update_player_button(button, player):
			if player.is_AI():
				button.textbox.text = 'AI'
				button.background_color = AI
			elif player.is_human():
				button.textbox.text = 'Human'
				button.background_color = HUMAN
				
		self.players[0].on_change_type = lambda player: update_player_button(player1_btn, self.players[0])
		self.players[1].on_change_type = lambda player: update_player_button(player2_btn, self.players[1])

		# reset type to update buttons
		self.players[0].type = self.players[0].type
		self.players[1].type = self.players[1].type
		self.gui.insert(player_vs_AI_wrapper)
		# ===              ===

		# === captures ===
		captures_wrapper = Gui((5, 19), (90, 10), background_color=LIGHT_GREY, border_color=GREY, border_width=5)
		captures_label = TextBox((10, 10), (80, 30), background_color=LIGHT_GREY, font_size=20, text='Captures:')
		captures1_textbox = TextBox((5, 40), (30, 50), background_color=LIGHT_GREY, font_size=25, text='0')
		captures2_textbox = TextBox((65, 40), (30, 50), background_color=LIGHT_GREY, font_size=25, text='0')

		captures_wrapper.insert(captures_label)
		captures_wrapper.insert(captures1_textbox)
		captures_wrapper.insert(captures2_textbox)

		# setup captures callback
		def update_captures_textbox(textbox, player):
			textbox.text = str(player.captures)

		self.players[0].on_update_captures = lambda player: update_captures_textbox(captures1_textbox, player)
		self.players[1].on_update_captures = lambda player: update_captures_textbox(captures2_textbox, player)
		self.gui.insert(captures_wrapper)
		# ===          ===

		# === start / restart ===
		start_restart_wrapper = Gui((5, 30.5), (90, 15), background_color=LIGHT_GREY, border_color=GREY, border_width=5)
		start_restart_btn = Button((10, 10), (80, 80), border_color=BLACK, border_width=5, font_size=30, color=BLACK, background_color=FOREST_GREEN, text='START !')
		
		def update_start_button(button):
			if not self.is_reset:
				button.textbox.text = 'START !'
				button.background_color = FOREST_GREEN
				self.game_reset()
				self.is_playing = False
				self.is_reset = True
			else:
				button.textbox.text = 'RESET'
				button.background_color = LIGHT_GREY
				self.game_start()
				self.is_playing = True
				self.is_reset = False

		start_restart_btn.on_click = lambda button: update_start_button(button)
		start_restart_wrapper.insert(start_restart_btn)
		self.gui.insert(start_restart_wrapper)
		# ===                 ===

		# === current player ===
		current_player_wrapper = Gui((5, 47.5), (90, 20), background_color=LIGHT_GREY, border_color=GREY, border_width=5)
		current_player_header = TextBox((10, 10), (80, 20), text='Current Player:', font_size=20, background_color=LIGHT_GREY)
		current_player_text = TextBox((5, 30), (90, 60), background_color=LIGHT_GREY, font_size=45, text=self.current_player.name)
		current_player_wrapper.insert(current_player_text)
		current_player_wrapper.insert(current_player_header)

		def update_current_player_text(text):
			current_player_text.text = text
			current_player_text.color = self.current_player.stone_color

		update_current_player_text(self.current_player.name)
		self.on_current_player_change = lambda button: update_current_player_text(self.current_player.name)
		self.gui.insert(current_player_wrapper)	
		# ===                ===

		# === message box ===
		message_box_wrapper = Gui((5, 70), (90, 20), background_color=LIGHT_GREY, border_color=GREY, border_width=5)
		self.message_textbox = TextBox((10, 10), (80, 80), text='_', font_size=20, background_color=LIGHT_GREY)
		message_box_wrapper.insert(self.message_textbox)
		self.gui.insert(message_box_wrapper)	
		# ===             ===

	def render(self):
		self.handle_events()
		self.render_game_objects()


	def handle_events(self):
		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				sys.exit()
			if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
				sys.exit()
			
			if event.type == pygame.KEYDOWN:
				for callback in self.on_key_down:
					callback(event)
			
			self.gui.handle_event(event, self.screen)
			self.board.handle_event(event, self.screen)

	def render_game_objects(self):
		for game_object in self.game_objects:
			game_object.render(self.screen)
		pygame.display.flip()
	
	def next_turn(self):
		self.current_player = self.players[0] if self.current_player == self.players[1] else self.players[1]
		self.board.grid.hover_color_valid = self.current_player.stone_color
		self.render()
		# for callback in self.on_new_turn:
		# 	callback(self)

	def place_stone_at(self, index, color=None):
		self.board.place_stone_at(index, color if color else self.current_player.stone_color)

	def remove_stone_from(self, index):
		self.board.remove_stone_from(index)

	def game_start(self):
		self.is_playing = True
		for callback in self.on_start:
			callback(self)

	def game_reset(self):
		self.is_playing = False
		self.message = '_'
		self.board.reset()
		self.current_player = self.players[0]
		for player in self.players:
			player.captures = 0
		for callback in self.on_reset:
			callback(self)

	def win(self):
		self.is_playing = False
		self.message = self.current_player.name + ' wins!'

	@property
	def current_player(self):
		return self.__current_player
	@current_player.setter
	def current_player(self, player):
		self.__current_player = player
		for callback in self.on_current_player_change:
			callback(self)

	@property
	def on_current_player_change(self):
		return self.__on_current_player_change
	@on_current_player_change.setter
	def on_current_player_change(self, fn):
		self.__on_current_player_change.append(fn)

	@property
	def on_key_down(self):
		return self.__on_key_down
	@on_key_down.setter
	def on_key_down(self, fn):
		self.__on_key_down.append(fn)

	@property
	def message(self):
		return self.message_textbox.text
	@message.setter
	def message(self, text):
		self.message_textbox.text = text

	def grid_click(self, index):
		for callback in self.on_grid_click:
			callback(self, index)

	@property
	def on_grid_click(self):
		return self.__on_grid_click
	@on_grid_click.setter
	def on_grid_click(self, callback):
		self.__on_grid_click.append(callback)

	@property
	def on_start(self):
		return self.__on_start
	@on_start.setter
	def on_start(self, callback):
		self.__on_start.append(callback)

	@property
	def on_reset(self):
		return self.__on_reset
	@on_reset.setter
	def on_reset(self, callback):
		self.__on_reset.append(callback)

	# @property
	# def on_new_turn(self):
	# 	return self.__on_new_turn
	# @on_new_turn.setter
	# def on_new_turn(self, callback):
	# 	self.__on_new_turn.append(callback)

	@property
	def on_player_type_change(self):
		return self.__on_player_type_change
	@on_player_type_change.setter
	def on_player_type_change(self, callback):
		self.__on_player_type_change.append(callback)

if __name__ == '__main__':
	main()
	