option(BUILD_EXAMPLES "defines if the examples will be built" ON)
option(SUI_SHARED "defines if the library is shared(exclusive of static)" ON)
option(SUI_STATIC "defines if the library is static(exclusive of shared)" OFF)

# make shared by default and static if SUI_STATIC is true
# If both are set then static wins
if(SUI_STATIC)
	set(SUI_BUILD_TYPE STATIC)
	set(SUI_SHARED OFF)
	set(SUI_STATIC ON)
elseif(SUI_SHARED)
	set(SUI_BUILD_TYPE SHARED)
	set(SUI_SHARED ON)
	set(SUI_STATIC OFF)
else()
	message(FATAL_ERROR "Both SUI_SHARED and SUI_STATIC are OFF! Please set one of them.")
endif()


if(NOT SUI_LIBS)
	message(FATAL_ERROR "SUI_LIBS not defined. See Build.txt for info on that variable")
endif()
