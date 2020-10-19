project('bitcount', 'c')

project_sources = []
inc = include_directories('include')
subdir('src')

thread_dep = dependency('threads')

bitcount = static_library('bitcount', project_sources, include_directories: inc,
	dependencies: thread_dep)

bitcount_dep = declare_dependency(include_directories: inc,
	link_with: bitcount)

