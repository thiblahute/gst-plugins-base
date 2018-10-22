html_theme = 'https://github.com/hotdoc/hotdoc_lumen_theme/releases/download/0.6/hotdoc_lumen_theme-0.6.tar.xz?sha256=0e2f175f4cf8c00ed7ac5014e30c806a294b0d3818565eb74e1424a948e8a452'
hotdoc_p = find_program('hotdoc', required: get_option('doc'))
build_hotdoc = false

if not hotdoc_p.found()
    message('Hotdoc not found, not building the documentation')
    subdir_done()
endif

hotdoc_extension_dir = []
required_hotdoc_extensions = ['gi-extension']
if gst_dep.type_name() == 'internal'
    gst_proj = subproject('gstreamer')
    if gst_proj.get_variable('build_hotdoc')
        hotdoc_extension_dir = [gst_proj.get_variable('hotdoc_extension_dir')]
    endif
else
    required_hotdoc_extensions += ['gst-extension']
endif

hotdoc = import('hotdoc')
if not hotdoc.has_extensions(required_hotdoc_extensions)
    if get_option('doc').enabled()
        error('Documentation enabled but gi-extension missing')
    endif

    message('@0@ extensions not found, not building documentation'.format(required_hotdoc_extensions))
    subdir_done()
endif

if not build_gir
    if get_option('doc').enabled()
        error('Documentation enabled but introspection not built.')
    endif

    message('Introspection not built, can\'t build the documentation')
    subdir_done()
endif

build_hotdoc = true
docconf = configuration_data()
docconf.set('GST_API_VERSION', api_version)

version_entities = configure_file(input : 'version.in',
    output : 'gst_api_version.md',
    configuration : docconf)

excludes = []
foreach h: ['pbutils-private.h', 'gsttageditingprivate.h', 'id3v2.h',
  'kiss_fft_f32.h', 'kiss_fft_f64.h', 'kiss_fftr_f32.h', 'kiss_fftr_f64.h',
  'kiss_fftr_s16.h', 'kiss_fftr_s32.h', 'kiss_fft_s16.h', 'kiss_fft_s32.h',
  '_kiss_fft_guts_f32.h', '_kiss_fft_guts_f64.h', '_kiss_fft_guts_s16.h',
  '_kiss_fft_guts_s16.h', '_kiss_fft_guts_s32.h', '_kiss_fft_guts_s32.h',
  'pbutils-marshal.h', 'audio-resampler-private.h', '*orc-dist.[ch]',
  '*-neon.h', 'audio-resampler-macros.[ch]']

    excludes += [join_paths(meson.current_source_dir(), '..', '..', 'gst-libs/gst/*/', h)]
endforeach

libs = [
  ['allocators', allocators_gir, allocators_dep],
  ['app', app_gir, app_dep],
  ['audio', audio_gir, audio_dep],
  # FIXME! ['fft', fft_gir, fft_dep],
  ['pbutils', pbutils_gir, pbutils_dep],
  ['rtp', rtp_gir, rtp_dep],
  ['rtsp', rtsp_gir, rtsp_dep],
  ['sdp', sdp_gir, sdp_dep],
  ['tag', tag_gir, tag_dep],
  ['video', video_gir, video_dep],
]

if build_gstgl
  libs + [['gl', gl_gir, gstgl_dep]]
endif

libs_doc = []
foreach lib: libs
  name = lib[0]
  gir = lib[1]
  deps = [lib[2], gir]
  libs_doc += [hotdoc.generate_doc('gst-plugins-base-' + name,
      project_version: api_version,
      gi_c_sources: ['../gst-libs/gst/' + name + '/*.[hc]'],
      gi_sources: gir[0].full_path(),
      gi_c_source_filters: excludes,
      gi_c_source_roots: [join_paths(meson.current_source_dir(), '../gst-libs/gst/' + name), ],
      sitemap: 'libs/' + name + '/sitemap.txt',
      index: 'libs/' + name + '/index.md',
      gi_index: 'libs/' + name + '/index.md',
      gi_smart_index: true,
      gi_order_generated_subpages: true,
      dependencies: deps,
      install: false,
      html_theme: html_theme,
  )]
endforeach

if not hotdoc.has_extensions('c-extension')
    if get_option('doc').enabled()
        error('Documentation enabled but c-extension missing')
    endif
    message('c-extension not found, not building documentation')
else
    libs_doc += [hotdoc.generate_doc('gst-plugins-base-riff',
        project_version: api_version,
        c_sources: ['../gst-libs/gst/riff/*.[hc]'],
        c_source_filters: excludes,
        sitemap: 'libs/riff/sitemap.txt',
        index: 'libs/riff/index.md',
        c_index: 'libs/riff/index.md',
        c_smart_index: true,
        c_order_generated_subpages: true,
        dependencies: [gst_base_dep, riff_dep],
        install: false,
        disable_incremental_build: true,
        html_theme: html_theme,
    )]
endif

plugins_doc = []
sitemap = 'all_index.md\n'

cache_file = join_paths(meson.current_source_dir(), 'plugins', 'cache.json')
list_plugin_res = run_command(python3, '-c',
'''
import sys
import json

try:
    with open("@0@") as f:
        print(':'.join(json.load(f).keys()), end='')
except FileNotFoundError:
    print("fakery", end='')
    pass
'''.format(cache_file))

assert(list_plugin_res.returncode() == 0,
  'Could not list plugins from @0@\n@1@\n@1@'.format(cache_file, list_plugin_res.stdout(), list_plugin_res.stderr()))

all_plugins_libs = []
all_plugins_paths = []
foreach l: plugins
    all_plugins_libs += [l]
    all_plugins_paths += l.full_path()
endforeach

message(list_plugin_res.stdout())
foreach plugin_name: list_plugin_res.stdout().split(':')
    plugins_doc += [hotdoc.generate_doc(plugin_name,
        project_version: api_version,
        sitemap: 'plugins/sitemap.txt',
        index: 'plugins/index.md',
        gst_index: 'plugins/index.md',
        gst_smart_index: true,
        gst_c_sources: ['../sys/*/*.[ch]',
                        '../ext/*/*.[ch]',
                        '../gst/*/*.[ch]',
                        ],
        gst_dl_sources: all_plugins_paths,
        dependencies: [gst_dep, all_plugins_libs],
        gst_order_generated_subpages: true,
        extra_extensions_paths: hotdoc_extension_dir,
        install: false,
        disable_incremental_build: true,
        gst_cache_file: join_paths(meson.current_source_dir(), 'plugins', 'cache.json'),
        gst_plugin_name: plugin_name,
        html_theme: html_theme,
    )]
    sitemap += '        @0@-doc.json\n'.format(plugin_name)
endforeach