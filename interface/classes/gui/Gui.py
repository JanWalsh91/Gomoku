import numpy as np
import pygame
from classes.GameObject import GameObject

# all giu elements are classes inerited from Gui
class Gui(GameObject):
	"""
	Creates a Gui element
	Arguments:
		position ((left, top))
		dimensions ((width, height))
		background_color? (color)
		border_color? (color)
		border_width? (int)
	"""

	# position: [left, top]
	# dimensions: [width, height]
	def __init__(self, position=(0, 0), dimensions=(100, 100), background_color=(250, 250, 250), border_color=(100, 100, 100), border_width=0):
		# print('init', self, 'bg', background_color)
		self.__border_width = None
		self.__border_color = None
		self.__background_color = None

		self.position = np.array(position)
		self.dimensions = np.array(dimensions)
		self.surface = pygame.Surface(self.dimensions)
		self.children = []
		self.parent = None
		self.background_color = background_color
		self.border_color = border_color
		self.border_width = border_width

	def render(self, screen):
		screen.blit(self.surface, self.abs_position)
		for child in self.children:
			child.render(screen)

	def insert(self, gui):
		if not isinstance(gui, Gui) or gui in self.children:
			return False
		self.children.append(gui)
		gui.parent = self;
		self.scale_children()
		gui.redraw()

	def scale_children(self):
		if len(self.children) == 0:
			return
		rect = self.surface.get_rect()
		for i, child in enumerate(self.children):
			child.surface = pygame.Surface(child.abs_dimensions)
			if (child.background_color):
				child.background_color = child.background_color
			child.scale_children()

	def redraw(self):
		if not self.surface:
			return
		# background_color
		self.surface.fill(self.background_color)
		# border
		if self.border_width and self.border_width > 0 and self.border_color:
			pygame.draw.rect(self.surface, self.border_color, self.surface.get_rect(), self.__border_width)
	
	def handle_event(self, event, screen):
		if len(self.children) > 0:
			for child in self.children:
				child.handle_event(event, screen)

	@property
	def background_color(self):
		return self.__background_color
	@background_color.setter
	def background_color(self, background_color):
		self.__background_color = background_color
		self.redraw()

	@property
	def parent(self):
		return self.__parent
	@parent.setter
	def parent(self, parent):
		if parent and not isinstance(parent, Gui):
			raise Exception('Parent must be Gui or subclass')
		self.__parent = parent

	@property
	def position(self):
		return self.__position
	@position.setter
	def position(self, position):
		if not isinstance(position, (tuple, np.ndarray)) or len(position) != 2:
			raise Exception('Position must be tuple of length 2')
		self.__position = position

	@property
	def abs_position(self):
		if self.parent:
			node = self
			abs_position = node.parent.abs_position + node.parent.abs_dimensions * self.position / 100
			return abs_position.astype(int)
		else:
			return self.position
	
	@property
	def abs_dimensions(self):
		abs_dimensions = self.dimensions.astype(float)
		if self.parent:
			node = self
			abs_dimensions *= (node.parent.abs_dimensions / 100)
			node = node.parent
			return abs_dimensions.astype(int)
		else:
			return abs_dimensions.astype(int)
	
	@property
	def border_color(self):
		return self.__border_color
	@border_color.setter
	def border_color(self, border_color):
		self.__border_color = border_color
		self.redraw()

	@property
	def border_width(self):
		return self.__border_width
	@border_width.setter
	def border_width(self, border_width):
		self.__border_width = border_width
		self.redraw()

	@property
	def background_color(self):
		return self.__background_color
	@background_color.setter
	def background_color(self, background_color):
		self.__background_color = background_color
		self.redraw()