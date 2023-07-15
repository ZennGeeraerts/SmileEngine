require("../thirdparty/premake/premake5-cuda/premake5-cuda")

project "SmileRaster"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/bin-int/" .. outputdir .. "/%{prj.name}")
	cudaIntDir ("%{wks.location}/build/bin-int/" .. outputdir .. "/%{prj.name}")

	includedirs
	{
		"%{wks.location}/smile-raster",
		"%{wks.location}/smile-raster/src"
	}

	files
	{
		"src/**.cuh",
		"src/**.cu"
	}

	buildcustomizations "BuildCustomizations/CUDA 11.5"

	if os.target() == "windows" then
		cudaPath "$(CUDA_PATH)"
		cudaFiles
		{
			"src/**.cuh",
			"src/**.cu"
		} -- files to be compiled into binaries by VS CUDA.
		--cudaPTXFiles {  } -- files to be compiled into ptx, Windows only.
	else
		toolset "nvcc"
		cudaPath "/usr/local/cuda"
		files 
		{ 
			"src/**.cuh",
			"src/**.cu"
		}
		rules {"cu"}
	end

	cudaRelocatableCode "On"
	cudaCompilerOptions { "-arch=sm_52", "-t0" }

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		cudaLinkerOptions { "-g" }
		defines "SM_C_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		cudaFastMath "On"
		cudaGenLineInfo "On"
		defines "SM_C_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SM_C_DIST"
		runtime "Release"
		optimize "on"