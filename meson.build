project('bitcount', 'c')

project_sources = []

inc = include_directories('include')

subdir('src')

thread_dep = dependency('threads')

library('bitcount', project_sources, include_directories: inc,
	dependencies: thread_dep)
