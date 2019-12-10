shaders_gen = generator(ld, output: '@PLAINNAME@.o', arguments: ['--format', 'binary', '--relocatable', '--output', '@OUTPUT@', '@INPUT@'])
shaders = shaders_gen.process(
    'shader/container.vert', 'shader/container.frag'
)

executable('gtk3gl',
    ['main.c', 'shader_compile.c', 'shader_make.c', shaders],
    include_directories: [glmath_inc],
    dependencies: [m_dep, gtk_dep, gdk_pixbuf_dep, glib_dep, epoxy_dep]
)
