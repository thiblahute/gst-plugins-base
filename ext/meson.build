subdir('alsa', components: [plugins_component])
subdir('cdparanoia', components: [plugins_component])
if cc.get_id() != 'msvc'
  # libvisual has MinGW-specific headers which are not compatible with MSVC
  # If we want to build this with MSVC, we must build libvisual with it too
  subdir('libvisual', components: [plugins_component])
endif
subdir('ogg', components: [plugins_component])
subdir('opus', components: [plugins_component])
subdir('pango', components: [plugins_component])
subdir('theora', components: [plugins_component])
subdir('vorbis', components: [plugins_component])
