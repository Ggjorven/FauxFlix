------------------------------------------------------------------------------
-- Utils
------------------------------------------------------------------------------
local function local_require(path)
	return dofile(path)
end

local function this_directory()
    local str = debug.getinfo(2, "S").source:sub(2)
	local path = str:match("(.*/)")
    return path:gsub("\\", "/") -- Replace \\ with /
end

function append_to_table(dest, value)
	if type(value) == "table" then
		for _, v in ipairs(value) do
        	table.insert(dest, v)
    	end
    else
		table.insert(dest, value)
    end

	return dest
end

function remove_from_table(dest, filter)
    for i = #dest, 1, -1 do  -- Iterate backwards
        local value = dest[i]

		-- Note: Allows lua patterns
        if string.find(value, filter) ~= nil then
            table.remove(dest, i)
        end
    end

	return dest
end

function copy_table(tbl)
    if type(tbl) ~= "table" then 
		return tbl 
	end

    local copy = {}
    for k, v in pairs(tbl) do
        copy[k] = copy_table(v)
    end
	
    return copy
end
------------------------------------------------------------------------------

------------------------------------------------------------------------------
-- Graphics API
------------------------------------------------------------------------------
OBSIDIAN_GRAPHICS_API = OBSIDIAN_GRAPHICS_API or "vulkan" 
------------------------------------------------------------------------------

------------------------------------------------------------------------------
-- Dependencies
------------------------------------------------------------------------------
local Dependencies =
{
    -- External dependencies
    Nano = 
    {
        --IncludeDir = local_require("Vendor/Photon/Photon/Dependencies.lua").Nano.IncludeDir
        IncludeDir = this_directory() .. "Vendor/Nano/Nano/Nano/include",
        Defines = { "NANO_EXPERIMENTAL" }
    },
	Obsidian = local_require("Vendor/Obsidian/Obsidian/Dependencies.lua").Obsidian,

    FFmpeg = {},

    -- Internal dependencies
    Faux = 
    {
		IncludeDir = {},
        LibDir = {},
        LibName = {},
        PostBuildCommands = {},
        Defines = {},

        BuildOptions = {},
        LinkOptions = {},
    }
}
------------------------------------------------------------------------------

------------------------------------------------------------------------------
-- Append Dependencies
------------------------------------------------------------------------------
-- IncludeDirs
append_to_table(Dependencies.Faux.IncludeDir, this_directory() .. "FauxFlix/Source/")
append_to_table(Dependencies.Faux.IncludeDir, Dependencies.Nano.IncludeDir)
append_to_table(Dependencies.Faux.IncludeDir, remove_from_table(Dependencies.Obsidian.IncludeDir, "Nano"))

-- LibDir
append_to_table(Dependencies.Faux.LibDir, Dependencies.Obsidian.LibDir)

-- LibNames
append_to_table(Dependencies.Faux.LibName, "FauxFlix")
append_to_table(Dependencies.Faux.LibName, Dependencies.Obsidian.LibName)

-- PostBuildCommands
append_to_table(Dependencies.Faux.PostBuildCommands, Dependencies.Obsidian.PostBuildCommands)

-- Defines
append_to_table(Dependencies.Faux.Defines, Dependencies.Nano.Defines)

if OBSIDIAN_GRAPHICS_API == "vulkan" then
    append_to_table(Dependencies.Faux.Defines, "OB_API_VULKAN")
elseif OBSIDIAN_GRAPHICS_API == "dx12" then
    append_to_table(Dependencies.Faux.Defines, "OB_API_DX12")
else
    error("Invalid API")
end

-- BuildOptions
if os.target() == "linux" then
    append_to_table(Dependencies.Faux.LinkOptions, "`pkg-config --cflags libavcodec libavformat libavutil libswscale libswresample`")
end

-- LinkOptions
if os.target() == "linux" then
    append_to_table(Dependencies.Faux.LinkOptions, "`pkg-config --libs libavcodec libavformat libavutil libswscale libswresample`")
end
------------------------------------------------------------------------------

return Dependencies