
iw4of = {}

iw4_open_formats = {
	source = path.join("deps", "iw4-open-formats"),
}

iw4of_dependencies = {
	basePath = path.join(iw4_open_formats.source, "deps")
}

function iw4_open_formats.imports()
	for dep_name, dependency in pairs(iw4of) do
		print("importing "..dep_name)
		dependency.import()
	end
end

function iw4_open_formats.import()
	links "iw4-open-formats"

	iw4_open_formats.includes()
	-- iw4_open_formats.imports()
end

function iw4_open_formats.includes()
	includedirs {
		path.join(iw4_open_formats.source, "include")
	}
end

function iw4_open_formats.load()
	dir = path.join("iw4-open-formats", "deps", "premake/*.lua")
	deps = os.matchfiles(dir)

	for i, dep in pairs(deps) do
		dep = dep:gsub(".lua", "")
		require(dep)
	end
end


function iw4_open_formats.projects()
	for dep_name, dependency in pairs(iw4of) do
		dependency.project()
	end
end

iw4_open_formats.load()

function iw4_open_formats.project()
	project "iw4-open-formats"
		language "C++"
		cppdialect "C++latest"

		pchheader "std_include.hpp"
		pchsource (path.join(iw4_open_formats.source, "src/iw4-of/std_include.cpp"))

		files {
			path.join(iw4_open_formats.source, "src/iw4-of/**.hpp"),
			path.join(iw4_open_formats.source, "src/iw4-of/**.cpp"),
		}

		includedirs {
			path.join(iw4_open_formats.source, "src/iw4-of"),
			path.join(iw4_open_formats.source, "include"),
		}

		iw4_open_formats.imports()

		kind "StaticLib"
		
		group "External Dependencies"
		iw4_open_formats.projects()
end
