from conan import ConanFile

class SmileEngine(ConanFile):
	settings = ("os", "compiler", "build_type", "arch")
	generators = ("CMakeToolchain", "CMakeDeps")

	def requirements(self):
		self.requires("box2d/2.4.1")

	def build_requirements(self):
		self.tool_requires("cmake/[>=3.25]")