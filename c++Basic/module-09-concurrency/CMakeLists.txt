file(GLOB examples "examples/*.cpp")
file(GLOB exercises "exercises/*.cpp")

foreach(src ${examples})
    get_filename_component(name ${src} NAME_WE)
    add_executable(mod9_ex_${name} ${src})
endforeach()

foreach(src ${exercises})
    get_filename_component(name ${src} NAME_WE)
    add_executable(mod9_todo_${name} ${src})
endforeach()
