pango_sources = [
  'gstbasetextoverlay.c',
  'gstclockoverlay.c',
  'gsttextoverlay.c',
  'gsttextrender.c',
  'gsttimeoverlay.c',
]
pango_comp = component('pango')
pango_dep = dependency('pangocairo', version : '>=1.22.0', required : false,
  components: pango_comp)

gstpango = library('gstpango',
  pango_sources,
  c_args : gst_plugins_base_args,
  link_args : noseh_link_args,
  include_directories: [configinc, libsinc],
  dependencies : glib_deps + [pango_dep, video_dep, gst_dep, gst_base_dep, libm],
  install : true,
  install_dir : plugins_install_dir,
  components: pango_comp
)
