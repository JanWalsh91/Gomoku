# TODO: integrate Background and Intersections and Stone into Board class

from GameObject import GameObject

class Board(GameObject):
	background_color1 = (133, 193, 233)
	background_color2 = (123, 183, 223)
	line_color = (21, 67, 96)
	line_width = 2

	def __init__(self, board_size=19):


class Background(GameObject):
	def __init__(self, board_size=19, background_color=(133, 193, 233), line_color=(21, 67, 96), line_width=2):
		# init background
		self.surface = pygame.Surface([BOARD_SIZE, BOARD_SIZE]).convert()
		self.surface.fill(background_color)
		# init lines
		self.space_between_lines = BOARD_SIZE / (board_size + 1)
		self.line_width = line_width
		for i in range(board_size):
			i += 1
			dist = i * self.space_between_lines # distance from edge
			pygame.draw.line(self.surface, line_color, (dist, 0), (dist, BOARD_SIZE), line_width)
			pygame.draw.line(self.surface, line_color, (0, dist), (BOARD_SIZE, dist), line_width)
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