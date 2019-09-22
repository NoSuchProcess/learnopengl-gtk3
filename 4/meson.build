project('gtk3gl', 'c', version: '1.0')
add_project_arguments(['-Wno-unused-parameter'], language: 'c')

gtk_dep = dependency('gtk+-3.0')
glib_dep = dependency('glib-2.0')

executable('gtk3gl', ['main.c'], dependencies: [gtk_dep, glib_dep])
