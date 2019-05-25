import sys, pygame
import pygame.gfxdraw

SCREEN_SIZE = 600


def main():
	game = Game()
	game.game_loop()

class Game:

	def __init__(self, board_size=19):
		print('init')
		self.board_size = board_size # number of lines vertically and horizontally

		pygame.init()
		pygame.display.set_caption('Gomoku')
		self.screen = pygame.display.set_mode([SCREEN_SIZE, SCREEN_SIZE])
		self.background = Background(board_size)
		self.intersections = Intersections(self.background.space_between_lines, board_size, self.background.line_width, size=self.background.space_between_lines)
		self.game_objects = [
			self.background, self.intersections
		]
		
		self.turn = 1 # 1 is black, 2 is white

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

	def render_game_objects(self):
		for game_object in self.game_objects:
			game_object.render(self.screen)
		pygame.display.flip()
	
	def place_stone(self, position):
		# TODO: handle rules preventing you from placing in certain places
		self.game_objects.append(Stone((240, 240, 240) if self.turn == 2 else (20, 20, 20), position, self.background.space_between_lines/2.2))
		return True
	
	def next_turn(self):
		self.turn = 1 if self.turn == 2 else 2
		print('next turn: ', self.turn)

class GameObject:
	def render(self, screen): pass

class Background(GameObject):
	def __init__(self, board_size=19, background_color=(133, 193, 233), line_color=(21, 67, 96), line_width=2):
		# init background
		self.surface = pygame.Surface([SCREEN_SIZE, SCREEN_SIZE]).convert()
		self.surface.fill(background_color)
		# init lines
		self.space_between_lines = SCREEN_SIZE / (board_size + 1)
		self.line_width = line_width
		for i in range(board_size):
			i += 1
			dist = i * self.space_between_lines # distance from edge
			pygame.draw.line(self.surface, line_color, (dist, 0), (dist, SCREEN_SIZE), line_width)
			pygame.draw.line(self.surface, line_color, (0, dist), (SCREEN_SIZE, dist), line_width)
		self.position = (0, 0)
	def render(self, screen):
		screen.blit(self.surface, self.position)

class Intersections(GameObject):
	def __init__(self, space_between_lines, board_size, line_width, size=50, hover_color=(250, 250, 250, 50)):
		self.hover_color = hover_color

		# set intersection positions
		self.rects = []
		for x in range(board_size):
			x += 1
			for y in range(board_size):
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
	