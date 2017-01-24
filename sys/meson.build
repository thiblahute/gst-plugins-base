# XShm is only used by ximage and xvimage
# FIXME: Need to check for XShmAttach inside libXext
xshm_dep = dependency('xext', required : false)
core_conf.set('HAVE_XSHM', xshm_dep.found())

subdir('ximage', components: [plugins_component,
    component('ximage', dependencies: [x11_dep])])
subdir('xvimage', components: [plugins_component,
    component('xvimage', dependencies: [x11_dep])])
