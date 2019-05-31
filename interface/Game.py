import sys, pygame
import pygame.gfxdraw
from enum import Enum, auto
from classes.GameObject import GameObject
from classes.gui.Gui import Gui
from classes.gui.Button import Button
from classes.Player import Player
from classes.gui.TextBox import TextBox
from classes.board.Board import Board
from colors import *

BOARD_SIZE = 600 # BOARD (not window) size in pixels (=> determines window size)

def main():
	game = Game()
	game.game_loop()



class Game:
	"""
	Creates a Game

	Args:
		line_num? (int=19): Number of lines on the grid

	Attributes:
		players (Player[]): list of players
	
	Set Callbacks:
		on_game_start
		on_game_reset
		on_click_grid
	"""
	ratio = 3 / 2

	def __init__(self, line_num=19):
		self.__on_current_player_change = []
		self.__on_intersection_click = []
		self.is_playing = False

		self.line_num = line_num # number of lines vertically and horizontally on board

		pygame.init()
		pygame.display.set_caption('Gomoku')
		# create space for gui on right
		self.screen = pygame.display.set_mode([int(BOARD_SIZE * Game.ratio), BOARD_SIZE])
		# self.background = Background(line_num)
		# self.intersections = Intersections(self.background.space_between_lines, line_num, self.background.line_width, size=self.background.space_between_lines)
		self.board = Board(BOARD_SIZE, on_intersection_click=lambda board, index: board.test())

		self.players = [
			Player('Salmon',Player.TYPE.AI, stone_color=SALMON),
			Player('Goldenrod', Player.TYPE.HUMAN, stone_color=GOLDENROD)
		]
		self.current_player = self.players[0];

		self.init_gui()

		self.game_objects = [
			# self.background,
			# self.intersections,
			self.board,
			self.gui
		]

	def init_gui(self):
		self.gui = Gui((BOARD_SIZE, 0), (BOARD_SIZE * Game.ratio - BOARD_SIZE, BOARD_SIZE), background_color=LIGHT_GREY, border_color=GREY, border_width=5)
		
		# === player VS AI ===
		player_vs_AI_wrapper = Gui((5, 2.5), (90, 20), background_color=LIGHT_GREY, border_color=GREY, border_width=5);
		
		player1_btn = Button((5, 5), (30, 90), border_color=self.players[0].stone_color, border_width=5, font_size=20, color=self.players[0].stone_color);
		player1_btn.on_click = lambda : self.players[0].change_type() if not self.is_playing else None;
		player2_btn = Button((65, 5), (30, 90), border_color=self.players[1].stone_color, border_width=5, font_size=20, color=self.players[1].stone_color);
		player2_btn.on_click = lambda : self.players[1].change_type() if not self.is_playing else None;

		vs_textbox = TextBox((40, 40), (20, 20), text='vs', background_color=LIGHT_GREY, color=DARK_GREY)

		player_vs_AI_wrapper.insert(player1_btn)
		player_vs_AI_wrapper.insert(vs_textbox)
		player_vs_AI_wrapper.insert(player2_btn)

		def update_player_button(button, player):
			if player.is_AI():
				button.textbox.text = 'AI'
				button.textbox.background_color = (200, 50, 50)
			elif player.is_human():
				button.textbox.text = 'Player'
				button.textbox.background_color = (50, 50, 200)

		self.players[0].on_change_type = lambda: update_player_button(player1_btn, self.players[0])
		self.players[1].on_change_type = lambda: update_player_button(player2_btn, self.players[1])

		# reset type to update buttons
		self.players[0].type = self.players[0].type
		self.players[1].type = self.players[1].type
		self.gui.insert(player_vs_AI_wrapper)
		# ===              ===

		# === start / restart ===
		start_restart_wrapper = Gui((5, 25), (90, 20), background_color=LIGHT_GREY, border_color=GREY, border_width=5);
		start_restart_btn = Button((10, 10), (80, 80), border_color=BLACK, border_width=5, font_size=30, color=BLACK, text='START')
		
		def update_start_button(button):
			if self.is_playing:
				button.textbox.text = 'START'
				self.on_game_start()
				self.is_playing = False
			else:
				button.textbox.text = 'RESET'
				self.on_game_reset()
				self.is_playing = True

		start_restart_btn.on_click = lambda: update_start_button(start_restart_btn)
		start_restart_wrapper.insert(start_restart_btn)
		self.gui.insert(start_restart_wrapper)
		# ===                 ===

		# === current player ===
		current_player_wrapper = Gui((5, 47.5), (90, 20), background_color=LIGHT_GREY, border_color=GREY, border_width=5);
		current_player_header = TextBox((10, 10), (80, 20), text='Current Player:', font_size=20, background_color=LIGHT_GREY)
		current_player_text = TextBox((5, 30), (90, 60), background_color=LIGHT_GREY, font_size=45, text=self.current_player.name)
		current_player_wrapper.insert(current_player_text)
		current_player_wrapper.insert(current_player_header)

		def update_current_player_text(text):
			current_player_text.text = text

		update_current_player_text(self.current_player.name)
		self.on_current_player_change = lambda: update_current_player_text(self.current_player.name)
		self.gui.insert(current_player_wrapper)	
		# ===                ===

		# === message box ===
		message_box_wrapper = Gui((5, 70), (90, 20), background_color=LIGHT_GREY, border_color=GREY, border_width=5);
		self.message_textbox = TextBox((10, 10), (80, 80), text='Message', font_size=20, background_color=LIGHT_GREY)
		message_box_wrapper.insert(self.message_textbox)
		self.gui.insert(message_box_wrapper)	
		# ===             ===

	def game_loop(self):
		while True:
			self.handle_events()
			self.render_game_objects()

	def handle_events(self):
		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				sys.exit()
			if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
				sys.exit()
			# if event.type == pygame.MOUSEBUTTONUP:
			# 	print('clicked')
				# rect = self.intersections.clicked_on((pygame.mouse.get_pos()))
				# if rect:
				# 	self.intersections.remove(rect)
				# 	placed = self.place_stone(rect.center)
				# 	if placed:
				# 		self.next_turn()
			
			self.gui.handle_event(event, self.screen)
			self.board.handle_event(event, self.screen)

	def render_game_objects(self):
		for game_object in self.game_objects:
			game_object.render(self.screen)
		pygame.display.flip()
	
	def place_stone(self, position):
		# TODO: call model/controller to handle rules preventing you from placing in certain places
		self.game_objects.append(Stone(self.current_player.stone_color, position, self.background.space_between_lines/2.2))
		return True
	
	def next_turn(self):
		self.current_player = self.players[0] if self.current_player == self.players[1] else self.players[1]

	
	# TODO: implement callbacks for
	def on_game_start(self): pass
	def on_game_reset(self): pass
	def on_click_grid(self): pass


	@property
	def current_player(self):
		return self.__current_player
	@current_player.setter
	def current_player(self, player):
		self.__current_player = player
		for x in self.on_current_player_change:
			x()

	@property
	def on_current_player_change(self):
		return self.__on_current_player_change
	@on_current_player_change.setter
	def on_current_player_change(self, fn):
		self.__on_current_player_change.append(fn)

	@property
	def message(self):
		return self.message_textbox.text
	@message.setter
	def message(self, text):
		self.message_textbox.text = text

	@property
	def on_intersection_click(self, index):
		return self.__on_intersection_click;
	@on_intersection_click.setter
	def on_intersection_click(self, fn):
		self.__on_intersection_click.append(fn)

# class Background(GameObject):
# 	def __init__(self, line_num=19, background_color=(133, 193, 233), line_color=(21, 67, 96), line_width=2):
# 		# init background
# 		self.surface = pygame.Surface([BOARD_SIZE, BOARD_SIZE]).convert()
# 		self.surface.fill(background_color)
# 		# init lines
# 		self.space_between_lines = BOARD_SIZE / (line_num + 1)
# 		self.line_width = line_width
# 		for i in range(line_num):
# 			i += 1
# 			dist = i * self.space_between_lines # distance from edge
# 			pygame.draw.line(self.surface, line_color, (dist, 0), (dist, BOARD_SIZE), line_width)
# 			pygame.draw.line(self.surface, line_color, (0, dist), (BOARD_SIZE, dist), line_width)
# 		self.position = (0, 0)
# 	def render(self, screen):
# 		screen.blit(self.surface, self.position)

# class Intersections(GameObject):
# 	def __init__(self, space_between_lines, line_num, line_width, size=50, hover_color=(250, 250, 250, 50)):
# 		self.hover_color = hover_color

# 		# set intersection positions
# 		self.rects = []
# 		for x in range(line_num):
# 			x += 1
# 			for y in range(line_num):
# 				y += 1
# 				pos = (x * space_between_lines - size / 2 + line_width / 2, y * space_between_lines - size / 2 + line_width / 2)
# 				self.rects.append(pygame.Rect(pos, (size, size)))

# 	# handle hover over rect
# 	def render(self, screen):
# 		for rect in self.rects:
# 			if rect.collidepoint(pygame.mouse.get_pos()):
# 				pygame.gfxdraw.box(screen, rect, self.hover_color)

# 	def clicked_on(self, pos):
# 		for rect in self.rects:
# 			if rect.collidepoint(pygame.mouse.get_pos()):
# 				return rect
# 		return None

# 	def remove(self, rect):
# 		self.rects.remove(rect)

# class Stone(GameObject):
# 	def __init__(self, color, position, size):
# 		self.color = color
# 		self.position = position
# 		self.size = int(size)

# 	def render(self, screen):
# 		pygame.draw.circle(screen, self.color, self.position, self.size)

if __name__ == '__main__':
	main()
	