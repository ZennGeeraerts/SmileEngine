from conan import ConanFile
from conan.tools.files import copy

class SmileEngine(ConanFile):
	settings = ("os", "compiler", "build_type", "arch")
	generators = ("CMakeToolchain", "CMakeDeps")

	def requirements(self):
		self.requires("box2d/2.4.1")
		self.requires("glad/0.1.36")
		self.requires("sdl/2.30.5")
		self.requires("physx/4.1.2")
		self.requires("fmt/11.1.1")

	def build_requirements(self):
		self.tool_requires("cmake/[>=3.25]")

	def configure(self):
		self.options["glad/*"].gl_profile = "core"
		self.options["glad/*"].gl_version = "4.6"
		self.options["physx"].shared = True

	def generate(self):
		# NOTE: In most cases it is not necessary to copy the shared libraries
		# of dependencies to use them. Conan environment generators that create
		# environment scripts allow to use the shared dependencies without copying
		# them to the current location
		for dep in self.dependencies.values():
			# This code assumes dependencies will only have 1 libdir/bindir, if for some
			# reason they have more than one, it will fail. Use ``dep.cpp_info.libdirs``
			# and ``dep.cpp_info.bindirs`` lists for those cases.
			# In Windows, dlls are in the "bindir", not "libdir"
			copy(self, "*.dll", dep.cpp_info.bindir, self.build_folder)