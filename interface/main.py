import sys, pygame
import pygame.gfxdraw
from enum import Enum, auto
from classes.GameObject import GameObject
from classes.gui.Gui import Gui
from classes.gui.Button import Button
from classes.Player import Player
from classes.gui.TextBox import TextBox

BOARD_SIZE = 600 # board size in pixels (determines window size)

def main():
	game = Game()
	game.game_loop()



class Game:
	"""
	Creates a Game

	Args:
		line_num (int): Number of lines on the grid

	Attributes:
		players (Player[]): list of players
	"""
	ratio = 3 / 2

	def __init__(self, line_num=19):
		self.line_num = line_num # number of lines vertically and horizontally on board

		pygame.init()
		pygame.display.set_caption('Gomoku')
		# create space for gui on right
		self.screen = pygame.display.set_mode([int(BOARD_SIZE * Game.ratio), BOARD_SIZE])
		self.background = Background(line_num)

		self.turn = 1 # 1 is black, 2 is white
		self.players = [Player(Player.TYPE.AI), Player(Player.TYPE.HUMAN)] # set one player to AI another to Human

		self.init_gui()

		self.intersections = Intersections(self.background.space_between_lines, line_num, self.background.line_width, size=self.background.space_between_lines)
		self.game_objects = [
			self.background, self.intersections, self.gui
		]

	def init_gui(self):
		self.gui = Gui((BOARD_SIZE, 0), (BOARD_SIZE * Game.ratio - BOARD_SIZE, BOARD_SIZE), background_color=(220, 220, 220), border_color=(150, 150, 150), border_width=5)
		playerVAIwrapper = Gui((5, 2.5), (90, 20), background_color=(200, 200, 200), border_color=(150, 150, 150), border_width=5);
		
		player1Btn = Button((5, 5), (30, 90), background_color=(0, 200, 0), border_color=(250, 250, 250), border_width=5, font_size=20, color=(250, 250, 250));
		player1Btn.on_click_callback = lambda : self.players[0].change_type();
		player2Btn = Button((65, 5), (30, 90), background_color=(0, 200, 0), border_color=(0, 0, 0), border_width=5, font_size=20, color=(0, 0, 0));
		player2Btn.on_click_callback = lambda : self.players[1].change_type();

		vsTextBox = TextBox((40, 40), (20, 20), text='vs', background_color=(220, 220, 220), color=(250, 0, 0))

		playerVAIwrapper.insert(player1Btn)
		playerVAIwrapper.insert(vsTextBox)
		playerVAIwrapper.insert(player2Btn)
		self.gui.insert(playerVAIwrapper)

		def update_button(button, player):
			if player.is_AI():
				button.textbox.text = 'AI'
				button.textbox.background_color = (200, 50, 50)
			elif player.is_human():
				button.textbox.text = 'Player'
				button.textbox.background_color = (50, 50, 200)

		self.players[0].on_change_type = lambda: update_button(player1Btn, self.players[0])
		self.players[1].on_change_type = lambda: update_button(player2Btn, self.players[1])

		# reset type to update buttons
		self.players[0].type = self.players[0].type
		self.players[1].type = self.players[1].type

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
			if event.type == pygame.MOUSEBUTTONUP:
				rect = self.intersections.clicked_on((pygame.mouse.get_pos()))
				if rect:
					self.intersections.remove(rect)
					placed = self.place_stone(rect.center)
					if placed:
						self.next_turn()
			
			self.gui.handle_event(event, self.screen)

	def render_game_objects(self):
		for game_object in self.game_objects:
			game_object.render(self.screen)
		pygame.display.flip()
	
	def place_stone(self, position):
		# TODO: call model/controller to handle rules preventing you from placing in certain places
		self.game_objects.append(Stone((240, 240, 240) if self.turn == 2 else (20, 20, 20), position, self.background.space_between_lines/2.2))
		return True
	
	def next_turn(self):
		self.turn = 1 if self.turn == 2 else 2
		print('next turn: ', self.turn)

class Background(GameObject):
	def __init__(self, line_num=19, background_color=(133, 193, 233), line_color=(21, 67, 96), line_width=2):
		# init background
		self.surface = pygame.Surface([BOARD_SIZE, BOARD_SIZE]).convert()
		self.surface.fill(background_color)
		# init lines
		self.space_between_lines = BOARD_SIZE / (line_num + 1)
		self.line_width = line_width
		for i in range(line_num):
			i += 1
			dist = i * self.space_between_lines # distance from edge
			pygame.draw.line(self.surface, line_color, (dist, 0), (dist, BOARD_SIZE), line_width)
			pygame.draw.line(self.surface, line_color, (0, dist), (BOARD_SIZE, dist), line_width)
		self.position = (0, 0)
	def render(self, screen):
		screen.blit(self.surface, self.position)

class Intersections(GameObject):
	def __init__(self, space_between_lines, line_num, line_width, size=50, hover_color=(250, 250, 250, 50)):
		self.hover_color = hover_color

		# set intersection positions
		self.rects = []
		for x in range(line_num):
			x += 1
			for y in range(line_num):
				y += 1
				pos = (x * space_between_lines - size / 2 + line_width / 2, y * space_between_lines - size / 2 + line_width / 2)
				self.rects.append(pygame.Rect(pos, (size, size)))

	# handle hover over rect
	def render(self, screen):
		for rect in self.rects:
			if rect.collidepoint(pygame.mouse.get_pos()):
				pygame.gfxdraw.box(screen, rect, self.hover_color)

	def clicked_on(self, pos):
		for rect in self.rects:
			if rect.collidepoint(pygame.mouse.get_pos()):
				return rect
		return None

	def remove(self, rect):
		self.rects.remove(rect)

class Stone(GameObject):
	def __init__(self, color, position, size):
		self.color = color
		self.position = position
		self.size = int(size)

	def render(self, screen):
		pygame.draw.circle(screen, self.color, self.position, self.size)

if __name__ == '__main__':
	main()
	