alsa_sources = [
  'gstalsa.c',
  'gstalsadeviceprobe.c',
  'gstalsamidisrc.c',
  'gstalsaplugin.c',
  'gstalsasink.c',
  'gstalsasrc.c',
]

alsa_comp = component('alsa')
alsa_dep = dependency('alsa', version : '>=0.9.1', required : false, components: alsa_comp)
gstalsa = library('gstalsa',
  alsa_sources,
  c_args : gst_plugins_base_args,
  include_directories: [configinc, libsinc],
  dependencies : glib_deps + [alsa_dep, audio_dep, tag_dep, gst_dep, gst_base_dep],
  install : true,
  install_dir : plugins_install_dir,
  components: alsa_comp
)
