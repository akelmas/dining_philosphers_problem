project('dining_philosophers', 'c',
  version : '0.2',
  default_options : ['warning_level=3'])
cc=meson.get_compiler('c')
if cc.get_id() != 'clang'
  thread_dep = dependency('threads')
  if thread_dep.found()
    add_project_link_arguments('-lpthread', language : 'c')
  endif
  bsd_dep = dependency('libbsd')
  if bsd_dep.found()
    add_project_link_arguments('-lbsd', language : 'c')
  endif
  m_dep = cc.find_library('m', required : false)
  if m_dep.found()
    add_project_link_arguments('-lm', language : 'c')
  endif
endif
incdir = include_directories('inc')
subdir('src')
exe = executable('dining_philosophers', 
  sources,
  include_directories : incdir,
  install : true)
test('basic', exe)
