from conan import ConanFile

class SmileEngine(ConanFile):
	settings = ("os", "compiler", "build_type", "arch")
	generators = ("CMakeToolchain", "CMakeDeps")

	def requirements(self):
		self.requires("box2d/2.4.1")
		self.requires("glad/0.1.36")
		self.requires("sdl/2.30.5")

	def build_requirements(self):
		self.tool_requires("cmake/[>=3.25]")

	def configure(self):
		self.options["glad/*"].gl_profile = "core"
		self.options["glad/*"].gl_version = "4.6"